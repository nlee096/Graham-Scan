#include <iostream>
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

double getAngle(double x, double y){
    if(x == 0 && y == 0){
        return 0;
    }
    else if(x > 0){
        return atan2(y, x);
    }
    else if(x < 0 && y >= 0){
        return (atan2(y, x) + 3.14);
    }
    else if(x < 0 && y < 0){
        return (atan2(y, x) - 3.14);
    }
    return -1;
}
int cmp(const void * p1, const void * p2){
    pair<double, double> * pt1 = (pair<double, double>*)p1;
    pair<double, double> * pt2 = (pair<double, double>*)p2;

    double alpha = getAngle(pt1->first - pointlist.at(0).first, pt1->second - pointlist.at(0).second );
    double beta = getAngle(pt2->first - pointlist.at(0).first, pt2->second - pointlist.at(0).second );

    if(alpha == beta){ // colinear
        double dist1 = sqrt((double)(pow(pointlist.at(0).first - pt1->first, 2) + pow(pointlist.at(0).second - pt1->second, 2)));
        double dist2 = sqrt((double)(pow(pointlist.at(0).first - pt2->first, 2) + pow(pointlist.at(0).second - pt2->second, 2)));
        if(pointlist.at(0).first < pt1->first){
            if(dist1 < dist2){
                return -1;
            }
            else{
                return 1;
            }
        }
        else{
            if(dist1 < dist2){
                return 1;
            }
            else{
                return -1;
            }
        }
    }
    else if(alpha < beta){
        return -1;
    }
    else{
        return 1;
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
        if( pointlist.at(i).second < minY){
            minY = pointlist.at(i).second ;
            index = i;
        }
    }

    //swap minY coordinate with front of array
    if(index != 0){
        pair<double,double> temp = pointlist.at(index);
        pointlist.at(index) = pointlist.at(0);
        pointlist.at(0) = temp;
    }
    // sort based on angle
    qsort(&pointlist.at(1), numpoints-1,  sizeof(pointlist.at(0)), cmp);
    save.push_back(pointlist.at(0));
    save.push_back(pointlist.at(1));
    
    double val;
    for (int i = 2; i < numpoints; i++){
       val = (save.back().second - save.at(save.size()-2).second) * (pointlist.at(i).first - save.back().first) -
            (save.back().first - save.at(save.size()-2).first) * (pointlist.at(i).second - save.back().second);

        if(val > 0){
            for(int j = save.size()-1; j >= 0; j--){
                save.pop_back();
                val = (save.back().second - save.at(save.size()-2).second) * (pointlist.at(i).first - save.back().first) -
                    (save.back().first - save.at(save.size()-2).first) * (pointlist.at(i).second - save.back().second);
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
    cout << "Time: " << tm << endl;
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

    double minY = pointlist.at(0).second;
    int min = 0;
    for(int i = 1 ; i < numpoints; i++){
        if( pointlist.at(i).second < minY){
            minY = pointlist.at(i).second ;
            min = i;
        }
    }

    //swap minY coordinate with front of array
    if(min != 0){
        pair<double,double> temp = pointlist.at(min);
        pointlist.at(min) = pointlist.at(0);
        pointlist.at(0) = temp;
    }

    // sort based on angle
    qsort(&pointlist.at(1), numpoints-1,  sizeof(pointlist.at(0)), cmp);

    for(int i = 0; i < numpoints; i++){
        if(pointlist.at(i) != save.at(i)){
            cout << "test failed. output wrong: (";
            cout << pointlist.at(i).first << ", " << pointlist.at(i).second << ") != (" << save.at(i).first << ", " << save.at(i).second << ")" << endl;
            return;
        }
    }
    cout << "circle test 1 passed. Time: " << tm << endl;
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
    cout << "circle test 2 see allpoints.txt and convexHull.txt. Time: " << tm << endl;
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
    circleTest1(numpoints);
    circleTest2(numpoints);
    randPtTest(numpoints);
    return 0;
}
