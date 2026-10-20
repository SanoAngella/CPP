#include "iostream"
using namespace std;

class Student{
    private:
    int code;
    string name;
    int age;

    public:
    Student(){}
    Student(int code, string name, int age){
        code=code;
        name=name;
        age=age;
    }

    void set_code(int code){
        code=code;
    }

    int get_code(){
        return code;
    }
};

int main(){
    Student s1;
    s1.set_code(101);
    cout<<s1.get_code()<<endl;

    return 0;
}