#pragma once

#include <string>

class Constructor {
private:
    std::string user;         // User for the task
    std::string taskType;     // Type of task (Print or Scan)
    std::string taskLength;   // Length of task (Short, Medium, Long)
    int pages;                // Number of pages for the task
    int arrivalTime;          // Arrival time for the task

public:
    // Default constructor
    Constructor();

    // Method to initialize task
    void initializeTask(const char* user, const char* taskType, const char* taskLength, int pages, int arrivalTime);

    // Getter methods
    const char* getUser() const;
    const char* getTaskType() const;
    const char* getTaskLength() const;
    int getPages() const;
    int getArrivalTime() const;
};
