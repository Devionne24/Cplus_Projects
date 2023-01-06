// figures: class adapter pattern
// Devionne Littleton
// 11/11/2022

#include <iostream>

using std::cout; using std::endl; using std::cin;

// base interface
class Figure {
public:
	virtual void draw() = 0;
	virtual ~Figure() {}
};

// adaptee/implementer
class LegacyRectangle {
public:
	LegacyRectangle(int topLeftX,
		int topLeftY,
		int bottomRightX,
		int bottomRightY) :
		topLeftX_(topLeftX),
		topLeftY_(topLeftY),
		bottomRightX_(bottomRightX),
		bottomRightY_(bottomRightY) {}

	void oldDraw() {
		for (int i = 0; i < bottomRightY_; ++i) {
			for (int j = 0; j < bottomRightX_; ++j)
				if (i >= topLeftY_ && j >= topLeftX_)
					cout << '*';
				else
					cout << ' ';
			cout << endl;
		}
	}

	//assigning new values to each corner
	void move(int topLeftX, int topLeftY, int bottomRightX, int bottomRightY) {
		topLeftX_ = topLeftX;
		topLeftY_ = topLeftY;
		bottomRightX_ = bottomRightX;
		bottomRightY_ = bottomRightY;
	}

	//getting values of each corner
	int& getTopLeftX() { return topLeftX_; }

	int& getTopLeftY() { return topLeftY_; }

	int& getBottomRightX() { return bottomRightX_; }

	int& getBottomRightY() { return bottomRightY_; }
	// defining top/left and bottom/right coordinates 
private:
	int topLeftX_;
	int topLeftY_;
	int bottomRightX_;
	int bottomRightY_;
};

// adapter uses multiple inheritance to inherit
// interface and implementation
class SquareAdapter : public Figure,
	private LegacyRectangle {
public:
	SquareAdapter(int size) :
		LegacyRectangle(0, 0, size, size) {};
	void draw() override {
		oldDraw();
	}

	int size() { return getBottomRightX(); }

	//resizing from the right
	void resize(int newSize) {
		move(0, 0, newSize, newSize);
	}
};


int main() {
	//getting inputs from user
	cout << "Size of a square? ";
	int size;
	cin >> size;
	//makes pointer object
	SquareAdapter* square = new SquareAdapter(size);
	square->draw();

	//resizing
	cout << "New size? ";
	cin >> size;
	square->resize(size);
	square->draw();
}
