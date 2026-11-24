#include <iostream>

using namespace std;
namespace myCustomNamespace {
    int value= 30;
    int square(){
        return value*value;
    }
}

int value= 21;

int main() {
    int value =20;
    // double value = 20.5; it won't work

    //:: scope resolution operator

    cout << "The local variable is " <<value<<endl;
    cout << "The global variable is " <<::value<<endl;
    cout << "The value in my custom namespace is " <<myCustomNamespace::value<<endl;
    cout << "The squared value in my custom namespace is " <<myCustomNamespace::square()<<endl;

    return 0;
}



#include <iostream>
#include <string>
#include <utility>
using namespace std;

// Define the Person class
class Person {

private:
    int id;
    string name;
    int age;

public:

    // Constructor
    Person(int personId, string personName, int personAge)
        : id(personId), name(move(personName)), age(personAge) {
    }

    // Getter methods (Accessors)
    int getId() const {
        return id;
    }

    const string& getName() const {
        return name;
    }

    int getAge() const {
        return age;
    }

    // Setter methods (Mutators)
    void setId(int newId) {
        id = newId;
    }

    void setName(string newName) {
        name = move(newName);
    }

    void setAge(int newAge) {
        age = newAge;
    }

    // Method to print Person details
    void printDetails() const {
        cout << "ID: " << id
             << ", Name: " << name
             << ", Age: " << age << endl;
    }
};

// Main function
int main() {

    // 1. Stack Allocation
    Person person1(101, "Jane", 30);

    cout << "Details of Person 1 (Stack Allocation):" << endl;

    person1.printDetails();


    // 2. Heap Allocation (Dynamic Allocation)
    Person* person2 = new Person(102, "John", 25);

    cout << "\nDetails of Person 2 (Heap Allocation):" << endl;

    person2->printDetails();


    // Release dynamically allocated memory
    delete person2;

    person2 = nullptr;

    return 0;
}
