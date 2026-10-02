#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <algorithm>

#include "WordGame.h"

// Category Database with Word Banks
struct CategoryData {
    std::string name;
    std::vector<std::string> words;
};

static const std::vector<CategoryData> CATEGORIES = {
    {
        "Animals",
        { "ELEPHANT", "GIRAFFE", "CHEETAH", "KANGAROO", "PENGUIN", "DOLPHIN", "LEOPARD", "OCTOPUS", "CROCODILE", "FLAMINGO" }
    },
    {
        "Technology",
        { "COMPUTER", "ALGORITHM", "PROGRAMMING", "DEVELOPER", "KEYBOARD", "DATABASE", "INTERNET", "SECURITY", "SOFTWARE", "VARIABLE" }
    },
    {
        "Films & Movies",
        { "INCEPTION", "GLADIATOR", "TITANIC", "AVATAR", "INTERSTELLAR", "MATRIX", "CASABLANCA", "GODFATHER", "JURASSIC" }
    },
    {
        "Countries & Districts",
        { "GERMANY", "BRAZIL", "AUSTRALIA", "CANADA", "JAPAN", "PORTUGAL", "RWANDA", "SWITZERLAND", "ARGENTINA", "NORWAY" }
    },
    {
        "Books & Literature",
        { "HAMLET", "ODYSSEY", "DRACULA", "MACBETH", "FRANKENSTEIN", "HOBBIT", "GATSBY", "ILLIAD" }
    }
};

void showCategoryMenu()
{
    std::cout << "\n============================================\n";
    std::cout << "         WORDLE WORD GUESSING GAME         \n";
    std::cout << "============================================\n";
    std::cout << " Please choose a category to guess from:\n\n";

    for (size_t i = 0; i < CATEGORIES.size(); ++i)
    {
        std::cout << "  [" << (i + 1) << "] " << CATEGORIES[i].name << "\n";
    }

    std::cout << "  [0] Exit Game\n";
    std::cout << "--------------------------------------------\n";
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    bool playAgain = true;

    while (playAgain)
    {
        showCategoryMenu();

        std::string choiceInput;
        int selectedCategory = -1;

        while (true)
        {
            std::cout << "Enter your choice (0-" << CATEGORIES.size() << ") or 'exit': ";
            std::cin >> choiceInput;

            std::string lowerInput = choiceInput;
            for (char& c : lowerInput) c = std::tolower(static_cast<unsigned char>(c));

            if (lowerInput == "exit" || lowerInput == "0")
            {
                std::cout << "\nThank you for playing Wordle Guess! Goodbye!\n";
                return 0;
            }

            try
            {
                int val = std::stoi(choiceInput);
                if (val >= 1 && val <= static_cast<int>(CATEGORIES.size()))
                {
                    selectedCategory = val - 1;
                    break;
                }
            }
            catch (...) {}

            std::cout << "Invalid choice! Please select a valid number between 1 and " << CATEGORIES.size() << ".\n";
        }

        // Pick random secret word from the selected category
        const CategoryData& cat = CATEGORIES[selectedCategory];
        int wordIdx = std::rand() % cat.words.size();
        std::string secretWord = cat.words[wordIdx];

        WordGame game(secretWord, 6, cat.name);

        std::cout << "\n[OK] Category Selected: " << cat.name << "!\n";
        std::cout << "The game has chosen a secret word. Let's start guessing!\n";
        std::cout << "(Tip: Type 'exit' at any time to quit the program)\n";

        // Main Guessing Loop
        while (!game.isGameOver())
        {
            game.displayGame();

            std::string input;
            std::cout << "Choose a letter of the alphabet (or type 'exit'): ";
            std::cin >> input;

            std::string lowerInput = input;
            for (char& c : lowerInput) c = std::tolower(static_cast<unsigned char>(c));

            // Check for exit command
            if (lowerInput == "exit")
            {
                std::cout << "\nGame aborted by user. The secret word was: " << game.getSecretWord() << "\n";
                std::cout << "Goodbye!\n";
                return 0;
            }

            if (input.length() != 1 || !std::isalpha(static_cast<unsigned char>(input[0])))
            {
                std::cout << "\n>> [!] Please enter a single alphabetical letter (A-Z).\n";
                continue;
            }

            char letter = static_cast<char>(std::toupper(static_cast<unsigned char>(input[0])));

            // Check if already guessed
            if (game.getGuessedLetters().find(letter) != std::string::npos)
            {
                std::cout << "\n>> [!] You already guessed '" << letter << "'! Try a different letter.\n";
                continue;
            }

            bool found = game.makeGuess(letter);

            if (found)
            {
                std::cout << "\n>> [CORRECT!] Good job, '" << letter << "' is in the word!\n";
            }
            else
            {
                std::cout << "\n>> [INCORRECT] Sorry, '" << letter << "' is not in the word.\n";
            }
        }

        // Final Game Result Display
        game.displayGame();

        if (game.hasWon())
        {
            std::cout << "********************************************\n";
            std::cout << "  CONGRATULATIONS! YOU WON!                \n";
            std::cout << "  You correctly guessed: " << game.getSecretWord() << "\n";
            std::cout << "********************************************\n\n";
        }
        else
        {
            std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n";
            std::cout << "  GAME OVER! You ran out of chances.        \n";
            std::cout << "  The secret word was: " << game.getSecretWord() << "\n";
            std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\n";
        }

        // Ask user if they want to play again
        while (true)
        {
            std::string response;
            std::cout << "Do you want to play again? (y/n): ";
            std::cin >> response;

            for (char& c : response) c = std::tolower(static_cast<unsigned char>(c));

            if (response == "y" || response == "yes")
            {
                playAgain = true;
                break;
            }
            else if (response == "n" || response == "no" || response == "exit")
            {
                playAgain = false;
                std::cout << "\nThank you for playing Wordle Guess! Have a great day!\n";
                break;
            }
            else
            {
                std::cout << "Please enter 'y' for yes or 'n' for no.\n";
            }
        }
    }

    return 0;
}