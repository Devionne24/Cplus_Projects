#include <vector>
#include <iostream>
#include<fstream>
#include<string>
#include "singleton.hpp"
using std::cout; using std::vector; using std::ifstream; using std::string;

void sayHi() {
	LoggerVect::instance().report("Hi!");
}
int main() {
	LoggerVect::instance().report("This is me seeing if this works.");
	LoggerVect::instance().report("I love my life.");
	sayHi();
	LoggerVect::instance().print();
}
