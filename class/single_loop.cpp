#include <iostream>
using namespace std;

int fun1(int n){
    int m =0;

    for(int i=0; i<n; i++){
        m+=1;
    }
    return m;
}

// test codes
int main(){
  std::cout <<  "N= 100, Number of instructions executed O(n): " << fun1(100) << std::endl;
  return 0;
}