// selecting oranges
// converting vectors to multimaps
// Devionne Littleton
// 10/7/2022


#include <iostream>
#include <map>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <string>


using std::cin; using std::cout; using std::endl;
using std::string; using std::multimap;
using std::vector; using std::pair;

enum class Variety { orange, pear, apple };
vector<string> colors = { "red", "green", "yellow" };

multimap<Variety, string> fruits;


int main() {
    srand(time(nullptr));

    int tree = rand() % 100 + 1;
    // random fruit and color selection
    for (int m = 0; m < tree; ++m) {
        fruits.emplace(static_cast<Variety>(rand() % 3), colors[rand() % 3]);
    }


    // printing colors of oranges
    cout << "Colors of the oranges: ";
    for (auto f = fruits.lower_bound(Variety::orange); f != fruits.upper_bound(Variety::orange); ++f)
        cout << f->second << ", ";
    cout << endl;

}
