README - COP4634 Project 2: Multi-Threaded Collatz Stopping Time Generator

Authors: Sarah Pierpoint, Melanie Vogt

DESCRIPTION
mt-collatz computes a histogram of Collatz stopping times for the numbers
1 through N using T threads. Each thread claims the next unclaimed number
from a shared counter, computes its stopping time, and records it in a
shared histogram array. The histogram is printed to stdout as "k,frequency"
lines for k = 0 to 1000.
By default the program uses mutex locks to protect the shared counter and the histogram from race conditions.
The optional -nolock argument disables synchronization for experimental comparison.
The program also measures elapsed execution time and prints it to stderr in the format N, T, seconds.nanoseconds, keeping the histogram output separate.

FILES
mt-collatz.cpp   Source code
Makefile         Builds the program (uses -g -Wall; includes a clean rule)
histogram.xlsx   Histogram of Collatz stopping times for N = 1,000,000
benchmark.sh     Runs performance experiments for 1-8 threads, with and without mutex locks
benchmark.csv    Individual runtime measurements from the Linux server
report.pdf       Experimental setup, results, disscusion, and conclusions
README           Program documentation and build instructions

BUILD AND RUN
make
./mt-collatz N T

Example: ./mt-collatz 1000000 4
To save the histogram to a file: ./mt-collatz 1000000 1 > histogram.csv
To remove the executable: make clean
To disable mutex synchronization for testing:
./mt-collatz 1000000 4 -nolock
To save the histogram and timing info to separate files on Linux:
./mt-collatz 1000000 4 > histogram.csv 2> timing.txt
The histogram is written to standard output, while the elapsed time is written to standard error.
To run the preformance experiments:
bash benchmark.sh
The benchmark script tests 1-8 threads, with and without locks, with 10 repetitions per configuration using N=1,000,000. Results are saved to benchmark.csv

DIVISION OF WORK
Sarah: Collatz computation, shared counter and histogram, worker thread
function, thread creation and joining, command-line parsing, histogram
output, histogram spreadsheet.
Melanie: Implemented mutex synchronization for the shared counter and histogram, added the optional -nolock mode, implemented elapsed-time measurement, created and tested benchmarking script, collected and analyzed performance results, contributed to the runtime spreadsheet, testing, and reporting.

PROBLEMS ENCOUNTERED
- Development was done on Windows, where "make" is not available in
  PowerShell. We used WSL (Ubuntu) to build and test, and confirmed the
  program compiles with no warnings on the department Linux server.
- Before locks were added, running with multiple threads gave incorrect
  histograms because of a race condition. For example, N = 100000 with 4
  threads totaled 110584 counts, and N = 10000000 with 8 threads totaled
  11855169, instead of N. Two threads can read the same value of COUNTER
  before either increments it, so the same number is counted twice.
- During synchronization testing the counter mutex was originally held for the entire worker-loop iteration, which unnecessarily prevented threads drom performing collatz calculations concurrently. This was corrected by limiting the lock's scope to the counter read and increment operations.
- Mutexes were added to protect both the shared counter and histogram updates. On the server the synchronized 5-thread run for N=500,000 produced the correct total of 500,00 counts and matched the single-thread reference. But an unsynchronized 5-thread run had 670,184 counts, thus demonstrating the effects of race conditions.
- PowerShell initially treated redirected standard-error output as a NativeCommandError. Testing on the server confirmed that histogram output and timing info could be redirected into separate files correctly.
- Benchmarking showed that additional threads did not always reduce runtime. Mutex contention, thread-management overhead, and the shared server environment were considered when analyzying the results.
