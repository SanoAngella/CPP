#include <iostream>
#include <string>
#include <cctype>

#include "WordGame.h"

int main()
{
    std::string word;

    std::cout << "Enter the secret word: ";
    std::cin >> word;

    for (char& c : word)
    {
        c = std::toupper(static_cast<unsigned char>(c));
    }

    WordGame game(word);

    while (!game.isGameOver())
    {
        game.displayGame();

        char guess;

        std::cout << "Guess a letter: ";
        std::cin >> guess;

        game.makeGuess(guess);
    }

    game.displayGame();

    if (game.hasWon())
    {
        std::cout << "\nCongratulations! You guessed the word!\n";
        std::cout << "The word was: " << word << "\n";
    }
    else
    {
        std::cout << "\nGame over!\n";
        std::cout << "The word was: " << word << "\n";
    }

    return 0;
}