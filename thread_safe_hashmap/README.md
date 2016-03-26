# Thread safe hash maps

Inspired by [Anthony William's thread safe lookup table from "C++ Concurrency in action"](https://www.manning.com/books/c-plus-plus-concurrency-in-action) with minor changes.

###### How to compile on Linux:

    clang++ -O3 -Wall -std=c++14 main.cpp -pthread

or

    g++ -O3 -Wall -std=c++14 main.cpp -pthread

