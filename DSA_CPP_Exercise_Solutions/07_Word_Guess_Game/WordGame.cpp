#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cctype>
using namespace std;

bool contains(const vector<char>& guessed, char letter) {
    for (char c : guessed) {
        if (c == letter) return true;
    }
    return false;
}

bool complete(const string& word, const vector<char>& guessed) {
    for (char c : word) {
        if (!contains(guessed, c)) return false;
    }
    return true;
}

void showWord(const string& word, const vector<char>& guessed) {
    for (char c : word) {
        if (contains(guessed, c))
            cout << c << ' ';
        else
            cout << "_ ";
    }
    cout << endl;
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    const string animals[] = {
        "elephant", "lion", "giraffe", "zebra", "rabbit"
    };

    const string teams[] = {
        "arsenal", "chelsea", "barcelona", "liverpool", "milan"
    };

    const string districts[] = {
        "kicukiro", "gasabo", "nyarugenge", "musanze", "huye"
    };

    const string films[] = {
        "avatar", "inception", "titanic", "gladiator", "frozen"
    };

    const string books[] = {
        "hamlet", "macbeth", "matilda", "hobbit", "odyssey"
    };

    char again = 'y';

    while (again == 'y' || again == 'Y') {
        cout << "\n===== WORD GUESS GAME =====\n";
        cout << "1. Animals\n";
        cout << "2. Teams\n";
        cout << "3. Districts\n";
        cout << "4. Films\n";
        cout << "5. Books\n";

        int category;
        cout << "Choose a category: ";
        cin >> category;

        const string* words = nullptr;
        int count = 0;

        switch (category) {
            case 1: words = animals; count = 5; break;
            case 2: words = teams; count = 5; break;
            case 3: words = districts; count = 5; break;
            case 4: words = films; count = 5; break;
            case 5: words = books; count = 5; break;
            default:
                cout << "Invalid category.\n";
                continue;
        }

        string secret = words[rand() % count];
        vector<char> guessed;
        int attempts = 6;

        while (attempts > 0 && !complete(secret, guessed)) {
            cout << "\nWord: ";
            showWord(secret, guessed);
            cout << "Attempts remaining: " << attempts << endl;

            cout << "Enter a letter or type exit: ";
            string input;
            cin >> input;

            if (input == "exit" || input == "EXIT") {
                cout << "Game exited.\n";
                return 0;
            }

            if (input.length() != 1 || !isalpha(static_cast<unsigned char>(input[0]))) {
                cout << "Please enter exactly one letter.\n";
                continue;
            }

            char guess = static_cast<char>(
                tolower(static_cast<unsigned char>(input[0]))
            );

            if (contains(guessed, guess)) {
                cout << "You already guessed that letter.\n";
                continue;
            }

            guessed.push_back(guess);

            if (secret.find(guess) != string::npos) {
                cout << "Correct!\n";
            } else {
                cout << "Wrong!\n";
                attempts--;
            }
        }

        if (complete(secret, guessed)) {
            cout << "\nYou WIN! The word was: " << secret << endl;
        } else {
            cout << "\nYou LOSE! The word was: " << secret << endl;
        }

        cout << "Play again? (y/n): ";
        cin >> again;
    }

    return 0;
}
