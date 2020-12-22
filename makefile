Test: test/judge test/customTest

test/judge: src/test/judge.cpp
	g++ $< -o $@

test/customTest: build/customTest.o build/struct.o build/tree.o
	g++ $^ -o $@

build/customTest.o: src/test/customTest.cpp
	g++ -c $< -o $@
	
test/runner: build/runner.o build/struct.o build/tree.o
	g++ $^ -o $@
	
test/bfRunner: build/bf/runner.o build/bf/struct.o build/bf/tree.o
	g++ $^ -o $@

build/runner.o: src/tree/runner.cpp
	g++ -c $< -o $@

build/bf/runner.o: src/test/bf/runner.cpp
	g++ -c $< -o $@

build/bf/struct.o: src/test/bf/struct.cpp src/test/bf/struct.hpp
	g++ -c $< -o $@ 

build/bf/tree.o: src/test/bf/tree.cpp src/test/bf/tree.hpp
	g++ -c $< -o $@ 

build/struct.o: src/tree/struct.cpp src/tree/struct.hpp
	g++ -c $< -o $@ 

build/tree.o: src/tree/tree.cpp src/tree/tree.hpp
	g++ -c $< -o $@ 

clean: 
	rm -f build/*.o 
