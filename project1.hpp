//
//  project1.hpp
//  
//
//  Created by zhouheng du on 2019-11-03.
//

#ifndef project1_hpp
#define project1_hpp

#include <cstdio>
#include <iostream>
#include <cmath>

using namespace std;

class Vertex{
public:
    Vertex();
    Vertex(int n, double a, double b, double c, double k);
    double get_x();
    void set_x(double a);
    double get_y();
    void set_y(double b);
    double get_dis();
    void set_dis(double c);
    int get_num();
    void set_num(int n);
    friend class PriorityQueue;
private:
    int num;            //vertex "id"
    double x;           //latitude
    double y;           //longitude
    double dis;         //distance
    double key;         //priority key
};


Vertex::Vertex(){
    num=0;
    x=0;
    y=0;
    dis=0;
}

Vertex::Vertex(int n, double a, double b, double c, double k){
    num=n;
    x=a;
    y=b;
    dis=c;
    key=k;
}

void Vertex::set_num(int n){
    num=n;
}
void Vertex::set_x(double a){
    x=a;
}
void Vertex::set_y(double b){
    y=b;
}
void Vertex::set_dis(double c){
    dis=c;
}
double Vertex::get_x(){
    return x;
}
double Vertex::get_y(){
    return y;
}
int Vertex::get_num(){
    return num;
}
double Vertex::get_dis(){
    return dis;
}

class PriorityQueue{
public:
    PriorityQueue();
    ~PriorityQueue();
    bool isEmpty();
    Vertex* extractMin();
    void decreaseKey(int n, double d);
    void decreaseKey(int n, double d, double k);
    void insert(int n, double la, double lo,double d,double k);
    int vertexInHeap(int n);    //return vertex[n]'s index in heap
    double disInHeap(int i);    //return the distance of PQ[i]
    void printList();
    int get_visited();          //return the number of visited vertices
private:
    void swap(int index1, int index2);
    Vertex* PQ[1001];
    int size;
    int visited;
};

PriorityQueue::PriorityQueue(){
    size=0;
    visited=1;
    for(int i=0; i< 1001; i++){
        PQ[i]=NULL;
    }
}
PriorityQueue::~PriorityQueue()
{
    for(int i=1; i< size; i++){
        PQ[i]=NULL;
        delete PQ[i];
    }
}

int PriorityQueue::get_visited(){
    return visited;
}
int PriorityQueue::vertexInHeap(int n){
    for(int i=1;i<=size;i++){
        if(PQ[i]->num == n){
            return i;
        }
    }
    return 0;
}
double PriorityQueue::disInHeap(int i){
    return PQ[i]->dis;
}
void PriorityQueue::swap(int index1, int index2){
    double d=PQ[index1]->dis;
    int number=PQ[index1]->num;
    double la=PQ[index1]->x;
    double lo=PQ[index1]->y;
    double k=PQ[index1]->key;
    Vertex* tmp= new Vertex(number,la,lo,d,k);
    for(; index1 * 2 <= size; index1 = index2)
    {
        index2 = index1 * 2;
        if (index2 != size && PQ[index2 + 1]->dis < PQ [index2]->dis)
        {
            index2++;
        }
        if (PQ[index2]->dis < tmp->dis)
        {
            PQ[index1] ->num=PQ[index2]->num;
            PQ[index1] ->x=PQ[index2]->x;
            PQ[index1] ->y=PQ[index2]->y;
            PQ[index1] ->dis=PQ[index2]->dis;
            PQ[index1] ->key=PQ[index2]->key;
        }
        else
        {
            break;
        }
    
        PQ[index2]->num = tmp->num;
        PQ[index2]->x = tmp->x;
        PQ[index2]->y = tmp->y;
        PQ[index2]->dis = tmp->dis;
        PQ[index2]->key = tmp->key;

    }
}

bool PriorityQueue::isEmpty()
{
    if (size == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void PriorityQueue::insert(int n,double la, double lo, double d, double k){
    Vertex* tmp = new Vertex(n,la,lo,d,k);
    int hole = ++size;
    for (; hole > 1 && d < PQ[hole/2]->key; hole /=2)
    {
       PQ[hole] = PQ[hole/2];
    }
    PQ[hole] = tmp;
}

Vertex* PriorityQueue::extractMin(){
    int n=PQ[1]->num;
    double la=PQ[1]->x;
    double lo=PQ[1]->y;
    double d=PQ[1]->dis;
    double k=PQ[1]->key;
    Vertex* tmp=new Vertex(n,la,lo,d,k);
    PQ[1]=PQ[size--];
    swap(1,size);
    return tmp;
}

void PriorityQueue::decreaseKey(int n, double d){
    int j = vertexInHeap(n);
    if (j==0)
        return;
    if(PQ[j]->key==INFINITY){
        visited++;
    }
    PQ[j]->dis=d;
    PQ[j]->key=d;
    while(j!=1 && PQ[j]->key < PQ[j/2]->key){
        swap(j/2,j);
        j=j/2;
    }
}

void PriorityQueue::decreaseKey(int n, double d, double k){
    int j = vertexInHeap(n);
    if (j==0)
        return;
    if(PQ[j]->key==INFINITY){
        visited++;
    }
    PQ[j]->dis=d;
    PQ[j]->key=d+k;
    while(j!=1 && PQ[j]->key < PQ[j/2]->key){       //bubble up
        swap(j/2,j);
        j=j/2;
    }
}

void PriorityQueue::printList()
{
    if (isEmpty())
    {
        cout << "empty!!" <<endl;
    }
    else
    {
        for (int i = 1; i<=size; i++)
        {
            cout << "[" << PQ[i]->num << "] " << ": " <<PQ[i]->x <<", "<< PQ[i]->y << ", "
            <<PQ[i]->dis << endl;
        }
    }
}
#endif /* project1_hpp */
