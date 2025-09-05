CXX = g++
CXXFLAGS = -std=c++17 -Wall -Werror -I../include

ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
else
    DETECTED_OS := $(shell uname -s)
endif

ifeq ($(DETECTED_OS),Darwin)
	CXXFLAGS += -mmacosx-version-min=13.3
endif

test_doubledouble: test_doubledouble.cpp checkit.h
	$(CXX) $(CXXFLAGS) test_doubledouble.cpp -o test_doubledouble

clean:
	rm -rf test_doubledouble
