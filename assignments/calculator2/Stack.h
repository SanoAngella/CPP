#ifndef STACK_H
#define STACK_H

template <typename T>
class Stack
{
private:
    T* data;
    int capacity;
    int topIndex;

public:
    Stack(int size = 100);
    ~Stack();

    void push(T value);
    void pop();
    T top() const;

    bool isEmpty() const;
    bool isFull() const;
};

#endif
