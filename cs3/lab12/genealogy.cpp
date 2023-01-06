// traversing genealogical tree using Composite and Visitors
// Devionne Littleton
// 12/2/2022

#include <iostream>
#include <string>
#include <vector>
#include "genealogy.hpp"

using std::cout; using std::endl; using std::cin;
using std::string;
using std::vector;

// demonstrating the operation
int main() {

    // setting up the genealogical tree      
    // the tree is as follows
    //    
    //
    //       James Smith  <--spouse-->   Mary 
    //	                                  |
    //	                                 children -------------------------
    //	                                  |              |                |
    //	                                  |              |                |
    //	   William Johnson <--spouse-> Patricia      Robert Smith       Linda
    //	                                  |
    //	                                 children------------
    //	                                  |                 |
    //                                     |                 |
    //	   Jennifer  <--spouse-->  Michael Johnson      Barbara
    //	       |
    //	     children
    //	       |
    //          |
    //	     Susan


    // first generation
    Man* js = new Man("Smith", "James", nullptr, nullptr, nullptr);
    Woman* ms = new Woman({}, "Mary", nullptr, nullptr, nullptr);
    ms->setSpouse(js); js->setSpouse(ms);

    // second generation
    Woman* ps = new Woman({}, "Patricia", nullptr, js, ms);
    Man* wj = new Man("Johnson", "William", nullptr, nullptr, nullptr);
    ps->setSpouse(wj); wj->setSpouse(ps);

    vector<Person*> marysKids = { ps,
                       new Man("Smith", "Robert", nullptr, js, ms),
                       new Woman({}, "Linda", nullptr, js, ms) };
    ms->setChildren(marysKids);

    // third generation
    Man* mj = new Man("Johnson", "Michael", nullptr, wj, ps);
    vector<Person*> patsKids = { mj, new Woman({}, "Barbara", nullptr, wj, ps) };
    ps->setChildren(patsKids);

    Woman* jj = new Woman({}, "Jennifer", nullptr, nullptr, nullptr);
    vector<Person*> jensKids = { new Woman({}, "Susan", nullptr, mj ,jj) };

    jj->setSpouse(mj); mj->setSpouse(jj);
    jj->setChildren(jensKids);


    // defining two visitors
    ChildrenPrinter* cp = new ChildrenPrinter;
    NamePrinter* np = new NamePrinter;

    string nameOne;
    string nameTwo;

    cout << "Now running the Relation Check, will be ran three times" << "\n";
    for (int i = 0; i < 3; i++) {
        cout << "Please enter the name of the first candidate" << "\n";
        std::getline(cin, nameOne);
        cout << "Please enter the name of the second candidate" << "\n";
        std::getline(cin, nameTwo);

        RelativeCheck* rp = new RelativeCheck(nameOne, nameTwo);

        ms->accept(rp);
        cout << rp->statusCheck() << "\n";
        cout << "\n";
        delete rp;
    }
}
