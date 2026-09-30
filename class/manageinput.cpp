#include <iostream>
using namespace std;

int main(){
    int age;
    string names, district, gender;

    cout<< "Enter your name: ";
    cin.ignore(); // to ignore the newline character left in the input buffer after reading age
    getline(cin, names);
    
    cout<< "Enter your age: ";
    cin >> age;

    cout << "Enter your district name: ";
    cin.ignore();
    getline(cin, district);

    cout << "Enter your gender: ";
    getline(cin, gender);

    cout<<names<< " you are " <<age<< " years old"<< " residing in " <<district<<" district and identified as " <<gender<<endl;

    return 0;
}