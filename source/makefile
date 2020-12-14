main: main.o BPlustree.o Struct.o
	g++ main.o BPlustree.o Struct.o -o main

main.o: main.cpp
	g++ -c main.cpp

judge: judge.o
	g++ judge.o -o judge
	
judge.o: judge.cpp customTest
	g++ -c judge.cpp

customTest: customTest.o BPlustree.o Struct.o
	g++ customTest.o BPlustree.o Struct.o -o customTest

customTest.o: customTest.cpp
	g++ -c customTest.cpp
	
BPlustree.o: BPlustree.cpp BPlustree.hpp
	g++ -c BPlustree.cpp
	
Struct.o: Struct.cpp Struct.hpp
	g++ -c Struct.cpp

clean: 
	rm -f *.o *.exe
