// needed for lab
// Devionne Littleton
// 11/04/2022

#include <iostream>
#include <vector>
#include <cstdlib>
#include <array>
#include "carFactory.hpp"

using std::vector;
using std::cout; using std::endl;

class CarLot {
public:
    CarLot();

    // goes through lot to find car
    Car* buyCar(Car* carBought) {
        for (int i = 0; i < lotSize_; i++) {
            if (carLot_[i] == carBought && bought == false) {
                //changes to true if they find the car they wanted
                bought = true;
                carLot_[i] = factories_[rand() % factories_.size()]->requestCar();
            }
       }
        return carBought;
    }

    //moves to the next car by incrementing carMover, will return to first car in the lot after the last car
    Car* nextCar() {
        static int carMover = 0;
        if (carMover == lotSize_) {
            carMover = 0;
            return carLot_[carMover++];
        }
        else {
            return carLot_[carMover++];
        }
    }
    int lotSize() { return lotSize_; }

private:
    const static int lotSize_ = 10;
    bool bought = false;
    
    vector <Car*> carLot_; // all cars for sale at the lot
    vector<CarFactory*> factories_;
};


CarLot::CarLot() {
    // creates 2 Ford factories and 2 Toyota factories 
    factories_.push_back(new FordFactory());
    factories_.push_back(new ToyotaFactory());
    factories_.push_back(new FordFactory());
    factories_.push_back(new ToyotaFactory());

    // gets the first car for sale
    for (int i = 0; i < lotSize_; i++) {
        carLot_.push_back(factories_[rand() % factories_.size()]->requestCar());
    }
   
}



CarLot* carLotPtr = nullptr; // global pointer instantiation

// test-drives a car
// buys it if Toyota
void toyotaLover(int id) {
    //generates random seed
    srand(time(nullptr));
    std::string name = "Jill Toyoter";
    if (carLotPtr == nullptr)
        carLotPtr = new CarLot();

    //chooses random car and that ends up being the car they look for
    vector<std::string> tModels = {"4Runner", "Camry", "Corolla", "Prius", "Yaris"};
    std::string randtModel = tModels[rand() % tModels.size()];
    bool carFinder = false;
    cout << name << ": " << id << ": is looking to get a new Toyota " << randtModel << endl;

    //states each car that is being tested, and sees if it is the car they're looking for
    for (int i = 0; i < carLotPtr->lotSize(); i++) {
        Car* currentCar = carLotPtr->nextCar();
        cout << name << ": " << id << " is test driving a ";

        //states name of car
        cout << currentCar->getMake() << " " << currentCar->getModel() << endl;
        
        if (currentCar->getModel() == randtModel && currentCar->getMake() == "Toyota") {
            carLotPtr->buyCar(currentCar);
            carFinder = true;
            i = carLotPtr->lotSize();
            cout << "This is the car i've always dreamed of! I'll take it!" << endl;
        }
        else {
            cout << "This isn't quite what i'm looking for." << endl;
        }
    }
    //says this if they were unable to find the car
    if (!carFinder) {
        cout << name << " didn't get to test the car they wanted today." << endl;
    }
    cout << "\n";
}

// test-drives a car
// buys it if Ford
void fordLover(int id) {
    //generates random seed
    srand(time(nullptr));
    std::string name = "Jack Fordman";
    if (carLotPtr == nullptr)
        carLotPtr = new CarLot();

    //chooses car they are looking for
    vector<std::string> fModels = { "Focus", "Explorer", "Mustang", "F-150" };
    std::string randfModel = fModels[rand() % fModels.size()];
    bool carFinder = false;
    cout << name << ": " << id <<": is looking to get a new Ford " << randfModel << endl;

    for (int i = 0; i < carLotPtr->lotSize(); i++) {
        Car* currentCar = carLotPtr->nextCar();
        //outputs each make and model as they're testing
        cout << name << ": " << id << " is test driving a ";
        cout << currentCar->getMake() << " " << currentCar->getModel() << endl;

        if (currentCar->getModel() == randfModel && currentCar->getMake() == "Ford") {
            carLotPtr->buyCar(currentCar);
            carFinder = true;
            i = carLotPtr->lotSize();
            cout << "This is the car i've always dreamed of! I'll take it!" << endl;
        }
        else {
            cout << "This isn't quite what i'm looking for." << endl;
        }
    }
    if (!carFinder) {
        cout << name << " didn't get the test the car they wanted today." << endl;
    }
    cout << "\n";
}



int main() {
    srand(time(nullptr));
    //sets up number of buyers
    const int numBuyers = 5;
    //chooses to start with either toyotalover or fordlover
    for (int i = 0; i < numBuyers; ++i)
        if (rand() % 2 == 0)
            toyotaLover(i);
        else
            fordLover(i);

}
