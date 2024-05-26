CXXFLAGS = -Wall -Werror -std=c++17 -I../include

all: demo readme_example

demo: demo.cpp
	g++ $(CXXFLAGS) demo.cpp -o demo

readme_example: readme_example.cpp
	g++ $(CXXFLAGS) readme_example.cpp -o readme_example

clean:
	rm -rf demo
