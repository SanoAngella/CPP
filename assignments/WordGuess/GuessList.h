#ifndef GUESSLIST_H
#define GUESSLIST_H

#include <string>

class GuessList
{
private:
    struct Node
    {
        char letter;
        Node* next;
    };

    Node* head;

public:
    GuessList();
    GuessList(const GuessList& other);
    GuessList& operator=(const GuessList& other);
    ~GuessList();

    void add(char letter);
    bool contains(char letter) const;

    std::string getLetters() const;
};

#endif