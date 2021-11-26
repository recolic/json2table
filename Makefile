
# Compile for windows: make CXX='x86_64-w64-mingw32-g++ -Ilib -static'
CXX ?= g++

build:
	$(CXX) -O3 json2table.cc -std=c++17 lib/fort.c -o json2table

install: build
	cp json2table /usr/bin/

