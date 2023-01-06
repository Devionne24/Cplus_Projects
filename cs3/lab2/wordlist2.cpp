#include <iostream>
#include <string>
#include<cctype>
#include<fstream>
#include "wordlist.hpp"

using namespace std; using std::ifstream;

int main(int argc, char *argv[]){
	//creates object and string
	WordList sortWord;
	string wordTransfer;
	//Won't run if argc is above 2
	if (argc == 2) { 
		//Opening file
		ifstream fileReader(argv[1]);
		while (fileReader >> wordTransfer){
			//transfers file to string then documents words from file 
			sortWord.addWord(wordTransfer);
		}
		//prints out the results
		sortWord.print();
	} else {
		cout<< "Error. Please remember to use only one file to read.";
	}

}

