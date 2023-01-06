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
#include <map>

using std::ifstream;
using std::string; using std::getline;
using std::list; using std::vector;
using std::cout; using std::endl;
using std::move; using std::map;
using std::pair;

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
    string firstName_;
    string lastName_;
};




// reading a map 
void readRoster(map <Student, list<string>>& roster, string fileName);
// printing a map
void printRoster(const map<Student, list<string>>& roster);

int main(int argc, char* argv[]) {

    if (argc <= 1) {
        cout << "usage: " << argv[0]
            << " list of courses, dropouts last" << endl; exit(1);
    }


    map<Student, list<string>> roster;


    for (int i = 1; i < argc - 1; ++i) {
        readRoster(roster, argv[i]);
    }
    printRoster(roster);

    
    // reading in dropouts
    map <Student, list<string>> dropouts;
    readRoster(dropouts, argv[argc - 1]);
    
  
    map <Student, list<string>> allStudents;  // master list of students
    for (auto lst = roster.cbegin(); lst != roster.cend(); ++lst) {
        bool checkerTwo = true;
        for (auto lstTwo = dropouts.cbegin(); lstTwo != dropouts.cend(); ++lstTwo) {
            //getting rid of dropouts
            if (lstTwo->first.print() == lst->first.print()) {
                checkerTwo = false;
            }
        }
        if (checkerTwo == true) {
            //push back new list
            allStudents.insert(std::make_pair(Student(lst->first), list<string>({ (lst->second) })));
        }

    }
    
    cout << "\n\n all students sorted and dropouts removed \n";
    //sorted alphabetically, last name
    printRoster(allStudents);
    
}


void readRoster(map<Student, list<string>>& roster, string fileName) {
    //opens file
    ifstream course(fileName);
    string first, last, courseCut;
    courseCut = fileName;
    //Gets name of class, gets rid of .txt
    courseCut.erase(courseCut.begin() + courseCut.find("."), courseCut.end());
    while (course >> first >> last) {
        //looking for the key, which is student
        auto lst = roster.find(Student(first, last));
        if (lst == roster.end()) {
            //inserting student and the associated class
            roster.insert(std::make_pair(Student(first, last), list<string>({ courseCut })));
        }
        else {
            (lst->second).push_back(courseCut);
        }
    }
    course.close();
}

// printing a list out
void printRoster(const map <Student, list<string>>& roster) {
    for (auto lst = roster.begin(); lst != roster.end(); lst++) {
        //setting up structure of print
        cout << (lst->first).print() << ": ";
        for (auto courses : lst->second) {
            cout << courses << " ";
        }
        cout << "\n";
    }
}
