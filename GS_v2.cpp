#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <math.h>
#include <vector>
#include <utility>
#include <stack>
#include <fstream>
#include "get_time.h" 
using namespace std;

vector< pair<double, double> > pointlist;
vector< pair<double, double> > save;

bool cmp(const pair<double, double> & pt1, const pair<double, double> & pt2){
    const pair<double, double>& pt0 =  pointlist[0];
    
    double y = pt1.second - pt0.second;
    double x = pt1.first - pt0.first;
   
    double val = (y) * (pt2.first - pt1.first) - (x) * (pt2.second - pt1.second);
	 
    if(val == 0){ // colinear
	double dist1 = (double)(x*x + y*y);
        double dist2 = (double)((pt0.first - pt2.first)*(pt0.first - pt2.first) + (pt0.second - pt2.second)*(pt0.second - pt2.second));
	if(pt0.first < pt1.first){
            if(dist1 < dist2){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            if(dist1 < dist2){
                return false;
            }
            else{
                return true;
            }
        }
    }
    else if(val > 0){ 
        return false;
    }
    else{ 
        return true;   
    }
}

int convex(int numpoints)
{
    if(pointlist.empty()){
        cout << "no points" << endl;
        return -1;
    }
    else if(pointlist.size() < 3){
        cout << "not enough points" << endl;
    }
    
    //find point with smallest y value
    double minY = pointlist.at(0).second;
    int index = 0;
    for(int i = 1 ; i < numpoints; i++){
        if( pointlist[i].second < minY){
            minY = pointlist[i].second ;
            index = i;
        }
    }
    
    //swap minY coordinate with front of array
    if(index != 0){
        pair<double,double> temp = pointlist[index];
        pointlist[index] = pointlist[0];
        pointlist[0] = temp;
    }
    
    // sort based on angle
    sort(&pointlist[1], &pointlist[numpoints], cmp);
    
    save.push_back(pointlist[0]);
    save.push_back(pointlist[1]);
    
    int prev;
    double val;
    for (int i = 2; i < numpoints; i++){
	prev = save.size()-2;
        val = (save.back().second - save[prev].second) * (pointlist[i].first - save.back().first) - 
            (save.back().first - save[prev].first) * (pointlist[i].second - save.back().second);
        
        if(val > 0){
            for(int j = save.size()-1; j >= 0; j--){
                save.pop_back();
                prev = save.size()-2;
                val = (save.back().second - save[prev].second) * (pointlist[i].first - save.back().first) - 
                    (save.back().first - save[prev].first) * (pointlist[i].second - save.back().second);
                if(val <= 0){
                    break;
                }
            }
        }
        save.push_back(pointlist.at(i));
    }
    return 0;
}
void getinput(int &numpoints){
    pointlist.clear();
    save.clear();
    cin >> numpoints;
    
    double x;
    double y;
    
    for(int i = 0 ; i < numpoints; i++){
        cin >> x;
        cin >> y;
        pointlist.push_back(make_pair(x,y));
    }
    
    ofstream file;
    file.open ("allpoints.txt");
    if(file.is_open()){
        for(int i = 0; i < pointlist.size(); i++){
            file << pointlist.at(i).first << " " << pointlist.at(i).second << "; ";
        }
        file.close();
    }
    
    convex(numpoints);
    
    file.open ("convexHull.txt");
    if(file.is_open()){
        while (!save.empty()){
           file << save.back().first << " " << save.back().second <<";";
           save.pop_back();
        }
        file.close();
    }
}
void circleTest1(int &numpoints){
    pointlist.clear();
    save.clear();
    numpoints = 1000000;
    double step = 6/(double)numpoints;
    double x;
    double y;
    for(int i = 0; i < numpoints; i++){
        x = -3.0 + (step * (double)i);
        y = sqrt(9.0 - (double)pow(x, 2));
        pointlist.push_back(make_pair(x,y));
    }
    
    srand (time(NULL));
    int index;
    pair<double,double> temp;
    for(int i = 0; i < pointlist.size(); i++){
        index = rand() % numpoints; 
        temp = pointlist.at(i);
        pointlist.at(i) = pointlist.at(index);
        pointlist.at(index) = temp;
    }

    timer t; t.start(); 
    convex(numpoints);
    t.stop();
    double tm = t.get_total();
    
    for(int i = 0; i < numpoints; i++){
        if(pointlist.at(i) != save.at(i)){
            cout << "test failed. output wrong: (";
            cout << pointlist.at(i).first << ", " << pointlist.at(i).second << ") != (" << save.at(i).first << ", " << save.at(i).second << ")" << endl;
            return;
        }
    }
    cout << "circle test 1 passed. Time : " << tm << endl;
}

void circleTest2(int &numpoints){
    pointlist.clear();
    save.clear();
    numpoints = 1000000;
    double x;
    double y;
    int numadded = 0;
    
    srand (time(NULL));
    while(numadded != numpoints){
        x = ((rand() % 60000)/10000.0) - 3.0;
        y = ((rand() % 60000)/10000.0) - 3.0;
        if(pow(x,2) + pow(y,2) <= 9.0){
            pointlist.push_back(make_pair(x,y));
            numadded++;
        }
    }
    
    int index;
    pair<double,double> temp;
    for(int i = 0; i < pointlist.size(); i++){
        index = rand() % numpoints; 
        temp = pointlist.at(i);
        pointlist.at(i) = pointlist.at(index);
        pointlist.at(index) = temp;
    }
    
    ofstream file;
    file.open ("allpoints.txt");
    if(file.is_open()){
        for(int i = 0; i < pointlist.size(); i++){
            file << pointlist.at(i).first << " " << pointlist.at(i).second << "; ";
        }
        file.close();
    }
    
    timer t; t.start();
    convex(numpoints);
    t.stop();
    double tm = t.get_total();
    
    file.open ("convexHull.txt");
    if(file.is_open()){
        while (!save.empty()){
           file << save.back().first << " " << save.back().second <<";";
           save.pop_back();
        }
        file.close();
    }
    
   cout << "circle test 2 see allpoints.txt and convexHull.txt. Time : " << tm << endl;
}

void randPtTest(int &numpoints){
    pointlist.clear();
    save.clear();
    numpoints = 1000000;
    double x;
    double y;
    int sign;
    
    srand (time(NULL));
    for(int i = 0 ; i < numpoints; i++){
        x = ((rand() % 10000)/10.0);
        sign = (rand() % 1);
        if(sign == 1){
            x = -1.0 * x;
        }
        y = ((rand() % 10000)/10.0);
        sign = (rand() % 1);
        if(sign == 1){
            y = -1.0 * y;
        }
        pointlist.push_back(make_pair(x,y));
    }
    
    ofstream file;
    file.open ("allpoints.txt");
    if(file.is_open()){
        for(int i = 0; i < pointlist.size(); i++){
            file << pointlist.at(i).first << " " << pointlist.at(i).second << "; ";
        }
        file.close();
    }
    
    timer t; t.start();
    convex(numpoints);
    t.stop();
    double tm = t.get_total();
    
    file.open ("convexHull.txt");
    if(file.is_open()){
        while (!save.empty()){
           file << save.back().first << " " << save.back().second <<";";
           save.pop_back();
        }
        file.close();
    }
    cout << "random points test see allpoints.txt and convexHull.txt. Time : " << tm << endl;
}

int main(){
    int numpoints;
    
    //getinput(numpoints);
    circleTest1(numpoints);
    circleTest2(numpoints);                        
    randPtTest(numpoints);
    return 0;
}
