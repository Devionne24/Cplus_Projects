//CS44001
//Devionne Littleton 
//9 September 2022
#include <iostream>
#include <string>
#include<algorithm>
#include <cctype>
#include<fstream>
#include "wordlist.hpp"

using namespace std;

//Checks if words are matching
bool WordOccurrence::matchWord(const string& s1)
{
	if (word_ == s1) {
		return true;
	}
	else {
		return false;
	}
}

//increments number of occurences of a word
void WordOccurrence::increment()
{
	num_++;
}

//returns word
string WordOccurrence::getWord() const
{
	return word_;
}

//returns name
int WordOccurrence::getNum() const
{
	return num_;
}


bool WordOccurrence::operator<(const WordOccurrence& tmp)
{
	return tmp.num_ > num_;
}

//checks to see if equal
bool equal(const WordList& w1, const WordList& w2)
{
	if (w1.size_ == w2.size_) {
		return true;
	}
	else {
		return false;
	}
}
//function to help with our operation overloading, swaps values
void swap(WordList& w1, WordList& w2)
{
	std::swap(w1.size_, w2.size_);
	std::swap(w1.wordArray_, w2.wordArray_);
}

//Destructor
WordList::~WordList()
{
	delete[] wordArray_;
}

//Operator Overloading
WordList& WordList::operator=(WordList other)
{
	swap(*this, other);
	return *this;
}

//Method made to account the word into the system and track occurences
void WordList::addWord(const string& s2)
{
	//Takes s2 values and copies them to use erase function
	string alpha = s2;
	for (int i = 0; i < alpha.size(); i++) {
		//Gets rid of any punctuation 
		if (ispunct(alpha[i])){
			alpha.erase(i--, 1);

		}
	}
	//Checks if word matches, will increment if it does
	for (int i = 0; i < size_; i++) {
		if (wordArray_[i].matchWord(alpha)) {
			wordArray_[i].increment();
			return;
		}
	}
	//Creating new array, copying it, and deleting the old array
	WordOccurrence* tmpWordArray = new WordOccurrence[size_ + 1];
		for (int i = 0; i < size_; i++) {
			tmpWordArray[i] = wordArray_[i];
		}
		size_++;
		tmpWordArray[size_ - 1] = alpha;
		delete[] wordArray_;
		wordArray_ = tmpWordArray;
		wordArray_[size_-1].increment();
}
//Prints out the word occurences
void WordList::print()
{
	cout << "Words and their Occurence within the file: " << "\n";
	sort(wordArray_, wordArray_ + size_);
	for (int i = 0; i < size_; i++) {
		cout << wordArray_[i].getWord() << " " << wordArray_[i].getNum() << "\n";
	}
}

