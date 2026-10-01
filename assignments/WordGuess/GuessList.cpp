#include "GuessList.h"

#include <cctype>

GuessList::GuessList()
{
    head = nullptr;
}

GuessList::GuessList(const GuessList& other)
{
    head = nullptr;
    Node* current = other.head;
    while (current != nullptr)
    {
        add(current->letter);
        current = current->next;
    }
}

GuessList& GuessList::operator=(const GuessList& other)
{
    if (this != &other)
    {
        Node* current = head;
        while (current != nullptr)
        {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head = nullptr;

        Node* otherCurrent = other.head;
        while (otherCurrent != nullptr)
        {
            add(otherCurrent->letter);
            otherCurrent = otherCurrent->next;
        }
    }
    return *this;
}

GuessList::~GuessList()
{
    Node* current = head;

    while (current != nullptr)
    {
        Node* next = current->next;

        delete current;

        current = next;
    }
}

void GuessList::add(char letter)
{
    letter = std::toupper(
        static_cast<unsigned char>(letter)
    );

    if (contains(letter))
        return;

    Node* newNode = new Node;

    newNode->letter = letter;
    newNode->next = nullptr;

    if (head == nullptr)
    {
        head = newNode;
        return;
    }

    Node* current = head;

    while (current->next != nullptr)
    {
        current = current->next;
    }

    current->next = newNode;
}

bool GuessList::contains(char letter) const
{
    letter = std::toupper(
        static_cast<unsigned char>(letter)
    );

    Node* current = head;

    while (current != nullptr)
    {
        if (current->letter == letter)
            return true;

        current = current->next;
    }

    return false;
}

std::string GuessList::getLetters() const
{
    std::string result;

    Node* current = head;

    while (current != nullptr)
    {
        result += current->letter;
        result += ' ';

        current = current->next;
    }

    return result;
}