/**
 *  mt-collatz.cpp
 *  Sarah Pierpoint
 *  This file implements a multi-threaded Collatz stopping time generator. It
 *  launches T worker threads that each claim numbers from 1 to N, compute
 *  their Collatz stopping times, and tally them in a shared histogram that is
 *  printed to standard output as "k,frequency" lines.
 */

#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <cstdlib>
#include <mutex>
#include <cstring>
#include <ctime>
#include <iomanip>


const int MAX_STOP = 1000;

// Global shared data
std::mutex counterMutex;
std::mutex histogramMutex;
long COUNTER = 1;
long N = 0;

bool useLock = true;
long histogram[MAX_STOP + 1] = {0};

//Collatz computation starts here

int computeStoppingTime(long n) {
    int steps = 0;
    unsigned long long value = n;

    while (value != 1) {
        if (value % 2 == 0) {
            value = value / 2;
        } else {
            value = 3 * value + 1;
        }
        steps++;
    }
    return steps;
}

//worker thread

void worker() {
    while (true) {
        long myNumber;

        // Protect the shared counter
        if (useLock) {
            std::lock_guard<std::mutex> lock(counterMutex);

            myNumber = COUNTER;
            COUNTER++;
        }
        else {
            myNumber = COUNTER;
            COUNTER++;
        }

        // Stop when all numbers have been claimed
        if (myNumber > N) {
            break;
        }

        // Compute without holding the counter mutex
        int stoppingTime = computeStoppingTime(myNumber);

        // Protect the shared histogram
        if (stoppingTime <= MAX_STOP) {
            if (useLock) {
                std::lock_guard<std::mutex> lock(histogramMutex);
                histogram[stoppingTime]++;
            }
            else {
                histogram[stoppingTime]++;
            }
        }
    }
}


int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) {
        std::cerr << "Usage: " << argv[0] << " N T [-nolock]" << std::endl;
        return 1;
    }

    N = std::atol(argv[1]);
    int T = std::atoi(argv[2]);

    //Disable mutex locking if -nolock is provided
    if (argc ==4) {
        if (std::strcmp(argv[3], "-nolock") == 0) {
            useLock = false;
        }
        else {
            std::cerr << "Invalid option: " << argv[3] << std::endl;
            return 1;
        }
    }

    if (N < 1 || T < 1) {
        std::cerr << "N and T must be positive integers" << std::endl;
        return 1;
    }

    struct timespec startTime, endTime;
    clock_gettime(CLOCK_MONOTONIC, &startTime);

    std::vector<std::thread> threads;
    for (int i = 0; i < T; i++) {
        threads.push_back(std::thread(worker));
    }

    for (int i = 0; i < T; i++) {
        threads[i].join();
    }

    clock_gettime(CLOCK_MONOTONIC, &endTime);
    long seconds = endTime.tv_sec - startTime.tv_sec;
    long nanoseconds = endTime.tv_nsec - startTime.tv_nsec;

    if (nanoseconds < 0) {
        seconds--;
        nanoseconds += 1000000000L;
    }

    std::cerr << N << "," << T << "," << seconds << "." <<
    std::setfill('0') << std::setw(9) << nanoseconds << std::endl;

    // print the histogram
    for (int k = 0; k <= MAX_STOP; k++) {
        std::cout << k << "," << histogram[k] << std::endl;
    }

    return 0;
}