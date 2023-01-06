//CS44001
//Devionne Littleton
//23 September 2022
#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include<algorithm>
#include <cstdlib>

using std::ifstream;
using std::string; using std::getline;
using std::list; using std::vector;
using std::cout; using std::endl;
using std::move; 
// reading a list from a fileName
void readRoster(vector<list<string>>& roster, string fileName);

// printing a list out
void printRoster(const vector<list<string>>& roster);

int main(int argc, char* argv[]) {

    if (argc <= 1) {
        cout << "usage: " << argv[0]
            << " list of courses, dropouts last"
            << endl; exit(1);
    }

    // vector of courses of students
    vector<list<string>> roster;
    cout << "\n\n" << "All Students" << "\n";
    for (int i = 1; i < argc-1; ++i) {
        readRoster(roster, argv[i]);
    }
    printRoster(roster);

    // reading in dropouts
    vector<list<string>> dropouts;
    readRoster(dropouts, argv[argc - 1]);
    // master list of students
    vector<list<string>> allStudents;
    for (const auto& lst : roster) {
        bool checkerTwo = true;
        for (const auto& lstTwo : dropouts) {
            //gets rid of the dropouts within the classes
            if (lst.front() == lstTwo.front()) {
                checkerTwo = false;
            }
        }
        if (checkerTwo == true) {
            allStudents.push_back(lst);
        }

    }

    cout << "\n\n all students sorted and dropouts removed \n";
    //Putting names in alphabetical order
    std::sort(allStudents.begin(), allStudents.end());
    //printing out new list
    printRoster(allStudents);
}

// reading in a file of names into a list of strings
void readRoster(vector<list<string>>& roster, string fileName) {
    //opening files
    ifstream course(fileName);
    //Dealing with extracting name of class
    string first, last, courseCut;
    //used to push back the information
    list<string> addStud;
    courseCut = fileName;
    //cutting down file names to get name of class
    courseCut.erase(courseCut.begin() + courseCut.find("."), courseCut.end());
        while (course >> first >> last) {
            bool checker = true;
            string structure = first + ' ' + last + ": ";
            //going through entire roster in file
            for (auto& lst : roster) {
                //if it finds the name, it associates it with the class
                if (lst.front() == structure && checker == true) {
                    lst.push_back(courseCut);
                    checker = false;
                }

            }
            if (checker == true) {
                addStud.push_back(move(structure)); 
                addStud.push_back(courseCut);
                roster.push_back(move(addStud));
            }
        }
    course.close();
}

// printing a list out
void printRoster(const vector<list<string>>& roster) {
    //going through the whole roster
    for (auto starter = roster.begin(); starter != roster.end(); ++starter) {
        //points to everything that was pushed back in files
        for (const auto& finisher : *starter) {
            //prints it out
            cout << finisher << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}
