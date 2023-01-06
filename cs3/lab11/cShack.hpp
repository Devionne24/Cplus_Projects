// drink class to be used in Coffee Shack lab
// Devionne Littleton 
// 11/18/2022

#include <iostream>
#include <string>
#include<set>

using std::cout; using std::set; using std::string;
//Component
enum class DrinkType { small, medium, large };

//Concrete Component
class Drink {
public:
    //Default case, sets up sentences
    Drink(DrinkType type = DrinkType::small, int price = 5) :
        type_(type), price_(price) {
        if (type_ == DrinkType::large) {
            name_ = "Large Coffee: ";
        }
        else if (type_ == DrinkType::medium) {
            name_ = "Medium Coffee: ";
        }
        else {
            name_ = "Small Coffee: ";
        }
    }
    virtual int getPrice() const { return price_; }
    virtual string getName() const { return name_; }
    virtual void setName (string n) {name_ = n;}
    virtual void setPrice(int p) { price_ = p; }
private:
    int price_;
    string name_;
    DrinkType type_;
};

//Decorators for Drink: Foam, Sugar, Cream
class Foam : public Drink {
public:
    Foam(const Drink* modDrink) : modDrink_(modDrink) {}

    int getPrice() const override { return modDrink_->getPrice() + 3; }
    string getName() const override { return modDrink_->getName() + "foam, "; }

private:
    const Drink* modDrink_;
};

class Sugar : public Drink {
public:
    Sugar (const Drink* modDrink): modDrink_(modDrink) {}

    int getPrice() const override { return modDrink_->getPrice() + 1; }
    string getName() const override { return modDrink_->getName() + "sugar, "; }
private:
    const Drink* modDrink_;
};

class Cream : public Drink {
public:
    Cream (const Drink* modDrink): modDrink_(modDrink) {}

    int getPrice() const override { return modDrink_->getPrice() + 2; }
    string getName() const override { return modDrink_->getName() + "cream, "; }
private:
    const Drink* modDrink_;
};

//Declared before Barista for Observer Pattern
class Customer {
public:
    Customer() {}
    Customer(class Barista* s);
    virtual void notify() const = 0;
};

//Beginning Chain of Responsibility with Baristas: Junior, Senior, Manager
class Barista {
public: 
    Barista (Barista* level = nullptr) : level_(level) {}
    virtual void makeDrink(std::string build) {
        if (level_ != nullptr) {
            level_->makeDrink(build);
        }
        else {
             cout << "No Barista is able to make the order." << "\n";
        }
    }
    //registers and deregisters when customer drinks are finished
    //Part of Observer Pattern
    void registerCustomer(Customer* c) { views_.insert(c); }
    void deregisterCustomer(Customer* c) { views_.erase(c); }
    void notifyCustomers() const {
        for (const auto& e : views_) {
            e->notify();
        }
    }

    size_t returnCustomerSize() const { return views_.size(); }
    string getCustName() const { return name_; }
    string getDrinkName() const { return drinkName_; }
    void notifyFinishDrink(const string name, const string drinkName) {
        name_ = name;
        drinkName_ = drinkName;
        notifyCustomers();
    }
private:  
    string drinkName_;
    string name_;
    Barista* level_;
    set <Customer*> views_;
};

//The concrete class for customer
class CustomerCon : public Customer {
public:
    //sets customer with name, order, and the barista, 2 constructors for 2 scenarios
    CustomerCon() : custName_(""), order_(nullptr), barista_(nullptr) {}
    CustomerCon(const string& custName, Barista* barista, Drink* order) : custName_(custName), barista_(barista), order_(order) {}

    void setOrder(Drink* newDrink) { order_ = newDrink; }
    string getName() const { return custName_; }
    void setName(string newCustName) { custName_ = newCustName; }
    string getDrinkName() const { return order_->getName(); }

    //notifies customer that their drink is ready
    void notify() const override {
        if (order_ != nullptr) {
            if (custName_ == barista_->getCustName() && barista_->getDrinkName() == order_->getName()) {
                int newPrice = order_->getPrice();
                barista_->makeDrink(order_->getName());
                cout << "The order for " << custName_ << " is done.";
                cout << " The total is " << newPrice << " dollars." << "\n";
            }
        }
    }
private:
    string custName_;
    Barista* barista_;
    Drink* order_;
};

Customer::Customer(class Barista* s) {
    s->registerCustomer(this);
}

//Begins Chain of Responsibility
class JuniorBarista : public Barista {
public:
    JuniorBarista(Barista* level = nullptr) : Barista(level) {}
    void makeDrink(string build) override {
        //Shows that Junior Barista cannot work with drinks with add ons
        size_t creamFinder = build.find("cream");
        size_t sugarFinder = build.find("sugar");
        size_t foamFinder = build.find("foam");
        //Finds part of string where these ingredients are mentioned and will not make drink if found
        if ((creamFinder != string::npos) || (sugarFinder != string::npos) || (foamFinder != string::npos)) {
            Barista::makeDrink(build);
        }
        else {
            cout << "The Junior Barista can make your order of: " << build << "\n";
        }
    }
};

class SeniorBarista : public Barista {
public:
    SeniorBarista(Barista* level = nullptr) : Barista(level) {}
    void makeDrink(string build) override {
        //Only cannot use foam
        size_t foamFinder = build.find("foam");
        if (foamFinder != string::npos) {
            Barista::makeDrink(build);
        }
        else {
            cout << "The Senior Barista can make your order of: " << build << "\n";
        }
    }
};

class Manager : public Barista {
public: 
    Manager(Barista* level = nullptr) : Barista(level) {}
    //Manager can make anything in the store
    void makeDrink(string build) override {
        cout << "The Manager can make your order of: " << build << "\n";
    }
};

