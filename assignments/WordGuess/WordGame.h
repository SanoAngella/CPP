#ifndef WORDGAME_H
#define WORDGAME_H

#include <string>
#include "GuessList.h"

class WordGame
{
private:
    std::string secretWord;
    std::string hiddenWord;
    std::string category;

    int attempts;

    GuessList guessedLetters;

    bool isWordComplete() const;
    void revealLetter(char letter);

public:
    WordGame(
        const std::string& word,
        int maxAttempts = 6,
        const std::string& cat = "General"
    );

    void displayGame() const;

    bool makeGuess(char letter);

    bool hasWon() const;
    bool hasLost() const;
    bool isGameOver() const;

    std::string getHiddenWord() const;
    std::string getSecretWord() const;
    std::string getCategory() const;
    int getAttempts() const;
    std::string getGuessedLetters() const;
};

#endif