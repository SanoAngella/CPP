#include "Student.h"
#include <iostream>

using namespace std;

Student::Student() {
    code = 0;
    name = "";
    age = 0;
}

Student::Student(int code, string name, int age) {
    this->code = code;
    this->name = name;
    this->age = age;
}

void Student::printDetails() {
    cout << "Student Code: " << code << endl;
    cout << "Student Name: " << name << endl;
    cout << "Student Age: " << age << endl;
}