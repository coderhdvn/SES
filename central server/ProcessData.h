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
    vector<int> timestamp;
    vector<Tuple> VP;
};
Tuple newTuple(int port, int n){
    Tuple res;
    res.P.port = port;
    for (int i=0; i< n; i++){
        res.T.push_back(0);
    }
    printf("port = %d\n", port);
    return res;
}

vector<int> initTm(int n){
    vector<int> res;
    for (int i=0;i < n; i++){
        res.push_back(0);
    }
    return res;
}

vector<Message> MessageBuffer;

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

void printTimeStamp(vector<int> vt){
    for (int i=0; i<vt.size(); i++){
        printf("%d, ", vt.at(i));
    }
    printf("\n");
}

void printTuple(Tuple tuple){
    printf("Process = %d\nVector= ", tuple.P.port);
    printTimeStamp(tuple.T);
}

void deliver(Message m, vector<int> my_timestamp, int my_pos){
    for (int i=0; i< my_timestamp.size(); i++){
        my_timestamp.at(i) = max<int>(my_timestamp.at(i), m.timestamp.at(i));
    }
    my_timestamp.at(my_pos) = my_timestamp.at(my_pos) +1;
    printTimeStamp(m.timestamp);
}

void buffering(Message m){
    MessageBuffer.push_back(m);
}

void updateVP(){

}

int lessthan(vector<int> a, vector<int> b){
    int c=0;
    for (int i=0; i< a.size(); i++){
        if (a.at(i) > b.at(i)){
            return 0;
        }
        if (a.at(i) == b.at(i)){
            c++;
        }
    }
    if (c == a.size()) return 0;
    return 1;
}

int mustBeBuffered(Message m, vector<int> tm, int my_port ){
    for (int i=0; i< m.VP.size(); i++){
        if (m.VP.at(i).P.port == my_port){
            if (lessthan(tm, m.VP.at(i).T)){
                return 0;
            }
        }
    }
    return 1;
}