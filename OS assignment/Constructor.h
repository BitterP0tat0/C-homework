#pragma once

class Constructor {
private:
    char userName[50];
    int taskId;
    char taskName1[50];
    int taskNum1;
    char taskName2[50];
    int taskNum2;
    char taskName3[50];
    int taskNum3;
    double taskScanTime;
    double taskPrintTime;
    int arrivalTime;

public:
    void initializeTask(const char* userName, int taskId,
        const char* taskName1, int taskNum1,
    const char* taskName2, int taskNum2, const char* taskName3, int taskNum3,
        double scanTime, double printTime, int arrivalTime);
    void printTask() const;  
};
