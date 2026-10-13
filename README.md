README - COP4634 Project 2: Multi-Threaded Collatz Stopping Time Generator

Authors: Sarah Pierpoint, Melanie Vogt

DESCRIPTION
mt-collatz computes a histogram of Collatz stopping times for the numbers
1 through N using T threads. Each thread claims the next unclaimed number
from a shared counter, computes its stopping time, and records it in a
shared histogram array. The histogram is printed to stdout as "k,frequency"
lines for k = 0 to 1000.
[MELANIE: add a sentence about the elapsed-time output on stderr and the
mutex locking / -nolock option once they are in the program.]

FILES
mt-collatz.cpp   Source code
Makefile         Builds the program (uses -g -Wall; includes a clean rule)
histogram.xlsx   Histogram of Collatz stopping times for N = 1,000,000
[MELANIE: add report.pdf, the runtime spreadsheet/tab, and any benchmark
script once they exist.]

BUILD AND RUN
make
./mt-collatz N T

Example: ./mt-collatz 1000000 4
To save the histogram to a file: ./mt-collatz 1000000 1 > histogram.csv
To remove the executable: make clean
[MELANIE: add how to use -nolock, how timing is redirected, and how to run
the benchmark script.]

DIVISION OF WORK
Sarah: Collatz computation, shared counter and histogram, worker thread
function, thread creation and joining, command-line parsing, histogram
output, histogram spreadsheet.
Melanie: [fill in once finished: mutex locking, -nolock option, elapsed-time
measurement, benchmark script, runtime spreadsheet.]

PROBLEMS ENCOUNTERED
- Development was done on Windows, where "make" is not available in
  PowerShell. We used WSL (Ubuntu) to build and test, and confirmed the
  program compiles with no warnings on the department Linux server.
- Before locks were added, running with multiple threads gave incorrect
  histograms because of a race condition. For example, N = 100000 with 4
  threads totaled 110584 counts, and N = 10000000 with 8 threads totaled
  11855169, instead of N. Two threads can read the same value of COUNTER
  before either increments it, so the same number is counted twice.
- [MELANIE: add any problems from the locking and timing work, and how the
  race condition was fixed.]
