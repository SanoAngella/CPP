#include <iostream>
using namespace std;

int fun6(int n){
    int m =0;

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            m+=1;
        }
    }
    return m;
}


// test
int main(){
  std::cout <<  "N= 100, Number of instructions executed O(n^2): " << fun6(100) << std::endl;
  return 0;
}