//CS44001
//Devionne Littleton 
//30 September 2022

#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <cstdlib>
#include <set>

using std::ifstream;
using std::string; using std::getline;
using std::list; using std::vector;
using std::cout; using std::endl;
using std::move; using std::set;


class Student {
public:
    Student(string firstName, string lastName) :
        firstName_(firstName), lastName_(lastName) {}

    // move constructor, not really needed, generated automatically
    Student(Student&& org) :
        firstName_(move(org.firstName_)),
        lastName_(move(org.lastName_))
    {}

    // force generation of default copy constructor
    Student(const Student& org) = default;

    string print() const { return firstName_ + ' ' + lastName_; }

    // needed for sort()
    friend bool operator< (Student left, Student right) {
        return left.lastName_ < right.lastName_ ||
            (left.lastName_ == right.lastName_ &&
                left.firstName_ < right.firstName_);
    }


private:
    //needed to move around files
    string firstName_;
    string lastName_;
};




// reading a set
void readRoster(set<Student>& roster, string fileName);
// printing a set out
void printRoster(const set<Student>& roster);

int main(int argc, char* argv[]) {

    if (argc <= 1) {
        cout << "usage: " << argv[0]
            << " list of courses, dropouts last" << endl; exit(1);
    }

    set<Student> roster;

    cout << "All Students: Past and Present." << "\n";
    for (int i = 1; i < argc - 1; ++i) {
        readRoster(roster, argv[i]);
    }
    printRoster(roster);

    // reading in dropouts
    set<Student> dropouts;
    readRoster(dropouts, argv[argc - 1]);
    for (auto itr = dropouts.begin(); itr != dropouts.end(); ++itr) {
        //value to hold dropouts
        auto searcher = roster.find(*itr);
        if (searcher != roster.end()) {
            //erases the instance the dropout in found within roster
            roster.erase(*itr);
        }
    }
    cout << "\n\n All Students Currently Enrolled: \n";
    //sorted alphabetically, last name
    printRoster(roster);
}


void readRoster(set<Student>& roster, string fileName) {
    //opens file
    ifstream course(fileName);
    string first, last;
    while (course >> first >> last) {
        //finds name and inserts them within set
       Student inserter(first, last);
       roster.insert(inserter);
    }
    course.close();
}

// printing a list out
void printRoster(const set<Student>& roster) {
    for (const auto& student : roster) {
        //goes thru each file and prints all
        cout << student.print() << "\n";
    }
}
