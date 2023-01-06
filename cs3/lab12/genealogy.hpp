// Devionne Littleton
// 12/2/2022

#include <iostream>
#include <string>
#include <vector>

using std::cout; using std::endl; using std::cin;
using std::string;
using std::vector;

class Person { // component
public:
    Person(string firstName, Person* spouse, Person* father, Person* mother) :
        firstName_(firstName), spouse_(spouse), father_(father), mother_(mother) {}
    const string& getFirstName() { return firstName_; }
    Person* getSpouse() { return spouse_; }
    void setSpouse(Person* spouse) { spouse_ = spouse; }
    Person* getFather() { return father_; }
    Person* getMother() { return mother_; }

    virtual void accept(class PersonVisitor*) = 0;
    virtual ~Person() {}
    virtual string firstAndLast() = 0;
private:
    const string firstName_;
    Person* spouse_;
    Person* father_;
    Person* mother_;
};


// leaf
// man has a last name 
class Man : public Person {
public:
    Man(string lastName, string firstName, Person* spouse,
        Person* father, Person* mother) :
        lastName_(lastName),
        Person(firstName, spouse, father, mother) {}
    const string& getLastName() const { return lastName_; }
    void accept(class PersonVisitor* visitor) override;
    string firstAndLast() override { return getFirstName() + " " + getLastName(); }
private:
    const string lastName_;
};

// composite
// woman has a list of children
class Woman : public Person {
public:
    Woman(vector<Person*> children,
        string firstName,
        Person* spouse,
        Person* father, Person* mother) :
        children_(children),
        Person(firstName, spouse, father, mother) {}
    const vector<Person*>& getChildren() const { return children_; }
    void setChildren(const vector<Person*>& children) { children_ = children; }
    void accept(class PersonVisitor* visitor) override;
    string firstAndLast() override {
        if (getSpouse() != nullptr) {
            return getFirstName() + " " + static_cast<Man*>(getSpouse())->getLastName();
        }
        else if (getFather() != nullptr) {
            return getFirstName() + " " + static_cast<Man*>(getFather())->getLastName();
        }
        else {
            return getFirstName() + " " + "Littleton";
        }
    }
private:
    vector<Person*> children_;
};

// abstract visitor
class PersonVisitor {
public:
    virtual void visit(Man*) = 0;
    virtual void visit(Woman*) = 0;
    virtual ~PersonVisitor() {}
};

// composite methods that define traversal
void Man::accept(PersonVisitor* visitor) {
    visitor->visit(this);
}

void Woman::accept(PersonVisitor* visitor) {
    // children traversal through mother only
    // father's children are not traversed so as not 
    // to traverse them twice: for mother and father

    visitor->visit(this);

    // traversing descendants
    for (auto child : children_)
        child->accept(visitor);
}

// concrete visitors
// the last name for a man is stored in object
// the last name a woman is determined by her 
// spouse if she is married
// or by her father if she is not
class NamePrinter : public PersonVisitor {
public:
    void visit(Man* m) override {
        cout << m->getFirstName() << " " << m->getLastName() << endl;
    }
    void visit(Woman* w) override {
        cout << w->getFirstName() << " ";
        if (w->getSpouse() != nullptr)
            cout << static_cast<Man*>(w->getSpouse())->getLastName();
        else if (w->getFather() != nullptr)
            cout << static_cast<Man*> (w->getFather())->getLastName();
        else
            cout << "Doe";
        cout << endl;
    }
};


class ChildrenPrinter : public PersonVisitor {
public:
    void visit(Man* m) override {
        cout << m->getFirstName() << ": ";
        Woman* spouse = static_cast<Woman*>(m->getSpouse());
        if (spouse != nullptr)
            printNames(spouse->getChildren());
        cout << endl;
    }
    void visit(Woman* w) override {
        cout << w->getFirstName() << ": ";
        printNames(w->getChildren());
        cout << endl;
    }
private:
    void printNames(const vector<Person*>& children) {
        for (const auto c : children)
            cout << c->getFirstName() << ", ";
    }
};

class RelativeCheck : public PersonVisitor {
public:
    RelativeCheck(string personOne, string personTwo) : pOne_(personOne), pTwo_(personTwo), check_(false) {}

    void visit(Man* m) override;
    void visit(Woman* w) override;

    string statusCheck();
private:
    string pOne_;
    string pTwo_;
    bool check_;
};

void RelativeCheck::visit(Man* man) {
    //getting rid of grandparents as relatives
    if (man->firstAndLast() == pOne_ && man->getMother() != nullptr && man->getMother()->getFather() != nullptr && man->getMother()->getFather()->firstAndLast() == pTwo_) {
        check_ = false;
        return;
    }
    if (man->firstAndLast() == pOne_ && man->getMother() != nullptr && man->getMother()->getMother() != nullptr && man->getMother()->getMother()->firstAndLast() == pTwo_) {
        check_ = false;
        return;
    }
    if (man->firstAndLast() == pTwo_ && man->getMother() != nullptr && man->getMother()->getFather() != nullptr && man->getMother()->getFather()->firstAndLast() == pOne_) {
        check_ = false;
        return;
    }
    if (man->firstAndLast() == pTwo_ && man->getMother() != nullptr && man->getMother()->getMother() != nullptr && man->getMother()->getMother()->firstAndLast() == pOne_) {
        check_ = false;
        return;
    }
    //parent, aunt, uncle check for person one
    if (man->firstAndLast() == pOne_ && man->getMother() != nullptr && man->getMother()->getMother() != nullptr) {
        for (auto itrChildren : static_cast<Woman*> (man->getMother()->getMother()) ->getChildren()) {
            string siblingName = itrChildren->firstAndLast();
            if (siblingName == pTwo_) {
                check_ = true;
                return;
            }
        }
    }
    else if (man->firstAndLast() == pOne_ && man->getFather() != nullptr && man->getFather()->getMother() != nullptr) {
        for (auto itrChildren : static_cast<Woman*> (man->getFather()->getMother())->getChildren()) {
            string siblingName = itrChildren->firstAndLast();
            if (siblingName == pTwo_) {
                check_ = true;
                return;
            }
        }
    }
 //sibling check
    if (man->firstAndLast() == pOne_ && man->getMother() != nullptr) {
        for (auto itrChildren : static_cast<Woman*> (man->getMother())->getChildren()) {
            string siblingName = itrChildren->firstAndLast();
            if (siblingName == pTwo_) {
                check_ = true;
                return;
            }
        }
    }
    else if (man->firstAndLast() == pTwo_ && man->getMother() != nullptr) {
        for (auto itrChildren : static_cast<Woman*> (man->getMother())->getChildren()) {
            string siblingName = itrChildren->firstAndLast();
            if (siblingName == pTwo_) {
                check_ = true;
                return;
            }
        }
    }
    //parent, aunt, uncle check for person two 
    if (man->firstAndLast() == pTwo_ && man->getMother() != nullptr && man->getMother()->getMother() != nullptr) {
        for (auto itrChildren : static_cast<Woman*> (man->getMother()->getMother())->getChildren()) {
            string siblingName = itrChildren->firstAndLast();
            if (siblingName == pOne_) {
                check_ = true;
                return;
            }
        }
    }
    else if (man->firstAndLast() == pTwo_ && man->getFather() != nullptr && man->getFather()->getMother() != nullptr) {
        for (auto itrChildren : static_cast<Woman*> (man->getFather()->getMother())->getChildren()) {
            string siblingName = itrChildren->firstAndLast();
            if (siblingName == pOne_) {
                check_ = true;
                return;
            }
        }
    }
   
    //father check
    if ((man->firstAndLast() == pOne_ || man->firstAndLast() == pTwo_) && man->getSpouse() != nullptr) {
        check_ = true;
        return;
    }
    if ((man->getFather() != nullptr && man->getFather()->firstAndLast() == pTwo_ && man->firstAndLast() == pOne_) || (man->getFather() != nullptr && man->getFather()->firstAndLast() == pOne_ && man->firstAndLast() == pTwo_)) {
        check_ = true;
        return;
    }
}

void RelativeCheck::visit(Woman* woman) {
    //getting rid of grandparents as relatives
    if (woman->firstAndLast() == pOne_ && woman->getMother() != nullptr && woman->getMother()->getFather() != nullptr && woman->getMother()->getFather()->firstAndLast() == pTwo_) {
        check_ = false;
        return;
    }
    if (woman->firstAndLast() == pOne_ && woman->getMother() != nullptr && woman->getMother()->getMother() != nullptr && woman->getMother()->getMother()->firstAndLast() == pTwo_) {
        check_ = false;
        return;
    }
    if (woman->firstAndLast() == pTwo_ && woman->getMother() != nullptr && woman->getMother()->getFather() != nullptr && woman->getMother()->getFather()->firstAndLast() == pOne_) {
        check_ = false;
        return;
    }
    if (woman->firstAndLast() == pTwo_ && woman->getMother() != nullptr && woman->getMother()->getMother() != nullptr && woman->getMother()->getMother()->firstAndLast() == pOne_) {
        check_ = false;
        return;
    }
    //children check
    if (woman->firstAndLast() == pOne_) {
        for (auto itrChildren : woman->getChildren()) {
            string childName = itrChildren->firstAndLast();
            if (childName == pTwo_) {
                check_ = true;
                return;
            }
        }
    }
    else if (woman->firstAndLast() == pTwo_) {
        for (auto itrChildren : woman->getChildren()) {
            string childName = itrChildren->firstAndLast();
            if (childName == pOne_) {
                check_ = true;
                return;
            }
        }
    }

    //parent, aunt, uncle check for person one
    if (woman->firstAndLast() == pOne_ && woman->getMother() != nullptr && woman->getMother()->getMother() != nullptr) {
        for (auto itrChildren : static_cast<Woman*> (woman->getMother()->getMother())->getChildren()) {
            string siblingName = itrChildren->firstAndLast();
            if (siblingName == pTwo_) {
                check_ = true;
                return;
            }
        }
    }
    else if (woman->firstAndLast() == pOne_ && woman->getFather() != nullptr && woman->getFather()->getMother() != nullptr) {
        for (auto itrChildren : static_cast<Woman*> (woman->getFather()->getMother())->getChildren()) {
            string siblingName = itrChildren->firstAndLast();
            if (siblingName == pTwo_) {
                check_ = true;
                return;
            }
        }
    } 
    //sibling check
    if (woman->firstAndLast() == pOne_ && woman->getMother() != nullptr) {
        for (auto itrChildren : static_cast<Woman*> (woman->getMother())->getChildren()) {
            string siblingName = itrChildren->firstAndLast();
            if (siblingName == pTwo_) {
                check_ = true;
                return;
            }
        }
    }
    else if (woman->firstAndLast() == pTwo_ && woman->getMother() != nullptr) {
        for (auto itrChildren : static_cast<Woman*> (woman->getMother())->getChildren()) {
            string siblingName = itrChildren->firstAndLast();
            if (siblingName == pTwo_) {
                check_ = true;
                return;
            }
        }
    }
    //parent, aunt, uncle check for person two 
    if (woman->firstAndLast() == pTwo_ && woman->getMother() != nullptr && woman->getMother()->getMother() != nullptr) {
        for (auto itrChildren : static_cast<Woman*> (woman->getMother()->getMother())->getChildren()) {
            string siblingName = itrChildren->firstAndLast();
            if (siblingName == pOne_) {
                check_ = true;
                return;
            }
        }
    }
    else if (woman->firstAndLast() == pTwo_ && woman->getFather() != nullptr && woman->getFather()->getMother() != nullptr) {
        for (auto itrChildren : static_cast<Woman*> (woman->getFather()->getMother())->getChildren()) {
            string siblingName = itrChildren->firstAndLast();
            if (siblingName == pOne_) {
                check_ = true;
                return;
            }
        }
    }
   
    //father check
    if ((woman->firstAndLast() == pOne_ || woman->firstAndLast() == pTwo_) && woman->getSpouse() != nullptr) {
        check_ = true;
        return;
    }
    if ((woman->getFather() != nullptr && woman->getFather()->firstAndLast() == pTwo_ && woman->firstAndLast() == pOne_) || (woman->getFather() != nullptr && woman->getFather()->firstAndLast() == pOne_ && woman->firstAndLast() == pTwo_)) {
        check_ = true;
        return;
    }
    
}

string RelativeCheck::statusCheck() {
    if (check_) {
        return pOne_ + " and " + pTwo_ + " are related.";
    }
    else {
        return pOne_ + " and " + pTwo_ + " are only friends.";
    }
}
