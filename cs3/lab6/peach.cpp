// jamming peaches
// non STL-algorithm code to be replaced by algorthms
// Devionne Littleton
// 10/7/2022


#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <deque>
#include <string>
#include<iterator>
#include <algorithm>
#include<functional>
#include<numeric>

using std::cin; using std::cout; using std::endl;
using std::string;
using std::vector; using std::deque;

struct Peaches {
    double weight; // oz
    bool ripe;  // ripe or not
    void print() const { cout << (ripe ? "ripe" : "green") << ", " << weight << endl; }
};

class peachJam {
public:
    peachJam(double cap): pJamCap_(cap) {};
    double operator() (double sumOfPeaches, const Peaches& x) {
        if (x.weight < pJamCap_) {
            return sumOfPeaches += x.weight;
        }
        return sumOfPeaches;
    }
private:
    double pJamCap_;
};



int main() {
    srand(time(nullptr));
    const double minWeight = 8.;
    const double maxWeight = 3.;

    cout << "Input basket size: ";
    int size;
    cin >> size;

    vector <Peaches> basket(size);

    // assign random weight and ripeness peaches in the basket
    // replace with generate()
    std::generate(basket.begin(), basket.end(), [=]() {
        Peaches peaOne; peaOne.weight = minWeight + static_cast<double>(rand()) / RAND_MAX * (maxWeight - minWeight);
        peaOne.ripe = rand() % 2;
        return peaOne;
    });



    // for_each() possibly
    cout << "all peaches" << endl;
    std::for_each(basket.begin(), basket.end(), [](Peaches peaPrint) {peaPrint.print(); });

    // moving all the ripe peaches from basket to peck
    // remove_copy_if() with back_inserter()/front_inserter() or equivalents
    deque<Peaches> peck;
    std::remove_copy_if(basket.begin(), basket.end(), std::front_inserter(peck), [](Peaches peaRipe) {return peaRipe.ripe == false; });

    basket.erase(std::remove_if(basket.begin(), basket.end(), [](Peaches peaOne) {return peaOne.ripe == true; }), basket.end());
   
    // for_each() possibly
    cout << "peaches remainng in the basket" << endl;
    std::for_each(basket.begin(), basket.end(), [](Peaches peaPrint) {peaPrint.print(); });

    cout << endl;

    // for_each() possibly
    cout << "peaches moved to the peck" << endl;
    std::for_each(peck.begin(), peck.end(), [](Peaches peaPrintPeck) {peaPrintPeck.print(); });


    // prints every "space" peach in the peck
    // 
    const int space = 3;
    cout << "\nevery " << space << "\'d peach in the peck" << endl;

    // replace with advance()/next()/distance()
    // no iterator arithmetic
   
    auto it = peck.cbegin(); int i = 0;
    std::advance(it, space);
    if (it != peck.cend()) {
        do
        {
            it->print();
            if (std::distance(it, peck.cend()) < space) {
                it = peck.cend();
            }
            else {
                std::advance(it, space);
            }
        } while (it != peck.cend());
    }
    

    // putting all small ripe peaches in a jam
    // use a binder to create a functor with configurable max weight
    // accumulate() or count_if() then remove_if()


    const double weightToJam = 10.0;
    double jamWeight = 0;
    jamWeight = std::accumulate(peck.begin(), peck.end(), jamWeight, std::bind(peachJam(weightToJam), std::placeholders::_1, std::placeholders::_2));

    cout << "Final weight of the jam is: " << jamWeight << "\n";

}
