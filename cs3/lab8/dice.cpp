// Games, Template Method, Added Dice Example
// Devionne Littleton
// 10/28/2022

#include <ctime>
#include <cstdlib>
#include <iostream>

using std::cout; using std::endl;

// template for any game where players take 
// turns to make moves
// and there is a winner
class Game {
public:
    Game() :playersCount_(0), movesCount_(0), playerWon_(noWinner) {}

    // template method
    void playGame(const int playersCount = 0) {
        playersCount_ = playersCount;
        movesCount_ = 0;

        initializeGame();

        for (int i = 0; !endOfGame(); i = (i + 1) % playersCount_) {
            makeMove(i);
            if (i == playersCount_ - 1)
                ++movesCount_;
        }
        printWinner();
    }

    virtual ~Game() {}

protected:
    // primitive operations
    virtual void initializeGame() = 0;
    virtual void makeMove(int player) = 0;
    virtual void printWinner() = 0;
    virtual bool endOfGame() { return playerWon_ != noWinner; } // this is a hook
    // returns true if winner is decided
    static const int noWinner = -1;

    int playersCount_;
    int movesCount_;
    int playerWon_;
};

// Monopoly - a concrete game implementing primitive 
// operations for the template method
class Monopoly : public Game {
public:
    // implementing concrete methods
    void initializeGame() {
        playersCount_ = rand() % numPlayers_ + 1; // initialize players
    }

    void makeMove(int player) {
        if (movesCount_ > minMoves_) {
            const int chance = minMoves_ + rand() % (maxMoves_ - minMoves_);
            if (chance < movesCount_) playerWon_ = player;
        }
    }

    void printWinner() {
        cout << "Monopoly, player " << playerWon_ << " won in "
            << movesCount_ << " moves." << endl;
    }

private:
    static const int numPlayers_ = 8; // max number of players
    static const int minMoves_ = 20; // nobody wins before minMoves_
    static const int maxMoves_ = 200; // somebody wins before maxMoves_
};

// Chess - another game implementing
// primitive operations
class Chess : public Game {
public:
    void initializeGame() {
        playersCount_ = numPlayers_; // initalize players
        for (int i = 0; i < numPlayers_; ++i)
            experience_[i] = rand() % maxExperience_ + 1;
    }

    void makeMove(int player) {
        if (movesCount_ > minMoves_) {
            const int chance = (rand() % maxMoves_) / experience_[player];
            if (chance < movesCount_) playerWon_ = player;
        }
    }

    void printWinner() {
        cout << "Chess, player " << playerWon_
            << " with experience " << experience_[playerWon_]
            << " won in " << movesCount_ << " moves over"
            << " player with experience " << experience_[playerWon_ == 0 ? 1 : 0]
            << endl;
    }

private:
    static const int numPlayers_ = 2;
    static const int minMoves_ = 2; // nobody wins before minMoves_
    static const int maxMoves_ = 100; // somebody wins before maxMoves_
    static const int maxExperience_ = 5; // player's experience
    // the higher, the greater probability of winning
    int experience_[numPlayers_];
};

class Dice : public Game {
public: 
    void initializeGame() {
        //sets players with a start of 0
        playersCount_ = numPlayers_;
        movesCount_ = 1;
        for (int i = 0; i < diceRoll_; i++) {
            cpuScore_[i] = 0;
            playerScore_[i] = 0;
            cpuSum_ = cpuSum_ + cpuScore_[i];
            pSum_ = pSum_ + playerScore_[i];
        }
    }

    void makeMove(int player) {
        //player 0 is CPU
        player = 0;
        cout << "Let's Play Dice!" << endl;
        //will continue until max move count or both player say no
        while (movesCount_ <= maxMoves_ && (pChooser_ == 'y' || cpuChooser_ == 0)) {
            cout << "Round " << movesCount_ << ": " << endl;
            if (player == 0 && cpuChooser_ == 0) {
                //resets sum
                cpuSum_ = 0;
                cout << "Computer Rolled: ";
                for (int i = 0; i < diceRoll_; i++) {
                    cpuScore_[i] = rand() % diceAmount_ + 1;
                    //adding sum together through loop of array
                    cpuSum_ = cpuSum_ + cpuScore_[i];
                    cout << cpuScore_[i] << " ";
                }
                cout << ", computer's score: " << cpuSum_;
                ++player;
            }
            else {
                //outputs last round score
                cout << "Kept Roll: ";
                for (int i = 0; i < diceRoll_; i++) {
                    cout << cpuScore_[i] << " ";
                }
                cout << ", computer's score: " << cpuSum_;
                //sets up for player 1
                ++player;
            }
            cout << endl;
            if (player == 1 && pChooser_ == 'y') {
                pSum_ = 0;
                cout << "Player Rolled: ";
                for (int i = 0; i < diceRoll_; i++) {
                    playerScore_[i] = rand() % diceAmount_ + 1;
                    pSum_ = pSum_ + playerScore_[i];
                    cout << playerScore_[i] << " ";
                }
                cout << ", your score: " << pSum_;
            }
            else {
                cout << "Kept Roll: ";
                for (int i = 0; i < diceRoll_; i++) {
                    cout << playerScore_[i] << " ";
                }
                cout << ", your score: " << pSum_;
            }
            cout << endl;
            //gves turn back to cpu
            --player;
            
            if (movesCount_ < 3) {
                cout << "Roll Again? [y/n] (lowercase) ";
                std::cin >> pChooser_;
            }
            //cpu decides if it wants to roll again randomly
            cpuChooser_ = rand() % 2;
            //will show if both players keep rolls
            cout << endl;
            ++movesCount_;
        }
        cout << "Final Scores: " << endl;
        cout << "Computer: " << cpuSum_ << endl;
        cout << "Player: " << pSum_ << endl;
        if (cpuSum_ > pSum_) {
            playerWon_ = 0;
        }
        else if (pSum_ > cpuSum_) {
            playerWon_ = 1;
        }
        else {
            playerWon_ = 2;
        }
    }

    void printWinner() {
        if (playerWon_ == 0) {
            cout << "Computer Wins. You lose." << endl;
        }
        else if (playerWon_ == 1) {
            cout << "Congratulations! You win!" << endl;
        }
        else if (playerWon_ == 2) {
            cout << "The game ended in a draw." << endl;
        }
    }
private: 
    static const int numPlayers_ = 2;
    static const int maxMoves_ = 3;
    static const int diceRoll_ = 5;
    char pChooser_ = 'y';
    int cpuChooser_ = 0;
    int cpuSum_ = 0;
    int pSum_ = 0;
    int diceAmount_ = 6;
    int cpuScore_[5];
    int playerScore_[5];
};

int main() {
    srand(time(nullptr));

    Game* gp = nullptr;
    /*
    // play chess 8 times
    for (int i = 0; i < 8; ++i) {
        gp = new Chess;
        gp->playGame();
        delete gp;
    }


    // play monopoly 8 times
    for (int i = 0; i < 8; ++i) {
        gp = new Monopoly;
        gp->playGame();
        delete gp;
    }
   */
    for (int i = 0; i < 3; ++i) {
        gp = new Dice;
        gp->playGame();
        delete gp;
    }
}
