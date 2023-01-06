
#include<iostream>
#include<string>
#include <cstring>

#ifndef WORDLIST_HPP_
#define WORDLIST_HPP_
using std::string; using std::cout;

//All class outlines and method prototypes
class WordOccurrence {
public:
    WordOccurrence(const string& word = "", int num = 0) : word_(word), num_(num) {};
    bool matchWord(const string& s1); // returns true if word matches stored
    void increment(); // increments number of occurrences
    string getWord() const;
    int getNum() const;
    bool operator<(const WordOccurrence& tmp);
private:
    string word_;
    int num_;
};

class WordList {
public:
    WordList(int size_ = 0) : size_(size_), wordArray_(new WordOccurrence[size_]) {};
    WordList(const WordList& other) : size_(other.size_),
        wordArray_(other.size_ > 0 ? new WordOccurrence[other.size_] : nullptr) {
        std::copy(other.wordArray_, other.wordArray_ + size_, wordArray_);
    }; //copy constructor
    ~WordList(); //destructor
    WordList& operator= (WordList other); //Operator Overloading
    friend bool equal(const WordList& w1, const WordList& w2);
    friend void swap(WordList& w1, WordList& w2);
    void addWord(const string& s2);
    void print();
private:
    WordOccurrence* wordArray_; // a dynamically allocated array of WordOccurrences
                           // may or may not be sorted
    int size_;
};

#endif
