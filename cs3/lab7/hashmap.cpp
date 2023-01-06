// hashmap player file 
// Devionne Littleton
// 10/28/2022

#include "hashmap.hpp"
#include <iostream>
#include <string>

using std::cin; using std::cout; using std::endl;
using std::string;

// template tests hashmap inserts
template <typename Key, typename Value>
void test_insert(hashmap<Key, Value>& hm, Key key, Value value) {
    cout << "[*] Player " << key << ": " << value << "...\n";
    pair<const pair<const Key, Value>*, bool> result = hm.insert(make_pair(key, value));

    if (result.second)
        cout << "--> Player Inserted!\n";
    else
        cout << "--> Failed! Player " << result.first->first
        << " already exists with value " << result.first->second << endl;
}

// template tests hashmap erases
template <typename Key, typename Value>
void test_erase(hashmap<Key, Value>& hm, Key key) {
    cout << "[*] Game Over for " << key << "...\n";
    pair<const pair<const Key, Value>*, bool> result = hm.erase(key);

    if (result.second) {
        cout << "--> And they're Gone! ";
        if (result.first)
            cout << "The next player is " << result.first->first
            << ": " << result.first->second << endl;
        else
            cout << "There is no other player\n";
    }
    else {
        cout << "--> Failed! Player " << key << " doesn't exist!\n";
    }
}


int main() {
    hashmap<int, string> players;

    cout << "Let's make a game! Insert Player Number: "<< endl;
    //inserting using input
    int numPlayer;
    string namePlayer;
    
    cin >> numPlayer;
    cout << "Now insert Player Name: " << endl;
    cin >> namePlayer;
    //Player should be in system 
    test_insert(players, numPlayer, namePlayer);

    cout << "Now inserting 3 more players to have in the game!" << endl;
    // testing hard coded inserts

    test_insert<int, string>(players, 2, "Sonic");
    test_insert<int, string>(players, 3, "Ash Ketchum");
    test_insert<int, string>(players, 4, "Shulk");

    //testing insertion of duplicate
    test_insert<int, string>(players, 1, "Mario");

    //testing the finder
    cout << "Finding Player One" << endl;
    auto x = players.find(1);
    if (x != nullptr)
        cout << "Player One is currently: " << x->second << endl;
    else
        cout << "Player One is not in the game." << endl;


    // deleting last player
    cout << "Now deleting Sonic because Mario is better!" << endl;
    test_erase(players, 2);

    //testing erasing a non-exsistent player
    test_erase(players, 5);

    cout << "Replacing Sonic with Crash Bandicoot!" << endl;
    test_insert<int, string>(players, 2, "Crash Bandicoot!");

    //removing from hashmap using inputs
    cout << "Remove any other player within the game by entering their player number." << endl;
    cin >> numPlayer;
    test_erase(players, numPlayer);
    
    // now testing rehash
   
    cout << endl << "will now be testing rehash" << endl;
    players.rehash(3);
    players.rehash(1);
    cout << "[*] New number of buckets is " << players.getNumBuckets() << endl;

    // testing if hashmap is still correct after rehash
    cout << 3 << ": " << players[3] << endl;
    cout << 1 << ": " << players[1] << endl;

    
}
