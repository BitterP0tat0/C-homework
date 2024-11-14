#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include <thread>
#include <cstring>
#include "Constructor.h"
#include <mutex>
#include <semaphore>
#include <algorithm>
std::counting_semaphore<1> printSemaphore(1);

std::mutex printMutex;
std::mutex taskMutex;
// Random users
const char* getRandomUser(int taskIndex) {
    const char* users[] = { "P1", "P2", "P3", "P4", "P5" };
    return users[taskIndex % 5];
}

// Set arrival time
int getArrivalTime(int arrivalTime) {
    return arrivalTime + (rand() % 10);
}

// Set the task length
const char* getTaskLength() {
    const char* lengths[] = { "Short", "Medium", "Long" };
    const char* taskLength = lengths[rand() % 3];
    return taskLength;
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
const char* getTaskName(const int pages) {

    if (pages > 5 && pages <= 15) {
        return "Medium";
    }
    if (pages > 15 && pages <= 50) {
        return "Long";
    }

    else {
        return "Short";
    }

}
void executeJobPreemptiveMutex(Constructor& task) {
    const char* taskType = task.getTaskType();
    const char* taskName = getTaskName(task.getPages());
    int pagesRemaining = task.getPages();

    while (pagesRemaining > 0) {
        std::lock_guard<std::mutex> printLock(printMutex);
        std::cout << "User " << task.getUser() << (strcmp(taskType, "Print") == 0 ? " is printing " : " is scanning ")
            << "page " << (task.getPages() - pagesRemaining + 1) << " of " << task.getPages() << " pages. Task length is: "
            << taskName << ". Remaining pages: " << pagesRemaining - 1 << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Each page takes 1 second
        pagesRemaining--; // Decrease the remaining pages

        std::this_thread::yield(); // Allow for task preemption
    }

    std::cout << "User " << task.getUser() << "'s " << taskType << " job completed." << std::endl;
}

void executeJobPreemptiveSemaphore(Constructor& task) {
    const char* taskType = task.getTaskType();
    const char* taskName = getTaskName(task.getPages());
    int pagesRemaining = task.getPages();

    while (pagesRemaining > 0) {
        printSemaphore.acquire();  // Acquire semaphore before printing or scanning

        std::cout << "User " << task.getUser() << (strcmp(taskType, "Print") == 0 ? " is printing " : " is scanning ")
            << "page " << (task.getPages() - pagesRemaining + 1) << " of " << task.getPages() << " pages. Task length is: "
            << taskName << ". Remaining pages: " << pagesRemaining - 1 << " Arrival time was " << task.getArrivalTime()<<std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Each page takes 1 second
        pagesRemaining--; // Decrease the remaining pages

        printSemaphore.release();  // Release semaphore after printing or scanning

        std::this_thread::yield(); 
    }

    std::cout << "User " << task.getUser() << "'s " << taskType << " job completed." << std::endl;
}

// Preemptive job execution with no synchronization (no mutex, no semaphore)
void executeJobNoSync(Constructor& task) {
    const char* taskType = task.getTaskType();
    const char* taskName = getTaskName(task.getPages());
    int pagesRemaining = task.getPages();

    while (pagesRemaining > 0) {
        std::cout << "User " << task.getUser() << (strcmp(taskType, "Print") == 0 ? " is printing " : " is scanning ")
            << "page " << (task.getPages() - pagesRemaining + 1) << " of " << task.getPages() << " pages. Task length is: "
            << taskName << ". Remaining pages: " << pagesRemaining - 1 << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Each page takes 1 second
        pagesRemaining--; // Decrease the remaining pages

        std::this_thread::yield(); // Allow for task preemption
    }

    std::cout << "User " << task.getUser() << "'s " << taskType << " job completed." << std::endl;
}

bool compareArrivalTime(const Constructor& a, const Constructor& b) {
    return a.getArrivalTime() < b.getArrivalTime();
}

int main() {
    std::vector<Constructor> tasks;
    std::vector<std::thread> threads;
    Constructor task;
    srand(time(0));
    int arrivalTime = 0;
    int tasksPerUser = 10;

    for (int j = 0; j < tasksPerUser; ++j) {
        for (int k = 0; k < 5; ++k) {  
            task.initializeTask(
                getRandomUser(k),
                getTaskType(),
                getTaskLength(),
                getPages(getTaskLength()),
                getArrivalTime(arrivalTime),
                getTaskName(getPages(getTaskLength()))
            );
            tasks.push_back(task);
        }
    }

    std::sort(tasks.begin(), tasks.end(), compareArrivalTime);
    for (auto& i : tasks) {
        std::cout << "User: " << i.getUser() << ", TaskType: " << i.getTaskType() << ", Pages: " << i.getPages() << " Arrival time is " << i.getArrivalTime() << std::endl;
    }

    for (auto& task : tasks) {
        threads.push_back(std::thread(executeJobNoSync, std::ref(task)));
    }
    
    for (auto& t : threads) {
        t.join();
    }

    return 0;
}