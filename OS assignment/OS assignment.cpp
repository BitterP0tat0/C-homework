#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include <thread>
#include <cstring>
#include "Constructor.h"
#include <mutex>

// Declare a global mutex to synchronize console output and task execution
std::mutex printMutex;
std::mutex taskMutex;  

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

// Pages based on task length
int getPages(const char* taskLength) {
    if (strcmp(taskLength, "Short") == 0) {
        return rand() % 5 + 1;  // 1 to 5 pages
    }
    if (strcmp(taskLength, "Medium") == 0) {
        return rand() % 10 + 6;  // 6 to 15 pages
    }
    if (strcmp(taskLength, "Long") == 0) {
        return rand() % 35 + 16;  // 16 to 50 pages
    }
    return 0;
}

// Function to execute a job with synchronization (mutex)
void executeJob(Constructor task) {
    const char* taskType = task.getTaskType();
    int pages = task.getPages();
    int sleepTime = std::rand() % 101 + 50;

    // Record the start time of the task
    auto start = std::chrono::high_resolution_clock::now();

    // Lock the mutex to ensure exclusive access to the task execution
    std::lock_guard<std::mutex> taskLock(taskMutex);  // Rename the lock variable

    for (int i = 1; i <= pages; ++i) {
        // Lock the mutex before printing the task details
        std::lock_guard<std::mutex> printLock(printMutex);  // Rename this lock variable as well

        if (strcmp(taskType, "Print") == 0) {
            std::cout << "User " << task.getUser() << " is printing page " << i
                << " of " << pages << " pages." << std::endl;
        }
        else {
            std::cout << "User " << task.getUser() << " is scanning page " << i
                << " of " << pages << " pages." << std::endl;
        }

        // Simulate task processing by sleeping
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
      
    }

    // Lock the mutex for the final message
    std::lock_guard<std::mutex> printLockFinal(printMutex);  // Another renamed lock variable
    std::cout << "User " << task.getUser() << "'s " << taskType << " job completed." << std::endl;

    // Record the end time of the task
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration of the task
    std::chrono::duration<double> duration = end - start;

    // Output the completion time of the task
    std::cout << "User " << task.getUser() << "'s " << taskType << " job took "
        << duration.count() << " seconds to complete." << std::endl;
}

int main() {
    std::vector<Constructor> tasks;
    std::vector<std::thread> threads;
    int arrivalTime = 0;

    // Create tasks
    for (int j = 0; j < 10; ++j) {
        Constructor task;
        task.initializeTask(getRandomUser(), getTaskType(), getRandomLength(), getPages(getTaskType()), getArrivalTime(arrivalTime));
        tasks.push_back(task);
    }

    // Launch threads to execute tasks
    for (auto& task : tasks) {
        threads.push_back(std::thread(executeJob, std::ref(task)));
    }

    // Wait for all threads to complete
    for (auto& t : threads) {
        t.join();
    }

    return 0;
}