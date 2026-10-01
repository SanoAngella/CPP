#ifndef WORDGAME_H
#define WORDGAME_H

#include <string>
#include "GuessList.h"

class WordGame
{
private:
    std::string secretWord;
    std::string hiddenWord;

    int attempts;

    GuessList guessedLetters;

    bool isWordComplete() const;
    void revealLetter(char letter);

public:
    WordGame(
        const std::string& word,
        int maxAttempts = 6
    );

    void displayGame() const;

    bool makeGuess(char letter);

    bool hasWon() const;
    bool hasLost() const;
    bool isGameOver() const;

    std::string getHiddenWord() const;
    std::string getSecretWord() const;
    int getAttempts() const;
    std::string getGuessedLetters() const;
};

#endif