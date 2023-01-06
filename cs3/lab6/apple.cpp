// sorting apples
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

struct Apples {
    double weight; // oz
    string color;  // red or green
    void print() const { cout << color << ", " << weight << endl; }
};

int main() {
    srand(time(nullptr));
    const double minWeight = 8.;
    const double maxWeight = 3.;

    cout << "Input crate size: ";
    int size;
    cin >> size;

    vector <Apples> crate(size);


    // assign random weight and color to apples in the crate
    // replace with generate()
    std::generate(crate.begin(), crate.end(), [=]() {
        Apples appOne; appOne.weight = minWeight + static_cast<double>(rand()) / RAND_MAX * (maxWeight - minWeight); 
        appOne.color = rand() % 2 == 1 ? "green" : "red";
        return appOne; 
    });


    // for_each() possibly
    std::for_each(crate.begin(), crate.end(), [](Apples appPrint) {appPrint.print();});

    cout << "Enter weight to find: ";
    double toFind;
    cin >> toFind;

    // count_if()
    int cnt = std::count_if(crate.begin(), crate.end(), [=](Apples appCompare) {return appCompare.weight > toFind; });

    cout << "There are " << cnt << " apples heavier than "
        << toFind << " oz" << endl;

    // find_if()
    cout << "at positions ";
    auto it = std::find_if(crate.begin(), crate.end(), [=](Apples appPositioner) {return appPositioner.weight > toFind; });
    if (it != crate.end()) {
        do
        {
            cout << it - crate.begin() << ", ";
            it = std::find_if(++it, crate.end(), [=](Apples appPlacer) {return appPlacer.weight > toFind; });
        } while (it != crate.end());
    }
    else {
        cout << "None found. ";
        cout << endl;
    }
    cout << endl;
    


    // max_element()
    auto it2 = std::max_element(crate.begin(), crate.end(), [=](Apples appOne, Apples appTwo) {return appOne.weight < appTwo.weight; });
    cout << "Heaviest apple weighs: " << it2 ->weight << " oz" << endl;


    // for_each() or accumulate()
    double sum = 0;
    auto result = std::accumulate(crate.begin(), crate.end(), sum, [=](double sum,Apples apple) {return sum + apple.weight; });
    cout << "Total apple weight is: " << result << " oz" << endl;


    // transform();
    cout << "How much should they grow: ";
    double toGrow;
    cin >> toGrow;

    std::transform(crate.begin(), crate.end(), crate.begin(), [=](Apples appGrower) {Apples appOne;
    appOne.weight = appGrower.weight + toGrow;
    appOne.color = appGrower.color;
    return appOne;    
    });
   
    // remove_if()
    cout << "Input minimum acceptable weight: ";
    double minAccept;
    cin >> minAccept;
    // removing small apples
    
    crate.erase(std::remove_if(crate.begin(), crate.end(), [=](Apples& remover) {return remover.weight < minAccept; }), crate.end());
    
    // bubble sort, replace with sort()

    std::sort(crate.begin(), crate.end(), [](Apples appOne, Apples appTwo) {return appOne.weight < appTwo.weight; });
    
    // for_each() possibly
    cout << "sorted remaining apples" << endl;
    std::for_each(crate.begin(), crate.end(), [](Apples appPrint) {appPrint.print(); });
    
}

