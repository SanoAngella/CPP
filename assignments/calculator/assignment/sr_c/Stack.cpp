#include "Stack.h"
#include <stdexcept>

template <typename T>
Stack<T>::Stack(int size)
{
    capacity = size;
    data = new T[capacity];
    topIndex = -1;
}

template <typename T>
Stack<T>::~Stack()
{
    delete[] data;
}

template <typename T>
void Stack<T>::push(T value)
{
    if (isFull())
        throw std::overflow_error("Stack is full");

    data[++topIndex] = value;
}

template <typename T>
void Stack<T>::pop()
{
    if (isEmpty())
        throw std::underflow_error("Stack is empty");

    topIndex--;
}

template <typename T>
T Stack<T>::top() const
{
    if (isEmpty())
        throw std::underflow_error("Stack is empty");

    return data[topIndex];
}

template <typename T>
bool Stack<T>::isEmpty() const
{
    return topIndex == -1;
}

template <typename T>
bool Stack<T>::isFull() const
{
    return topIndex == capacity - 1;
}

// Explicit instantiations
template class Stack<double>;
template class Stack<char>;