#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

int capacity, nums;

struct Item{
    double value;
    double volume;
    double cp;
};

bool cmp(Item a, Item b) {
    return a.cp > b.cp;
}

struct Node{
    Node(){}
    Node(int index, int upperBound, int totalValue, int remainCapacity):
    index(index), upperBound(upperBound), totalValue(totalValue), remainCapacity(remainCapacity){}
    bool operator<(const struct Node& other)const{
        return upperBound < other.upperBound | remainCapacity < other.remainCapacity;
    }
    int index;
    int upperBound;
    int totalValue;
    int remainCapacity;
};


inline int upper(const Item items[], int start, int preValue, int remainCapacity){
    while(remainCapacity > 0 && start < nums){
        if(items[start].volume >= remainCapacity)
            return preValue + int(remainCapacity * items[start].cp);
        preValue += items[start].value;
        remainCapacity -= items[start].volume;
        start++;
    }
    return preValue;
}

inline int lower(const Item items[], int start, int preValue, int remainCapacity){
    while(remainCapacity > 0 && start < nums){
        if(items[start].volume <= remainCapacity){
            preValue += items[start].value;
            remainCapacity -= items[start].volume;
        }
        start++;
    }
    return preValue;
}

int main(){
    cin >> capacity >> nums;
    Item items[nums]; 
    for( int i = 0; i < nums; i++){
        cin >> items[i].value >> items[i].volume;
        items[i].cp = items[i].value / items[i].volume;
    }
    sort(items, items+nums, cmp);

    priority_queue<Node> pq;
    Node root(-1, upper(items, 0, 0, capacity), 0, capacity);
    pq.push(root);
    Node parent;
    int lowerBound = lower(items, 0, 0, capacity);

    while(!pq.empty()){
        parent= pq.top();
        pq.pop();
        if(parent.index == nums-1)
            continue;
        int index = parent.index + 1;
        int remain = parent.remainCapacity - items[index].volume;
        if(remain >= 0 ){
            int value = parent.totalValue + items[index].value;
            lowerBound = max(lowerBound, value);
            Node L_child(index, upper(items, index, value, remain), value, remain);
            if(L_child.upperBound > lowerBound) pq.push(L_child);
        }
        Node R_child(index, upper(items, index, parent.totalValue, parent.remainCapacity), parent.totalValue, parent.remainCapacity);
        if(R_child.upperBound > lowerBound) pq.push(R_child);
    }

    cout << lowerBound << "\n";
    return 0;
}