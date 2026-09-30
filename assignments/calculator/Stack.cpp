#include "Stack.h"
#include <iostream>

using namespace std;

Stack::Stack(int size)
{
    capacity = size;
    data = new int[capacity];

    topIndex = -1;
}

Stack::~Stack()
{
    delete[] data;
}

bool Stack::isEmpty()
{
    return topIndex == -1;
}

bool Stack::isFull()
{
    return topIndex == capacity - 1;
}

void Stack::push(int value)
{
    if (isFull())
    {
        cout << "Stack is full." << endl;
        return;
    }

    topIndex++;

    data[topIndex] = value;
}

void Stack::pop()
{
    if (isEmpty())
    {
        cout << "Stack is empty." << endl;
        return;
    }

    topIndex--;
}

int Stack::top()
{
    if (isEmpty())
    {
        cout << "Stack is empty." << endl;
        return -1;
    }

    return data[topIndex];
}