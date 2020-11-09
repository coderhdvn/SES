#include <stdio.h> 
#include <string.h> //strlen 
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h> //close 
#include <iostream>
#include <vector>

using namespace std;

struct Address{
    int port;
    char * s = "192.168.0.1";
};

struct Tuple{
    Address P;
    vector<int> T;
};

struct Message{
    char s[100];
    int timestamp;
    vector<Tuple> VP;
};
Tuple newTuple(int port, int n){
    Tuple res;
    res.P.port = port;
    for (int i=0; i< n; i++){
        res.T.push_back(rand()%30);
    }
    return res;
}

void * VPtobtyes(vector<Tuple> vp, int & n, int timestamp){
    int * arr = (int *)malloc(100);
    arr[0] = vp.size();
    arr[1] = timestamp;
    int c=2;
    for (int i=0; i< vp.size(); i++){
        arr[c++]= vp.at(i).P.port;
        vector<int> v =vp.at(i).T;
        for (int j=0;j< v.size(); j++){
            arr[c++]= v.at(j);
        }   
    }
    if (vp.empty()){
        n=2;
    }else{
        n= vp.size()*(vp.at(0).T.size()+1) +2;
    }
    return arr;
}

vector<Tuple> bytestoVP(void * byte, int no_peers){
    vector<Tuple> res;
    int * arr = (int *) byte;
    int tstamp= arr[1];
    int size = arr[0];
    int c=2;
    for (int i= 0; i < size; i++){
        Tuple tuple;
        vector<int> T;
        tuple.P.port = arr[c++];
        for (int j = 0; j< no_peers ;j++){
            T.push_back(arr[c++]);
        }
        tuple.T = T;
        res.push_back(tuple);
    }
    return res;
}

void printTuple(Tuple tuple){
    printf("Process = %d\nVector= ", tuple.P.port);
    for (int i=0; i<tuple.T.size(); i++){
        printf("%d, ", tuple.T.at(i));
    }
    printf("\n");
}
