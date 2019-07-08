//
// Created by sara on 11/3/18.
//

#ifndef LAB2OS_MERGESORT_LOWHIGH_H
#define LAB2OS_MERGESORT_LOWHIGH_H


using namespace std;

class LowHigh {
private:
    int low;
    int mid;
    int high;

public:
    //default constructor
    LowHigh(int, int, int);

    void setLow(int low);

    int getLow();

    void setMid(int m);

    int getMid();

    void setHigh(int high);

    int getHigh();
};


#endif //LAB2OS_MERGESORT_LOWHIGH_H
