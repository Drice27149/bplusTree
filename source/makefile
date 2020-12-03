main: main.o BPlustree.o Struct.o
	g++ main.o BPlustree.o Struct.o -o main

main.o: main.cpp
	g++ -c main.cpp

BPlustree.o: BPlustree.cpp BPlustree.hpp
	g++ -c BPlustree.cpp
	
Struct.o: Struct.cpp Struct.hpp
	g++ -c Struct.cpp
