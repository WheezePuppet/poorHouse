#include<iostream>
using namespace std;
int main(){
    int p=0;
    int r=0;
    for(int i=0; i<100; i++){
        for(int k=0; k<100; k++){
            r=p/417;
            cout<<"java Model "<<i<<" "<<k<<" >> /home/rruud/work/poorHouse/marketModel/output/consumption"<<r<<".txt"<<endl;
            p++;
        }
    }
    return 0;
}
