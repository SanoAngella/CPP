#include <iostream>
#include <string>
using namespace std;

string rps(string p1, string p2) {
    return (p1 == p2) ? "TIE" :
           ((p1 == "rock" && p2 == "scissors") ||
            (p1 == "paper" && p2 == "rock") ||
            (p1 == "scissors" && p2 == "paper"))
            ? "Player 1 wins"
            : "Player 2 wins";
}

int main() {
    string p1, p2;

    cout << "Player 1: ";
    cin >> p1;

    cout << "Player 2: ";
    cin >> p2;

    cout << rps(p1, p2) << endl;
    return 0;
}
