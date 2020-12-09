2020.11.23 13:06~

* Coding style:
  * Don't use ```pre_work``` , use ```preWork``` instead for variable name and method name
  * The first letter of type name is capital: ```ActorClass```
  * The key of coding style is being consistent
* Note
  * Follow a consistent standard/tutorial 
  * Testing is necessary
* Reference
  * [washington cs course slide](https://courses.cs.washington.edu/courses/cse326/09wi/lectures/11-b-trees.pdf)

2020.11.24 12:21~

2020.11.26 14:20~

* Leaf node's children is record
* Internal node's children is internal node or leaf node
* The number of children: [ceil(M/2), M]
* each leaf hold a set of pointer to records

2020.11.27 19:29~

* Trying to figure out the relationship between key and node

2020.11.28 14:54~

* todo: finish insertion implementation [done]
* todo: test insertion implementation

2020.11.29 19:00~

* Bug: runtime error when doing insert [fixed]
* currently, key = value = key
* todo: find the difference between new/delete and malloc/free

2020.11.30 8:10~

2020.12.1 9:09~

* todo: improve and finish implementation of insert [done]
* issue: no assertion or error detection
* left most child node is unchanged, because every time split we propagate the right part
* todo: test new implementation of insert

2020.12.3 10:02~

* todo: figure out how makefile & compile work

* difference between compile & link

  * preprocessing: replace #include, #if, #define with pure c++ code

  * compilation: code without # into assemble, then into machine code

    * symbols are referred by name

    * symbols that not defined is ok, so you can compile each source code seperately, a undefined but ok symbol example is following:

      ```c++
      //a.hpp
      class A{
      public:
          void NotImplementedFun();
      }
      ```

      ```c++
      //a.cpp
      #include "a.hpp"
      //empty
      ```

      ```c++
      //main.cpp
      #include "a.hpp"
      
      int main(){
          A a;
          a.NotImplementedFum();
          return 0;
      }
      ```

      this can compile, but will report error in link phase

    * linking: replace all references to undefined symbols with correct address, and produce library or executable, missing/duplicate definition is reported here

2020.12.5 9:00~

* when merge or insert internal node, key is from parent 

2020.12.6 9:02~

* helper function is useful

2020.12.7 8:40~

* 

2020.12.8 9:20~

* finishing delete operation
* todo:
  * generate custom cases to see its correctness
  * code test
* insert at index, so on...

2020.12.9 8:40~

* Fix RE bug about insertion