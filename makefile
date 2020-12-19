Test: test/judge test/customTest

test/judge: src/test/judge.cpp
	g++ $< -o $@

test/customTest: build/customTest.o build/struct.o build/tree.o
	g++ $^ -o $@

build/customTest.o: src/test/customTest.cpp
	g++ -c $< -o $@

build/struct.o: src/tree/struct.cpp src/tree/struct.hpp
	g++ -c $< -o $@ 

build/tree.o: src/tree/tree.cpp src/tree/tree.hpp
	g++ -c $< -o $@ 

clean: 
	rm -f build/*.o 
