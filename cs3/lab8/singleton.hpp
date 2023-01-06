#include <vector>
#include <iostream>
#include<fstream>
#include<string>
using std::cout; using std::vector; using std::ifstream; using std::string;

class LoggerVect {
public: 
	static LoggerVect& instance() {
		static LoggerVect l;
		return l;
	}

	void report(const string& s) { 
		fout << s << "\n";
		strings_.push_back(s);
	}

	void print() {
		for (auto lst : strings_) {
			cout << lst << "\n";
		}
	}

	

private:

	LoggerVect() {
		fout.open(fileName, std::fstream::out | std::fstream::app);
		if (!fout.is_open()) {
			cout << "Could not open file." << "\n";
			exit(0);
		}
	}
	LoggerVect(const LoggerVect&) = delete;
	LoggerVect& operator=(const LoggerVect&) = delete;

	string fileName = "singleton.txt";
	std::ofstream fout;
	vector<string> strings_;
};
