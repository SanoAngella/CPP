#include <iostream>
using namespace std;

class Animal {
    public:
    void print(){
        cout << "I am an animal" << endl;
    }
};

class Dog : public Animal {
    public:
    void print(){
        cout << "I am a dog" << endl;
    }
};

class cat :public Animal {
    public:
    void print(){
        cout << "I am a cat" << endl;
    }
};

int main(){
    
    Animal *a = new Animal();
    a->print();

    Animal *d = new Dog();
    d->print();

    Dog *d1 = new Dog();
    d1->print();

    return 0;
}