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

double distance(const Point &a, const Point &b){
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

int cmpX(const void *a, const void *b){ 
    return int(((Point *)a)->x - ((Point *)b)->x); 
}

int cmpY(const void *a, const void *b){ 
    return int(((Point *)a)->y - ((Point *)b)->y); 
}

double findMedian(int start, int end){
    if((end - start +1) % 2 == 0){
        return (point[(start+end)/2].x + point[(start+end)/2+1].x)/2;
    }else{
        return point[(start+end)/2].x;
    }
}


void closestPair(int start, int end){
    int size = end - start + 1;
    if(size < 2) return;
    if(size == 2) {
        minDistance = min(distance(point[start],point[end]), minDistance);
        return;
    }
    
    int mid = (start + end) / 2;
    double median = findMedian(start, end);

    closestPair(start, mid);
    closestPair(mid+1, end);

    int j = 0;
    for(int i=start; i<=end; i++){
        if( fabs(point[i].x - median) < minDistance){
            k[j] = point[i];
            j++;
        }
    }
    qsort(k, j, sizeof(Point), cmpY);
    for(int i=0; i<j; i++){
        for(int l=i+1; l<j&&l<i+7; l++){
            minDistance = min(distance(k[i], k[l]), minDistance);
        }
    }
    
}

int main(){
    auto start = high_resolution_clock::now(); 
    cin >> nums;
    for(int i=0; i<nums; i++){
        cin >> point[i].x >> point[i].y;
    }

    qsort(point, nums, sizeof(Point), cmpX);
    closestPair(0, nums-1);
    
    cout << (long long)minDistance << "\n";
    auto stop = high_resolution_clock::now(); 
    auto duration = duration_cast<microseconds>(stop - start); 
    cout << duration.count() << endl; 
    
    return 0;
}