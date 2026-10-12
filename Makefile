# Makefile for COP4634 Project 2: mt-collatz

# compiler and flags
CXX = g++
CXXFLAGS = -g -Wall -std=c++11 -pthread

# name of the final program
TARGET = mt-collatz

all: $(TARGET)

# link the source file 
$(TARGET): mt-collatz.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) mt-collatz.cpp


clean:
	rm -f $(TARGET) *.o


.PHONY: all clean