#include <iostream>
using namespace std;

class Student {
    public:
    int code;
    string name;
    int age;

    Student(int code, string name, int age) {
        this->code = code;
        this->name = name;
        this->age = age;
    }

     Student(){}

    void printDetails() {
        cout << "Student Code: " << code << endl;
        cout << "Student Name: " << name << endl;
        cout << "Student Age: " << age << endl;
    }
};

int main() {

    // 1. Normal object
    // call the default constructor
    Student s1;
    Student s1(101, "Mary", 20);

    cout << s1.name << endl;       // Using .
    s1.printDetails();             // Using function


    // 2. Another normal object
    Student s2(102, "Jane", 30);
    s2.printDetails();             // Using function


    // 3. Pointer object
    Student* s3 = new Student(103, "Peter", 25);

    cout << s3->code << endl;      // Using ->
    cout << s3->name << endl;
    cout << s3->age << endl;

    s3->printDetails();            // Pointer calling function

    delete s3;

    return 0;
}

// #include <iostream>
// using namespace std;

// struct Student {
//     int code;
//     string name;
//     int age;

//     Student() {}

//     Student(int cod, string nm, int ag) {
//         code = cod;
//         name = nm;
//         age = ag;
//     }

//     void print() {
//         cout << code << " " << name << " " << age << endl;
//     }
// };

// int main() {
//     Student s1;
//     s1.code = 101;
//     s1.name = "Mary";
//     s1.age = 15;
//     s1.print();

//     Student s2(105, "John", 18);
//     s2.print();

//     Student s3{108, "Mugisha", 17};
//     s3.print();

//     Student *s4 = new Student(100, "Mike", 16);
//     s4->print();

//     return 0;
// }