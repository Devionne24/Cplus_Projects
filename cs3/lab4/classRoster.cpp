//CS44001
//Devionne Littleton 
//23 September 2022

#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <cstdlib>

using std::ifstream;
using std::string; using std::getline;
using std::list; using std::vector;
using std::cout; using std::endl;
using std::move;


class Student {
public:
    Student(string firstName, string lastName) :
        firstName_(firstName), lastName_(lastName) {}

    // move constructor, not really needed, generated automatically
    Student(Student&& org) :
        firstName_(move(org.firstName_)),
        lastName_(move(org.lastName_)),
        course(move(org.course))
    {}

    // force generation of default copy constructor
    Student(const Student& org) = default;

    string print() const { return firstName_ + ' ' + lastName_; }

    // needed for unique() and for remove()
    friend bool operator== (Student left, Student right) {
        return left.lastName_ == right.lastName_ &&
            left.firstName_ == right.firstName_;
    }

    // needed for sort()
    friend bool operator< (Student left, Student right) {
        return left.lastName_ < right.lastName_ ||
            (left.lastName_ == right.lastName_ &&
                left.firstName_ < right.firstName_);
    }
    //made to push back all courses
    void Adder(string add) {course.push_back(add);}

    //used within printRoster to print everything
    void printAll() const {
        //separates names from classes
        cout << this->print() << ": ";
        for (auto mover = course.begin(); mover != course.end(); mover++) {
            cout << *mover << " ";
        }
        cout << "\n";
    }
private:
    //needed to move around files
    list<string> course;
    string firstName_;
    string lastName_;
};




// reading a list from a fileName
void readRoster(list<Student>& roster, string fileName);
// printing a list out
void printRoster(const list<Student>& roster);

int main(int argc, char* argv[]) {

    if (argc <= 1) {
        cout << "usage: " << argv[0]
            << " list of courses, dropouts last" << endl; exit(1);
    }

   
    list<Student> roster;
   

    cout << "All Students" << "\n";
    for (int i = 1; i < argc - 1; ++i) {
        readRoster(roster, argv[i]);
    }
    printRoster(roster);

    // reading in dropouts
    list<Student> dropouts;
    readRoster(dropouts, argv[argc - 1]);

    list<Student> allStudents;  // master list of students
    for (auto& lst : roster) {
        bool checkerTwo = true;
        for (auto& lstTwo : dropouts) {
            //getting rid of dropouts
            if (lst.print() == lstTwo.print()) {
                checkerTwo = false;
            }
        }
        if (checkerTwo == true) {
            //push back new list
            allStudents.push_back(lst);
        }

    }
    cout << "\n\n all students sorted \n";
    //sorted alphabetically, last name
    allStudents.sort();
    printRoster(allStudents);

}


void readRoster(list<Student>& roster, string fileName) {
    //opens file
    ifstream course(fileName);
    string first, last, courseCut;
    courseCut = fileName;
    //Gets name of class, gets rid of .txt
    courseCut.erase(courseCut.begin() + courseCut.find("."), courseCut.end());
    while (course >> first >> last) {
        string structure = first + ' ' + last;
        bool checker = true;
        for (auto& lst : roster) {
            //checks for name, associates with class if found
            if (lst.print() == structure) {
                lst.Adder(courseCut);
                checker = false;
            }
        }
        if (checker == true) {
            //object to push back the course and add to roster
            Student molder(first, last);
            molder.Adder(courseCut);
            roster.push_back(move(molder));
        }
    }
    course.close();
}

// printing a list out
void printRoster(const list<Student>& roster) {
    for (const auto& student : roster) {
        //goes thru each file and prints all
        student.printAll();
    }
}
