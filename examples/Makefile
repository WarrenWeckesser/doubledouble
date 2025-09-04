CXXFLAGS = -Wall -Werror -std=c++17 -I../include

all: demo readme_example one_third find_eps

demo: demo.cpp
	g++ $(CXXFLAGS) demo.cpp -o demo

readme_example: readme_example.cpp
	g++ $(CXXFLAGS) readme_example.cpp -o readme_example

one_third: one_third.cpp
	g++ $(CXXFLAGS) one_third.cpp -o one_third

find_eps: find_eps.cpp
	g++ $(CXXFLAGS) find_eps.cpp -o find_eps

clean:
	rm -rf demo readme_example one_third find_eps
