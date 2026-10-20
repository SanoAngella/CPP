#include <iostream>
#include "Student.h"

using namespace std;

int main() {

    // 1. Default constructor
    Student s1;

    s1.code = 100;
    s1.name = "John";
    s1.age = 20;

    s1.printDetails();


    // 2. Parameterized constructor
    Student s2(101, "Mary", 22);

    cout << s2.name << endl;
    s2.printDetails();


    // 3. Another Student
    Student s3(102, "Jane", 30);

    s3.printDetails();

    return 0;
}
