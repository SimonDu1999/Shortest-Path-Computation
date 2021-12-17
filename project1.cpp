//
//  project1.cpp
//  
//
//  Created by zhouheng du on 2019-11-03.
//

#include "project1.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <algorithm>

using namespace std;

double distance(int a, int b, Vertex arr[]);                                                // calculate the distance between two vertices
int DijkstraAlgorithm(int sourceV, int targetV, Vertex v[], vector<int> adjacencyList[]);   //return the number of visited vertices
int Astar(int sourceV, int targetV, Vertex v[], vector<int> adjacency[]);                   //return the number of visited vertices
int Landmark(int sourceV, int targetV, Vertex v[], vector<int> adjacency[]);                //return the number of visited vertices

int main(){
   
    srand(time(NULL));
    cout.setf(ios::fixed);
    cout.setf(ios::showpoint);
    cout.precision(10);
    Vertex vmap[1000];
    vector<int> adjacencyList[1000];
    ifstream graphFile("graph1000.txt");                                                    //read the "graph1000.txt" and collect the data
    if(!graphFile.is_open()) {
        cout << "Unable to open file graph1000.txt" << endl;
        exit(1);
    }
    int number;
    double latitude, longitude;
    char a;
    for(int i=0;i<1000;i++){
        graphFile >> number;
        graphFile >> a ;
        graphFile >> latitude;
        graphFile >> a ;
        graphFile >> longitude;
        vmap[i].set_num(number);
        vmap[i].set_x(latitude);
        vmap[i].set_y(longitude);
    }
    graphFile>>number;
    graphFile>>a;
    for(int i=0;i<999;i++){
        do{
            graphFile >> number;
            adjacencyList[i].push_back(number);
            graphFile >> a;
        }while(a!=':');
        adjacencyList[i].pop_back();
    }
    while(graphFile >> number){
        adjacencyList[999].push_back(number);
        graphFile >> a;
    }

    int totalDj=0;
    int totalA=0;
    int totalL=0;
    int sourceVertex;
    int targetVertex;
    for(int i=0;i<20;i++){
        sourceVertex=rand()%1000+1;
        targetVertex=rand()%1000+1;
        totalDj+=DijkstraAlgorithm(sourceVertex,targetVertex,vmap, adjacencyList);
        totalA+=Astar(sourceVertex,targetVertex,vmap, adjacencyList);
        totalL+=Landmark(sourceVertex,targetVertex,vmap, adjacencyList);
    }
    cout << "On average: A* algorithm saves " << (totalDj-totalA)/20 << " vertices and Landmark algorithm saves " << (totalDj-totalL)/20 << " vertices comparing to Dijkstra's algorithm." << endl;
}


int DijkstraAlgorithm(int sourceV, int targetV, Vertex v[], vector<int> adjacency[]){

    PriorityQueue p1;
    int latitude=v[sourceV-1].get_x();
    int longitude=v[sourceV-1].get_y();
    p1.insert(sourceV,latitude,longitude,0,0);                                  //insert the source vertex first
    for(int i=0;i<sourceV-1;i++){                                               //insert the rest vertices with infinite distance and key
        p1.insert(i+1,v[i].get_x(),v[i].get_y(),INFINITY,INFINITY);
    }
    for(int i=sourceV; i<1000; i++){
        p1.insert(i+1,v[i].get_x(),v[i].get_y(),INFINITY,INFINITY);
    }
    
    Vertex* tmp;
    int indexInQueue;
    double dis;
    while(!p1.isEmpty()){
        tmp=p1.extractMin();
        if(tmp->get_num() == targetV){                                          //target find
            break;
        }
        vector<int> vList=adjacency[tmp->get_num()-1];

        for(auto it=vList.begin(); it!=vList.end(); it++ ){                     //*it- each adjacency vertex
            indexInQueue=p1.vertexInHeap(*it);
            
            if(indexInQueue==0)                                                 //vertex already deleted
                continue;
            dis=p1.disInHeap(indexInQueue);
            if(dis > tmp->get_dis()+distance(*it,tmp->get_num(),v)){
                    p1.decreaseKey(*it,tmp->get_dis()+distance(*it,tmp->get_num(),v));  //new key<- new distance
            }
        }
    }
    
    cout << "[" << sourceV << ", "<< targetV <<"] has the shortest path: "<< tmp->get_dis() << " and Dijkstra's Algorithm visited "<< p1.get_visited() << " vertices."<< endl;
    return p1.get_visited();
 
}

int Astar(int sourceV, int targetV, Vertex v[], vector<int> adjacency[]){
    
    PriorityQueue p1;
    int latitude=v[sourceV-1].get_x();
    int longitude=v[sourceV-1].get_y();
    p1.insert(sourceV,latitude,longitude,0,0);
    for(int i=0;i<sourceV-1;i++){
        p1.insert(i+1,v[i].get_x(),v[i].get_y(),INFINITY,INFINITY);
    }
    for(int i=sourceV; i<1000; i++){
        p1.insert(i+1,v[i].get_x(),v[i].get_y(),INFINITY,INFINITY);
    }
    
    Vertex* tmp;
    int indexInQueue;
    double dis;
    while(!p1.isEmpty()){
        tmp=p1.extractMin();
        if(tmp->get_num() == targetV){
            break;
        }
        vector<int> vList=adjacency[tmp->get_num()-1];
        
        for(auto it=vList.begin(); it!=vList.end(); it++ ){
            indexInQueue=p1.vertexInHeap(*it);
            
            if(indexInQueue==0)
                continue;
            dis=p1.disInHeap(indexInQueue);                                                 //until here, they are all same as above
            if(dis > tmp->get_dis()+distance(*it,tmp->get_num(),v)){
                p1.decreaseKey(*it,tmp->get_dis()+distance(*it,tmp->get_num(),v),distance(*it,targetV,v));  //new key<- new distance + to_t[u]
            }
        }
    }
    
    cout << "[" << sourceV << ", "<< targetV <<"] has the shortest path: "<< tmp->get_dis() << " and A* Algorithm visited "<< p1.get_visited() << " vertices."<< endl;
    return p1.get_visited();
    
}


int Landmark(int sourceV, int targetV, Vertex v[], vector<int> adjacency[]){
    PriorityQueue p1;
    int latitude=v[sourceV-1].get_x();
    int longitude=v[sourceV-1].get_y();
    p1.insert(sourceV,latitude,longitude,0,0);
    for(int i=0;i<sourceV-1;i++){
        p1.insert(i+1,v[i].get_x(),v[i].get_y(),INFINITY,INFINITY);
    }
    for(int i=sourceV; i<1000; i++){
        p1.insert(i+1,v[i].get_x(),v[i].get_y(),INFINITY,INFINITY);
    }
    
    Vertex* tmp;
    int indexInQueue;
    double dis;
    int landmark[3];
    landmark[0]=500;
    landmark[1]=333;
    landmark[2]=666;            //picking three landmarks
    double ut;
    while(!p1.isEmpty()){
        tmp=p1.extractMin();
        if(tmp->get_num() == targetV){
            break;
        }
        vector<int> vList=adjacency[tmp->get_num()-1];
        
        for(auto it=vList.begin(); it!=vList.end(); it++ ){
            indexInQueue=p1.vertexInHeap(*it);
            
            if(indexInQueue==0)
                continue;
            dis=p1.disInHeap(indexInQueue);
            if(dis > tmp->get_dis()+distance(*it,tmp->get_num(),v)){
                ut=max(abs(distance(landmark[0],*it,v)-distance(landmark[0],targetV,v)),abs(distance(landmark[1],*it,v)-distance(landmark[1],targetV,v)));
                ut=max(ut,abs(distance(landmark[2],*it,v)-distance(landmark[2],targetV,v)));            //to_t[u]=max{|disZ[u]-dis[Z]t| for all landmark Z}
                p1.decreaseKey(*it,tmp->get_dis()+distance(*it,tmp->get_num(),v),ut);                   //new key <- new distance + to_t[u]
            }
        }
    }
    
    cout << "[" << sourceV << ", "<< targetV <<"] has the shortest path: "<< tmp->get_dis() << " and Landmark Algorithm visited "<< p1.get_visited() << " vertices."<< endl;
    return p1.get_visited();
    
}



double distance(int a, int b, Vertex arr[]){
    a=a-1;
    b=b-1;
    double pi=3.1415926535897;
    double dlat=2*pi*(arr[b].get_x()-arr[a].get_x())/360;
    double mlat=2*pi*(arr[a].get_x()+arr[b].get_x())/2/360;
    double dlon=2*pi*(arr[b].get_y()-arr[a].get_y())/360;
    return 6371009*sqrt((pow(dlat,2.0)+pow(cos(mlat)*dlon,2.0)));
}

