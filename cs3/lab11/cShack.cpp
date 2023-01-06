// Devionne Littleton 
// 11/18/2022

#include <iostream>
#include <cstdlib>
#include<deque>

#include "cShack.hpp"

using std::cout; using std::cin; using std::deque;

int main() {
	cout << "Open for business!" << "\n";
	srand(time(nullptr));
	//applying randomness to baristas
	const int numOrders = 4;
	//4 different orders being made
	cout << "There is a total of " << numOrders << " orders." << "\n";
	deque<CustomerCon> customerList;
	//Chain of responsibility for Baristas
	Barista* baristaOptions = new JuniorBarista(new SeniorBarista(new Manager));
	//add customers into a deque with a barista
	for (int i = 0; i < numOrders; i++) {
		customerList.push_back(CustomerCon("", baristaOptions, nullptr));
	}

	//loop to make 4 orders
	int amountOfDrinks = 0;
	while (amountOfDrinks != numOrders) {
		int chooseBarista = rand() % 2;
		char size;
		int custIndex = 0;
		if (amountOfDrinks == 0 || chooseBarista == 0) {
			cout << "Hello! Would you like a [s]mall, [m]edium, or [l]arge coffee today: ";
			cin >> size;
			Drink* customerDrink = nullptr;
			//Different cases for each size
			if (size == 'm') {
				customerDrink = new Drink(DrinkType::medium, 7);
			}
			else if (size == 'l') {
				customerDrink = new Drink(DrinkType::large, 10);
			}
			else if (size == 's') {
				customerDrink = new Drink();
			}
			else {
				cout << "Please input a size listed above" << "\n";
			}
			//gets name and amount for the toppings while adding to price
			cout << "Would like to add any [s]ugar, [c]ream, or milk [f]oam? Just say [n]o more when you don't want anymore toppings! " << "\n";
			if (customerDrink != nullptr) {
				char toppings = 'p';
				while (toppings != 'n') {
					cin >> toppings;
					if (toppings == 's') {
						customerDrink->setPrice(Sugar(customerDrink).getPrice());
						customerDrink->setName(Sugar(customerDrink).getName());
					}
					else if (toppings == 'c') {
						customerDrink->setPrice(Cream(customerDrink).getPrice());
						customerDrink->setName(Cream(customerDrink).getName());
					}
					else if (toppings == 'f') {
						customerDrink->setPrice(Foam(customerDrink).getPrice());
						customerDrink->setName(Foam(customerDrink).getName());
					}
					//In case of any incorrect inputs
					else if (toppings != 's' && toppings != 'c' && toppings != 'f' && toppings != 'n') {
						cout << "Please input a correct add on!" << "\n";
					}

					if (toppings != 'n') {
						cout << "Any other add ons?: ";
					}
				}
				//indexes the customer along with their drink
				cout << "Excellent! Now can I just get a name for this order?" << "\n";
				string custName;
				cin >> custName;
				cout << custName << "? That's an awesome name." << "\n";
				//matches the customer with the drink
				customerList[custIndex].setName(custName);
				customerList[custIndex].setOrder(customerDrink);
				baristaOptions->registerCustomer(&customerList[custIndex]);

				++amountOfDrinks;
				++custIndex;
				
			}
		} 
		else {
			//chooses a random drink to be finished
			int randDrink;
			if (baristaOptions != nullptr && baristaOptions->returnCustomerSize() != 0) {
				randDrink = rand() % (baristaOptions->returnCustomerSize());
				baristaOptions->notifyFinishDrink(customerList[randDrink].getName(), customerList[randDrink].getDrinkName());
				baristaOptions->deregisterCustomer(&customerList[randDrink]);
				customerList.erase(customerList.begin() + randDrink);
				--custIndex;
			}
			
		}
	}
}	
