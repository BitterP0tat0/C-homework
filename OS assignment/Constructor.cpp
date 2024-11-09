#include "Constructor.h"

// Default constructor
Constructor::Constructor() : user(""), taskType(""), taskLength(""), pages(0), arrivalTime(0) {}

// initialize task
void Constructor::initializeTask(const char* user, const char* taskType, const char* taskLength, int pages, int arrivalTime) {
    this->user = user;
    this->taskType = taskType;
    this->taskLength = taskLength;
    this->pages = pages;
    this->arrivalTime = arrivalTime;
}

// Getter methods
const char* Constructor::getUser() const {
    return user.c_str();
}

const char* Constructor::getTaskType() const {
    return taskType.c_str();
}

const char* Constructor::getTaskLength() const {
    return taskLength.c_str();
}

int Constructor::getPages() const {
    return pages;
}

int Constructor::getArrivalTime() const {
    return arrivalTime;
}
