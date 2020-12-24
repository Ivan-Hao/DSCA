#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cmath>
#include <chrono> 
using namespace std::chrono; 
using namespace std;

int nums;
double minDistance = std::numeric_limits<double>::max();

struct Point{
    Point(){}
    Point(double x, double y): x(x), y(y){}
    bool operator<(const struct Point& other)const{
        return y > other.y;
    }
    double x;
    double y;
};
Point point[200000];
Point k[200000];
double xCoord[200000];


double distance(const Point &a, const Point &b){
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

int cmpY(const void *a, const void *b){ 
    return int(((Point *)a)->y - ((Point *)b)->y); 
}

double findMedian(int n){
    if (n % 2 == 0) { 
        nth_element(xCoord, xCoord + n / 2, xCoord + n); 
        nth_element(xCoord, xCoord + (n - 1) / 2, xCoord + n); 
        return (xCoord[(n - 1) / 2] + xCoord[n / 2]) / 2.0; 
    
    }else { 
        nth_element(xCoord, xCoord + n / 2, xCoord + n); 
        return xCoord[n / 2]; 
    } 
}


void closestPair(vector<Point> p, int size){
    if(size < 2) return;
    if(size == 2) {
        minDistance = min(distance(p[0],p[1]), minDistance);
        return;
    }

    for(int i=0; i<size; i++){
        xCoord[i] = p[i].x;
    }
    double median = findMedian(size);

    vector<Point> l;
    vector<Point> r;

    for(int i=0; i<size; i++){
        if( p[i].x < median )
            l.push_back(p[i]);
        else if(p[i].x > median)
            r.push_back(p[i]);
        else{
            if(l.size() > r.size())
                r.push_back(p[i]);
            else
                l.push_back(p[i]);
        }
    }

    closestPair(l, l.size());
    closestPair(r, r.size());

    int j = 0;
    for(int i=0; i<size; i++){
        if( fabs(p[i].x - median) < minDistance){
            k[j] = p[i];
            j++;
        }
    }
    for(int i=0; i<j; i++){
        for(int l=i+1; l<j&&l<i+7; l++){
            minDistance = min(distance(k[i], k[l]), minDistance);
        }
    }
    
}

int main(){
    cin >> nums;
    for(int i=0; i<nums; i++){
        cin >> point[i].x >> point[i].y;
    }
    qsort(point, nums, sizeof(Point), cmpY);
    vector<Point> p(point,point+nums); 
    closestPair(p, nums);
    cout << (long long)minDistance << "\n";
    
    return 0;
}