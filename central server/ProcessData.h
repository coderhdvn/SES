#include <iostream>
#include <vector>

using namespace std;

struct Address{
    int port;
};

struct Tuple{
    Address P;
    vector<int> T;
};

Tuple newTuple(int port, int n){
    Tuple res;
    res.P.port = port;
    for (int i=0; i< n; i++){
        res.T.push_back(12);
        
    }
    return res;
}

void * VPtobtyes(vector<Tuple> vp, int & n){
    int * arr = (int *)malloc(100);

    for (int i=0; i< vp.size(); i++){
        vector<int> v =vp.at(i).T;
        for (int j=0;j< v.size(); j++){
            arr[i*v.size() +j]= v.at(j);
        }   
    }
    n= vp.size()*vp.at(0).T.size();
    return arr;
}