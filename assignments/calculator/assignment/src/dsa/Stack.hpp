#ifndef STACK_HPP
#define STACK_HPP

#include "Stack.h"
#include <algorithm>

template <typename T>
Stack<T>::Stack(int initialCapacity)
    : capacity(initialCapacity > 0 ? initialCapacity : 16),
      topIndex(-1) {
    data = new T[capacity];
}

template <typename T>
Stack<T>::~Stack() {
    delete[] data;
    data = nullptr;
}

template <typename T>
Stack<T>::Stack(const Stack<T>& other)
    : capacity(other.capacity),
      topIndex(other.topIndex) {
    data = new T[capacity];
    for (int i = 0; i <= topIndex; ++i) {
        data[i] = other.data[i];
    }
}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack<T>& other) {
    if (this != &other) {
        T* newData = new T[other.capacity];
        for (int i = 0; i <= other.topIndex; ++i) {
            newData[i] = other.data[i];
        }
        delete[] data;
        data = newData;
        capacity = other.capacity;
        topIndex = other.topIndex;
    }
    return *this;
}

template <typename T>
void Stack<T>::resize(int newCapacity) {
    T* newData = new T[newCapacity];
    for (int i = 0; i <= topIndex; ++i) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

template <typename T>
void Stack<T>::push(const T& value) {
    // Check if array is full; if so, double capacity (amortized O(1) time)
    if (isFull()) {
        resize(capacity * 2);
    }
    topIndex++;
    data[topIndex] = value;
}

template <typename T>
void Stack<T>::pop() {
    if (isEmpty()) {
        throw std::underflow_error("Stack underflow: cannot pop from an empty stack.");
    }
    topIndex--;
}

template <typename T>
T& Stack<T>::top() {
    if (isEmpty()) {
        throw std::underflow_error("Stack is empty: no top element exists.");
    }
    return data[topIndex];
}

template <typename T>
const T& Stack<T>::top() const {
    if (isEmpty()) {
        throw std::underflow_error("Stack is empty: no top element exists.");
    }
    return data[topIndex];
}

template <typename T>
bool Stack<T>::isEmpty() const {
    return topIndex == -1;
}

template <typename T>
bool Stack<T>::isFull() const {
    return topIndex == capacity - 1;
}

template <typename T>
int Stack<T>::size() const {
    return topIndex + 1;
}

template <typename T>
int Stack<T>::getCapacity() const {
    return capacity;
}

template <typename T>
void Stack<T>::clear() {
    topIndex = -1;
}

#endif // STACK_HPP
