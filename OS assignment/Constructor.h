#pragma once

class Constructor {
private:
    char userName[50];
    char taskType[50];
    char taskLength[50];
    int Page;
    int arrivalTime;

public:
    void initializeTask(const char* userName, const char* taskType, const char* taskLength, int Page, int arrivalTime);

    void printTask() const;
};
