#include <iostream>
#include <cstring>
#include "Constructor.h"
using namespace std;

void Constructor::initializeTask(const char* userName, const char* taskType, const char* taskLength, int Page, int arrivalTime) {

    strncpy_s(this->userName, userName, sizeof(this->userName) - 1);

    strncpy_s(this->taskType, taskType, sizeof(this->taskType) - 1);

    strncpy_s(this->taskLength, taskLength, sizeof(this->taskLength) - 1);

    this->Page = Page;

    this->arrivalTime = arrivalTime;
}

void Constructor::printTask() const {
    std::cout << "UserName: " << userName << " TaskType: " << taskType << endl;
        std::cout << "TaskLength: " << taskLength << ";" << " Page: " << Page << " ArrivalTime: " << arrivalTime << "   " <<endl;
}
