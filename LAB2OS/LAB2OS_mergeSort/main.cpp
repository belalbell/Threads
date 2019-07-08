#include <iostream>
#include <pthread.h>
#include <fstream>
#include <vector>
#include "LowHigh.h"

using namespace std;

int *readFile();

void *merge_sort(void *arg);

void merge(int low, int mid, int high);

void printSortedArray();

int arraySize = 0;
int *arrayPointer;
int currentThreadNumber = 0;
pthread_t threadID = currentThreadNumber;

int main() {
    //read the array from input file
    arrayPointer = readFile();
    //calculate the first values for the low ,mid and high to call merge_sort function
    LowHigh lowHigh(0, (arraySize - 1) / 2, arraySize - 1);
    //sort
    merge_sort(&lowHigh);

    // print sorted array
    cout << "Sorted array: ";
    printSortedArray();
    return 0;
}

// thread function for multi-threading
// the data type void * for the parameter arg means it accepts a pointer of ay type ..the same as object data type in java
void *merge_sort(void *arg) {
    LowHigh lowHigh = *(LowHigh *) arg;

    // evaluating mid point
    lowHigh.setMid(lowHigh.getLow() + (lowHigh.getHigh() - lowHigh.getLow()) / 2);

    LowHigh left(lowHigh.getLow(), lowHigh.getMid(), lowHigh.getMid()),
            right(lowHigh.getMid() + 1, lowHigh.getMid(), lowHigh.getHigh());

    if (lowHigh.getLow() < lowHigh.getHigh()) {

        //creating a thread
        currentThreadNumber++;

        //recursion left subarray
        left.setLow(lowHigh.getLow());
        left.setMid(lowHigh.getMid());
        left.setHigh(lowHigh.getMid());
        pthread_create(&threadID, NULL, merge_sort, &left);
        pthread_join(threadID, NULL);

        //recursion right subarray
        right.setLow(lowHigh.getMid() + 1);
        right.setMid(lowHigh.getMid());
        right.setHigh(lowHigh.getHigh());
        pthread_create(&threadID, NULL, merge_sort, &right);
        pthread_join(threadID, NULL);

        // merge two parts
        merge(lowHigh.getLow(), lowHigh.getMid(), lowHigh.getHigh());//low...mid...high
    }
}


// merge function for merging two parts

void merge(int low, int mid, int high) {

    int leftSubArraySize = mid - low + 1;
    int rightSubArraySize = high - mid;
    int leftSubArrayIndex, rightSubArrayIndex;

    int *leftSubArray = new int[leftSubArraySize];
    int *rightSubArray = new int[rightSubArraySize];

    // storing values in left part
    for (leftSubArrayIndex = 0; leftSubArrayIndex < leftSubArraySize; leftSubArrayIndex++)
        leftSubArray[leftSubArrayIndex] = arrayPointer[leftSubArrayIndex + low];

    // storing values in right part
    for (leftSubArrayIndex = 0; leftSubArrayIndex < rightSubArraySize; leftSubArrayIndex++)
        rightSubArray[leftSubArrayIndex] = arrayPointer[leftSubArrayIndex + mid + 1];

    int k = low;
    leftSubArrayIndex = rightSubArrayIndex = 0;

    // merge left and right in ascending order
    while (leftSubArrayIndex < leftSubArraySize && rightSubArrayIndex < rightSubArraySize) {
        if (leftSubArray[leftSubArrayIndex] <= rightSubArray[rightSubArrayIndex]) {
            arrayPointer[k] = leftSubArray[leftSubArrayIndex];
            leftSubArrayIndex++;
        } else {
            arrayPointer[k] = rightSubArray[rightSubArrayIndex];
            rightSubArrayIndex++;
        }
        k++;
    }

    // insert remaining values from left
    while (leftSubArrayIndex < leftSubArraySize) {
        arrayPointer[k] = leftSubArray[leftSubArrayIndex];
        leftSubArrayIndex++;
        k++;
    }

    // insert remaining values from right
    while (rightSubArrayIndex < rightSubArraySize) {
        arrayPointer[k] = rightSubArray[rightSubArrayIndex];
        rightSubArrayIndex++;
        k++;
    }
}

//read the unsorted array from input file
int *readFile() {
    FILE *input;
    input = fopen("input.txt", "r");
    fscanf(input, "%d", &arraySize);
    int *array = new int[arraySize];
    for (int index = 0; index < arraySize; index++) {
        fscanf(input, "%d", &array[index]);
    }
    return array;
}

void printSortedArray() {
    for (int index = 0; index < arraySize; index++) {
        cout << arrayPointer[index] << " ";
    }
}