#ifndef STUDENT_H
#define STUDENT_H

#include <string>
using namespace std;

class Student {
    public :
    int code, age;
    string name;


    Student();
    Student(int code, string name, int age);

    void printDetails();
};

#endif