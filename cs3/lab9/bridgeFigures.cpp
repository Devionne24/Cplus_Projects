// filled/hollow figures demonstrates Bridge Design Pattern,
// square is either hollow or square and is painted with a particular character,
// it is bridged over Figure --> Fill abstract body/handle
// Devionne Littleton
// 11/04/2022

#include <iostream>
#include <array>

using std::cout; using std::endl; using std::cin;

// abstract body
class Fill {
public:
	Fill(char fillChar, char borderChar) : fillChar_(fillChar), borderChar_(borderChar) {}
	virtual char getBorder() = 0;
	virtual char getInternal() = 0;
	virtual ~Fill() {}
protected:
	char fillChar_;
	char borderChar_;
};

// concrete body
class Hollow : public Fill {
public:
	Hollow(char fillChar) :Fill(' ', fillChar) {}
	char getBorder() override { return borderChar_; }
	char getInternal() override { return fillChar_; }
	~Hollow() override {}
};


// another concrete body
class Filled : public Fill {
public:
	Filled(char fillChar) : Fill(fillChar, fillChar) {};
	Filled(char fillChar, char borderChar) :Fill(fillChar, borderChar) {}
	char getBorder() override { return borderChar_; }
	char getInternal() override { return fillChar_; }
	~Filled() override {}
};

class FullyFilled : public Filled {
public:
	FullyFilled(char fillChar, char borderChar):Filled(fillChar, borderChar) {}
	char getBorder() override { return borderChar_; }
	char getInternal() override { return fillChar_; }
	~FullyFilled() override {}
};

class RandomFilled : public Filled {
public:
	RandomFilled(char forbChar, char borfChar) : Filled(forbChar, borfChar) {}
	//choosing between fillChar and borderChar
	char getBorder() override {
		srand(time(nullptr));
		int randChar = rand() % 2;
		if (randChar == 0) {
			return borderChar_;
		}
		else {
			return fillChar_;
		}
	}
	char getInternal() override {
		srand(time(nullptr));
		int randChar = rand() % 2;
		if (randChar == 0) {
			return fillChar_;
		}
		else {
			return borderChar_;
		}
	}
	~RandomFilled() override {}
};
// abstract handle
class Figure {
public:
	Figure(int size, Fill* fill) : size_(size), fill_(fill) {}
	virtual void draw() = 0;
	virtual ~Figure() {}
protected:
	int size_;
	Fill* fill_;
};

// concrete handle
class Square : public Figure {
public:
	Square(int size, Fill* fill) : Figure(size, fill) {}
	void draw() override;

};

void Square::draw() {
	for (int i = 0; i < size_; ++i) {
		for (int j = 0; j < size_; ++j)
			if (i == 0 || j == 0 || i == size_ - 1 || j == size_ - 1)
				cout << fill_->getBorder();
			else
				cout << fill_->getInternal();
		cout << endl;
	}
}

int main() {
	srand(time(nullptr));
	// creating an array of characters
	std::array<char, 6> charArray = { 'd', 'e', 'v', 'i', 'o', 'n' };

	//choosing random character from array
	char fchar = charArray[rand() % 6];
	cout << "Now creating 4 squares based on randomly selected characters from this array: [d, e, v, i, o, n]" << endl;
	cout << "Enter size for all the squares: " ; int size; cin >> size;
	cout << endl;

	//creating the squares
	cout << "First, the filled square" << endl;
	Figure* userBox = new Square(size, 
		static_cast<Fill*>(new Filled(fchar))
	);

	userBox->draw();
	cout << endl;

	//clearing userBox after every square
	delete userBox;

	fchar = charArray[rand() % 6];
	cout << "Second, the hollow square" << endl;

	userBox = new Square(size, static_cast<Fill*>(new Hollow(fchar)));
	
	userBox->draw();
	cout << endl;

	cout << "Third is the Fully Filled square, this introduces a border character!" << endl;

	//introducing second character, chosen randomly as well
	fchar = charArray[rand() % 6];
	char bchar = charArray[rand() % 6];

	userBox = new Square(size,
		static_cast<Fill*>(new FullyFilled(fchar, bchar)) 
	);

	userBox->draw();
	cout << endl;

	delete userBox;

	cout << "Finally we have the Randomly Filled square" << endl;

	//using set characters just for randomly filled
	fchar = charArray[0];
	bchar = charArray[1];

	userBox = new Square(size,
		static_cast<Fill*>(new RandomFilled(fchar, bchar))
	);

	userBox->draw();
	delete userBox;
}
