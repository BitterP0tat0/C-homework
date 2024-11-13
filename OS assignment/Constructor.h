#pragma once

#include <string>

class Constructor {
private:
    std::string user;         // User for the task
    std::string taskType;     // Type of task (Print or Scan)
    std::string taskLength;   // Length of task (Short, Medium, Long)
    std::string taskName;     // Nmae of the tasks 
    int pages;                // Number of pages for the task
    int arrivalTime;          // Arrival time for the task

public:
    Constructor();

    void initializeTask(const char* user, const char* taskType, const char* taskLength, int pages, int arrivalTime, const char* taskName);

 


    // Getter methods
    const char* getUser() const;
    const char* getTaskType() const;
    const char* getTaskName() const;
    const char* getTaskLength() const;
    int getPages() const;
    int getArrivalTime() const;

   

};
