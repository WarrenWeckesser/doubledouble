CXXFLAGS = -pedantic -Wall -Werror -std=c++17 -I../include

ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
else
    DETECTED_OS := $(shell uname -s)
endif

ifeq ($(DETECTED_OS),Darwin)
	CXXFLAGS += -mmacosx-version-min=13.3
endif

EXECUTABLES = demo readme_example one_third find_eps

all: $(EXECUTABLES)

$(EXECUTABLES): %: %.cpp
	$(CXX) $(CXXFLAGS) $< -o  $@

clean:
	rm -f $(EXECUTABLES)
