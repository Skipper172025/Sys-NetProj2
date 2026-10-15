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
        // TODO(Part B): lock here so only one thread reads/increments COUNTER
        std::lock_guard<std::mutex> lock(counterMutex);

        long myNumber = COUNTER;
        COUNTER++;
        // TODO(Part B): unlock here

        // stop once every number up to and including N has been claimed
        if (myNumber > N) {
            break;
        }

        int stoppingTime = computeStoppingTime(myNumber);

        // TODO(Part B): lock here so only one thread updates the histogram
        if (stoppingTime <= MAX_STOP) {
            std::lock_guard<std::mutex> lock(histogramMutex);
            
            histogram[stoppingTime]++;
        }
        // TODO(Part B): unlock here
    }
}


int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) {
        std::cerr << "Usage: " << argv[0] << " N T [-nolock]" << std::endl;
        return 1;
    }

    N = std::atol(argv[1]);
    int T = std::atoi(argv[2]);

    if (N < 1 || T < 1) {
        std::cerr << "N and T must be positive integers" << std::endl;
        return 1;
    }

    // TODO(Part B): start the timer here

    std::vector<std::thread> threads;
    for (int i = 0; i < T; i++) {
        threads.push_back(std::thread(worker));
    }

    for (int i = 0; i < T; i++) {
        threads[i].join();
    }

    // TODO(Part B): stop the timer and print N,T,time to stderr here

    // print the histogram
    for (int k = 0; k <= MAX_STOP; k++) {
        std::cout << k << "," << histogram[k] << std::endl;
    }

    return 0;
}