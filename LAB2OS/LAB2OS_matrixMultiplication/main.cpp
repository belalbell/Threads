#include <time.h>
#include <iostream>
#include <mutex>
#include <cstring>
#include <fstream>

using namespace std;

void print();

void readFile();

int **initializeMatC();

void writeInOutputFile();

void joinThreads(pthread_t threads[], int noOfThreads);

clock_t startTime, endTime;
int aRow, bCol, aColBRow;
int **matAPointer, **matBPointer, **matCPointer;
int noOfRows, noOfColumns;


//number of columns of A == number of rows of B

void *computationOfEachElementOfTheOutputMatrixHappensInAThread(void *arg) {

    string s = *((string *) arg);
    int delimiterIndex = s.find(' ');
    int j = stoi(s.substr(0, delimiterIndex));
    int i = stoi(s.substr(delimiterIndex + 1, s.size()));

    for (int k = 0; k < aColBRow; k++) {//at the end of this loop we calculate one element
        matCPointer[j][i] += matAPointer[j][k] * matBPointer[k][i];
    }

}


void *computationOfEachRowOfTheOutputMatrixHappensInAThread(void *arg) {
    //args is the iMAX
    int i = *((int *) arg);
    for (int j = 0; j < bCol; j++) {//at the end of this loop we calculate one row
        for (int k = 0; k < aColBRow; k++) {//at the end of this loop we calculate one element
            matCPointer[i][j] += matAPointer[i][k] * matBPointer[k][j];
        }
    }
}


int main() {
    // read input file
    readFile();
    cout << "computation Of Each element Of The Output Matrix Happens In A Thread" << endl;
    pthread_t elementThreads[aRow * bCol];
    string indexMat[aRow * bCol];

    //save the start time
    startTime = clock();

    for (int j = 0; j < aRow; j++) {//j is c rows
        for (int i = 0; i < bCol; i++) {
            indexMat[i + (j * bCol)] = to_string(j) + " " + to_string(i);
            pthread_create(&elementThreads[i + (j * bCol)], NULL,
                           computationOfEachElementOfTheOutputMatrixHappensInAThread,
                           (void *) (&indexMat[i + (j * bCol)]));
        }
    }
    //join
    joinThreads(elementThreads, aRow * bCol);

    //save the end time
    endTime = clock();

    //write the answer to the file
    writeInOutputFile();
    //print Matrix A , Matrix B, Matrix C
    print();



    //computationOfEachRowOfTheOutputMatrixHappensInAThread
    //reinitialize matrix c to accept the new value from the second function call
    matCPointer = initializeMatC();
    cout << "computation Of Each row Of The Output Matrix Happens In A Thread" << endl;
    int numberI[aRow];
    pthread_t rowThreads[aRow];

    //save the start time
    startTime = clock();

    for (int i = 0; i < aRow; i++) {
        numberI[i] = i;
        pthread_create(&rowThreads[i], NULL, computationOfEachRowOfTheOutputMatrixHappensInAThread,
                       (void *) (&numberI[i]));
    }

    //join or wait for each thread to finish work
    joinThreads(rowThreads, aRow);

    //save the end time
    endTime = clock();

    //write the answer to the file
    writeInOutputFile();
    //print Matrix A , Matrix B, Matrix C
    print();

    return 0;
}

//print tha 3 matrices in the console
void print() {
// Displaying matA
    cout << endl
         << "Matrix A" << endl;
    for (int i = 0; i < aRow; i++) {
        for (int j = 0; j < aColBRow; j++)
            cout << matAPointer[i][j] << " ";
        cout << endl;
    }

    // Displaying matB
    cout << endl
         << "Matrix B" << endl;
    for (int i = 0; i < aColBRow; i++) {
        for (int j = 0; j < bCol; j++)
            cout << matBPointer[i][j] << " ";
        cout << endl;
    }

    // Displaying matC
    cout << endl
         << "Matrix C" << endl;

    for (int i = 0; i < aRow; i++) {
        for (int j = 0; j < bCol; j++)
            cout << matCPointer[i][j] << " ";
        cout << endl;
    }

}

//file matrixa and matrix b from input file
int **fillTheMatrix(FILE *input) {
    noOfRows, noOfColumns;
    fscanf(input, "%d", &noOfRows);
    fscanf(input, "%d", &noOfColumns);
    int **matrix = new int *[noOfRows];
    for (int rowIndex = 0; rowIndex < noOfRows; rowIndex++) {
        matrix[rowIndex] = new int[noOfColumns];
        for (int columnIndex = 0; columnIndex < noOfColumns; columnIndex++) {
            fscanf(input, "%d", &matrix[rowIndex][columnIndex]);
        }
    }
    return matrix;
}

int **initializeMatC() {
    int **matC = new int *[aRow];//[bCol]
    for (int i = 0; i < aRow; i++) {
        matC[i] = new int[bCol];
        for (int j = 0; j < bCol; j++) {
            matC[i][j] = 0;
        }
    }
    return matC;
}

//open input file and call fillTheMatrixFunction to fill matrix A and matrix B
void readFile() {
    FILE *input;
    input = fopen("input.txt", "r");
    matAPointer = fillTheMatrix(input);
    aRow = noOfRows;
    aColBRow = noOfColumns;
    matBPointer = fillTheMatrix(input);
    bCol = noOfColumns;
    matCPointer = initializeMatC();
}

//join or wait for all threads to finish work
void joinThreads(pthread_t threads[], int numberOfThreads) {
    for (int i = 0; i < numberOfThreads; i++)
        // int pthread_join(thread id, void **returnValue);
        pthread_join(threads[i], NULL);
}

//write the elapsed time and the matrix multiplication answer to output file
void writeInOutputFile() {
    ofstream logFile;
    logFile.open("output.txt", std::ofstream::out | std::ofstream::app);
    for (int i = 0; i < aRow; i++) {
        for (int j = 0; j < bCol; j++)
            logFile << matCPointer[i][j] << " ";
        logFile << endl;
    }
    logFile << endl;
    logFile << "Elapsed time = " << endTime - startTime;
    logFile << endl;
    logFile.close();
}