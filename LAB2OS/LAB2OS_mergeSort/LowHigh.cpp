#include "LowHigh.h"

//defult constructor
LowHigh::LowHigh(int l,int m, int h) {
    low = l;
    mid=m;
    high = h;
}


void LowHigh::setLow(int h) {
    high = h;
}

int LowHigh::getLow() {
    return low;
}

void LowHigh::setMid(int m) {
    mid = m;
}

int LowHigh::getMid() {
    return mid;
}

void LowHigh::setHigh(int h) {
    high = h;
}

int LowHigh::getHigh() {
    return high;
}