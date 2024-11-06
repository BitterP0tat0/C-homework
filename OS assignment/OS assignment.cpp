#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "Constructor.h"
using namespace std;

// Random users
const char* getRandomUser() {
    const char* users[] = { "P1", "P2", "P3", "P4", "P5" };
    return users[rand() % 5];
}

// Set arrival time
int getArrivalTime(int arrivalTime) {
    return arrivalTime + (rand() % 10);
}

// Set the task length
const char* getRandomLength() {
    const char* lengths[] = { "Short", "Medium", "Long" };
    return lengths[rand() % 3];
}

// Set up the task type
const char* getTaskType() {
    return (rand() % 2 == 0) ? "Print" : "Scan";
}

// Generate pages based on task type
int getPage(const char* taskType) {
    if (taskType == "Short") {
        return rand() % 5 + 1;
    }
    if (taskType == "Medium") {
        return rand() % 10 + 6;
    }
    if (taskType == "Long") {
        return rand() % 35 + 16;
    }
    return 0;  
}

int main() {

    Constructor test;
    int arrivaltime = 0;

    
    for (int j = 0; j < 10; ++j) {
        test.initializeTask(getRandomUser(), getTaskType(), getRandomLength(), getPage(getRandomLength()), getArrivalTime(arrivaltime));
        test.printTask();
    }
    return 0;
}