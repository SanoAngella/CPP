#ifndef STACK_H
#define STACK_H

#include <iostream>
using namespace std;

class Stack {
    private:
        int* data;
        int capacity;
        int topIndex;

    public:
        Stack(int size);
         ~Stack();

         void push(int value);
         void pop();

         int top();

         bool isEmpty();
         bool isFull();
};

#endif