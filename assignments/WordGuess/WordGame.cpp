#include "WordGame.h"

#include <iostream>
#include <cctype>

WordGame::WordGame(
    const std::string& word,
    int maxAttempts
)
{
    secretWord = word;
    for (char& c : secretWord)
    {
        c = std::toupper(static_cast<unsigned char>(c));
    }

    attempts = maxAttempts;

    hiddenWord = "";

    for (char c : secretWord)
    {
        if (c == ' ')
            hiddenWord += ' ';
        else
            hiddenWord += '_';
    }
}

void WordGame::displayGame() const
{
    std::cout << "\n================================\n";
    std::cout << "       WORD GUESSING GAME\n";
    std::cout << "================================\n\n";

    std::cout << "Word: ";

    for (char c : hiddenWord)
    {
        std::cout << c << " ";
    }

    std::cout << "\n\nAttempts remaining: "
              << attempts;

    std::cout << "\nGuessed letters: "
              << guessedLetters.getLetters();

    std::cout << "\n\n";
}

void WordGame::revealLetter(char letter)
{
    for (size_t i = 0; i < secretWord.length(); i++)
    {
        if (secretWord[i] == letter)
        {
            hiddenWord[i] = letter;
        }
    }
}

bool WordGame::makeGuess(char letter)
{
    letter = std::toupper(
        static_cast<unsigned char>(letter)
    );

    if (!std::isalpha(
            static_cast<unsigned char>(letter)))
    {
        return false;
    }

    if (guessedLetters.contains(letter))
    {
        return false;
    }

    guessedLetters.add(letter);

    bool found = false;

    for (char c : secretWord)
    {
        if (c == letter)
        {
            found = true;
            break;
        }
    }

    if (found)
    {
        revealLetter(letter);
    }
    else
    {
        attempts--;
    }

    return found;
}

bool WordGame::isWordComplete() const
{
    return hiddenWord == secretWord;
}

bool WordGame::hasWon() const
{
    return isWordComplete();
}

bool WordGame::hasLost() const
{
    return attempts <= 0;
}

bool WordGame::isGameOver() const
{
    return hasWon() || hasLost();
}

std::string WordGame::getHiddenWord() const
{
    return hiddenWord;
}

std::string WordGame::getSecretWord() const
{
    return secretWord;
}

int WordGame::getAttempts() const
{
    return attempts;
}

std::string WordGame::getGuessedLetters() const
{
    return guessedLetters.getLetters();
}