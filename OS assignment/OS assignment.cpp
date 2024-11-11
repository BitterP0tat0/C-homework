
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

std::counting_semaphore<1> printSemaphore(1);

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
const char* getTaskLength() {
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


void executeJobNoSyc(Constructor& task) {
    const char* taskType = task.getTaskType();
    int sleepTime = std::rand() % 101 + 50;

    auto start = std::chrono::system_clock::now();
    for (int i = 1; i <= task.getPages(); ++i) {

        std::cout << "User " << task.getUser() << (strcmp(taskType, "Print") == 0 ? " is printing " : " is scanning ")
            << "page " << i << " of " << task.getPages()
            << " pages. Remaining pages: " << task.getPages() - i << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }

    auto end = std::chrono::system_clock::now();

    auto duration = end - start;
    auto durationInSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    // Final output without synchronization
    std::cout << "User " << task.getUser() << "'s " << taskType << " job completed."
        << " Completion time is " << durationInSeconds << " milliseconds without Mutex Sync " << std::endl;
}

// Function to execute a job with synchronization (mutex)

void executeJobMutex(Constructor& task) {
    const char* taskType = task.getTaskType();
    int sleepTime = std::rand() % 101 + 50;  // Random sleep time for task


    auto start = std::chrono::system_clock::now();
    for (int i = 1; i <= task.getPages(); ++i) {
        {
            std::lock_guard<std::mutex> printLock(printMutex);  // Lock 
            std::cout << "User " << task.getUser() << (strcmp(taskType, "Print") == 0 ? " is printing " : " is scanning ")
                << "page " << i << " of " << task.getPages()
                << " pages. Remaining pages: " << task.getPages() - i << std::endl;
        }


        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));


    }

    auto end = std::chrono::system_clock::now();

    auto duration = end - start;
    auto durationInSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    {
        std::lock_guard<std::mutex> printLock(printMutex);
        std::cout << "User " << task.getUser() << "'s " << taskType << " job completed." << " Completion time is " << durationInSeconds << " Miliseconds with Mutex Sync " << std::endl;
    }
}

//Syc with semaphore
void executeJobSemaphore(Constructor& task) {
    const char* taskType = task.getTaskType();

    auto start = std::chrono::system_clock::now();
    for (int i = 1; i <= task.getPages(); ++i) {
        int sleepTime = std::rand() % 101 + 50;  // Random sleep time for task

        printSemaphore.acquire();  // Acquire semaphore
        std::cout << "User " << task.getUser() << (strcmp(taskType, "Print") == 0 ? " is printing " : " is scanning ")
            << "page " << i << " of " << task.getPages()
            << " pages. Remaining pages: " << task.getPages() - i << std::endl;
        printSemaphore.release();  // Release semaphore

        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }

    auto end = std::chrono::system_clock::now();
    auto duration = end - start;
    auto durationInSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    printSemaphore.acquire();  // Acquire semaphore
    std::cout << "User " << task.getUser() << "'s " << taskType << " job completed."
        << " Completion time is " << durationInSeconds << " milliseconds with Semaphore Sync" << std::endl;
    printSemaphore.release();  // Release semaphore
}

int main() {
    std::vector<Constructor> tasks;
    std::vector<std::thread> threads;
    Constructor task;
    srand(time(0));
    int arrivalTime = 0;
    int nums_Limit = 10;
    // Create tasks
    for (int j = 0; j < 10; ++j) {
        task.initializeTask(getRandomUser(), getTaskType(), getTaskLength(), getPages(getTaskLength()), getArrivalTime(arrivalTime));

        tasks.push_back(task);
    }


    for (auto& i : tasks) {
        std::cout << "User: " << i.getUser() << ", TaskType: " << i.getTaskType() << ", Pages: " << i.getPages() << std::endl;
    }

    //Switch the function in this and see different result
    for (auto& task : tasks) {
        threads.push_back(std::thread(executeJobSemaphore, std::ref(task)));
    }

    for (auto& t : threads) {
        t.join();
    }
    return 0;
}