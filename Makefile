
CXX ?= g++ -O3

build:
	$(CXX) json2table.cc -std=c++17 lib/fort.c -o json2table


