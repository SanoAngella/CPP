#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <stdexcept>

/**
 * @brief Generic Stack Data Structure implemented using a Dynamic Array.
 * 
 * DSA Concept:
 * A Stack is a linear data structure that follows the LIFO (Last-In, First-Out)
 * principle. The element added most recently is the first one to be removed.
 * 
 * Why Dynamic Array instead of Static Array?
 * A fixed-size array limits maximum expression complexity or requires excessive
 * pre-allocated memory. Dynamic resizing (doubling capacity when full) guarantees
 * amortized O(1) push time while preventing stack overflow during complex nested
 * expressions.
 * 
 * Key Operations:
 *  - push(item)   : O(1) amortized - Adds an element to the top.
 *  - pop()        : O(1) - Removes the element at the top.
 *  - top()        : O(1) - Returns a reference to the top element without removing it.
 *  - isEmpty()    : O(1) - Checks if the stack contains zero elements.
 *  - isFull()     : O(1) - Checks if current array capacity is reached (before resize).
 *  - size()       : O(1) - Returns the number of elements currently stored.
 */
template <typename T>
class Stack {
private:
    T* data;         ///< Pointer to dynamically allocated contiguous array
    int capacity;    ///< Total allocated slots in the array
    int topIndex;    ///< Index of the top element (-1 when empty)

    /**
     * @brief Doubles the array capacity when the stack becomes full.
     */
    void resize(int newCapacity);

public:
    /**
     * @brief Constructs a Stack with an initial capacity.
     * @param initialCapacity Starting size of dynamic array (default 16).
     */
    explicit Stack(int initialCapacity = 16);

    /**
     * @brief Destructor releasing dynamically allocated array memory.
     */
    ~Stack();

    /**
     * @brief Copy Constructor (Rule of Three - deep copy to avoid double-free).
     */
    Stack(const Stack<T>& other);

    /**
     * @brief Copy Assignment Operator (Rule of Three - deep copy).
     */
    Stack<T>& operator=(const Stack<T>& other);

    /**
     * @brief Pushes a new element onto the top of the stack.
     * If capacity is reached, automatically resizes array.
     */
    void push(const T& value);

    /**
     * @brief Removes the top element from the stack.
     * Throws std::underflow_error if the stack is empty.
     */
    void pop();

    /**
     * @brief Returns a reference to the top element.
     * Throws std::underflow_error if the stack is empty.
     */
    T& top();

    /**
     * @brief Returns a const reference to the top element.
     * Throws std::underflow_error if the stack is empty.
     */
    const T& top() const;

    /**
     * @brief Checks whether the stack has no elements.
     * @return true if topIndex == -1, false otherwise.
     */
    bool isEmpty() const;

    /**
     * @brief Checks whether current capacity is completely occupied.
     * @return true if size() == capacity, false otherwise.
     */
    bool isFull() const;

    /**
     * @brief Returns the number of elements in the stack.
     */
    int size() const;

    /**
     * @brief Returns current total allocated capacity.
     */
    int getCapacity() const;

    /**
     * @brief Clears all elements from the stack (resets topIndex to -1).
     */
    void clear();
};

#include "Stack.hpp"

#endif // STACK_H
