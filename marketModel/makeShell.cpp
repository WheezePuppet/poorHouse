#include<iostream>
using namespace std;
int main(){
    int p = 0;
    for(int i=0; i<100; i++){
        for(int k=0; k<100; k++){
            int r = p/417;
            cout<<"java Model "<<i<<" "<<k<<" >> output/consumptions"<<r<<".txt"<<endl;
            p++;
        }
        cout<<endl;
    }
    return 0;
}
