#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std; using std::ifstream;
// Prototype function 
int spaceChar(int a);
int main(int argc, char* argv[]) {
	// Declaring variables for the two files and strings. 
	ifstream fiOne(argv[1]);
	ifstream fiTwo(argv[2]);
	string myStringOne, myStringTwo;
	int lCounter = 1;
	
	// Won't run if argument count is above 3
	if (argc == 3) {
		while (!fiOne.eof() || !fiTwo.eof()) {
			//Variable to adjust spacing of the third line
			int spCount = strlen(argv[1]) + (spaceChar(lCounter) + 4);

			//Allows code to find differences even if one file is larger
			if (fiOne.eof()) {
				myStringOne = "";
			}
			if (fiTwo.eof()) {
				myStringTwo = "";
			}

			// Transfers file text into the string
			getline(fiOne, myStringOne);
			getline(fiTwo, myStringTwo);
			int spacingArrow = 0;


			if (myStringOne != myStringTwo) {
				// Outputs the line where differences were spotted
				cout << "Strings don't match on: " << "\n";
				cout << argv[1] << ": " << lCounter << ": " << myStringOne << "\n";
				cout << argv[2] << ": " << lCounter << ": " << myStringTwo << "\n";
				//Loops used to push arrow into position of the difference
				for (int i = 0; i < spCount; i++) {
					spacingArrow++;
				}
				for (int i = 0; myStringOne[i] == myStringTwo[i]; i++) {
					spacingArrow++;

				}
				//Creates new string that will output the spacing
				string spArrow;
				cout << spArrow.assign(spacingArrow, ' ');
				cout << "^" << "\n";
				//Increments after each line to help specify the location
				++lCounter;
			}
			else {
				++lCounter;
			}
			//Resets spacing so it doesn't go off the rails
			spacingArrow = 0;
		}
	}
	else {
		cout << "Error Occured. Remember to use two arguments when using this code." << "\n";
	}
	//Close files
	fiOne.close();
	fiTwo.close();

}
//Function used to help get spacing past the line Count
int spaceChar(int a) {
	int count = 1;
	if (a / 10 == 0) {
		return count;
	}
	else {
		a /= 10;
		while (a != 0) {
			a /= 10;
			count++;
		}
		return count;
	}
}
