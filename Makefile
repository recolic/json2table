
CXX ?= g++

build:
	$(CXX) -O3 json2table.cc -std=c++17 lib/fort.c -o json2table

install: build
	cp json2table /usr/bin/

