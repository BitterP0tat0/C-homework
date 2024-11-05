#include <iostream>
#include <cstring>
#include "Constructor.h"

void Constructor::initializeTask(const char* userName, int taskId,
    const char* taskName1, int taskNum1, const char* taskName2, int taskNum2, const char* taskName3, int taskNum3,
    double scanTime, double printTime, int arrivalTime) {

    strncpy_s(this->userName, userName, sizeof(this->userName) - 1);

    this->taskId = taskId;

    strncpy_s(this->taskName1, taskName1, sizeof(this->taskName1) - 1);

    this->taskNum1 = taskNum1;

    this->taskNum2 = taskNum2;

    this->taskNum3 = taskNum3;


    strncpy_s(this->taskName2, taskName2, sizeof(this->taskName2) - 1);

    strncpy_s(this->taskName3, taskName3, sizeof(this->taskName3) - 1);


    this->taskScanTime = scanTime;

    this-> taskPrintTime = printTime;

    this->arrivalTime = arrivalTime;
}

void Constructor::printTask() const {
    std::cout << "User Name: " << userName << std::endl;
    std::cout << "Task ID: " << taskId << std::endl;
    std::cout << "Task Type 1: " << taskName1 << std::endl;
    std::cout << "Num Task 1: " << taskNum1 << std::endl;

    std::cout << "Task Type 2: " << taskName2 << std::endl;
    std::cout << "Num Task 2: " << taskNum2 << std::endl;

    std::cout << "Task Type 3: " << taskName3 << std::endl;
    std::cout << "Num Task 3: " << taskNum3 << std::endl;

    std::cout << "Task Scan Time: " << taskScanTime << std::endl;
    std::cout << "Task Print Time: " << taskPrintTime << std::endl;
    std::cout << "Arrival Time: " << arrivalTime << std::endl;
}
