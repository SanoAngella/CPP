#include <iostream>
using namespace std;

int fun5(int n){
    int i, j, m=0;

    for(i=1; i<=n; i*=2){
        for(j=1; j<=n; j*=2){
            m+=1;
        }
        return m;
    }

}

// test
int main(){
    cout << "N= 100, Number of instructions::"<< fun5(100) << " O(n) " << endl;
    return 0;
}