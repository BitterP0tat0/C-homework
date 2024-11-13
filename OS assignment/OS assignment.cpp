
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

void executeJobNoSyc(Constructor& task) {
    const char* taskType = task.getTaskType();
    int sleepTime = 2;
    const char* taskName = getTaskName(task.getPages());

    auto start = std::chrono::system_clock::now();
    for (int i = 1; i <= task.getPages(); ++i) {

        std::cout << "User " << task.getUser() << (strcmp(taskType, "Print") == 0 ? " is printing " : " is scanning ")
            << "page " << i << " of " << task.getPages() << "pages. Task length is: " << taskName
            << ". Remaining pages: " << task.getPages() - i << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

    }

    auto end = std::chrono::system_clock::now();

    auto duration = end - start;
    auto durationInSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    std::cout << "User " << task.getUser() << "'s " << taskType << " job completed."
        << " Completion time is " << durationInSeconds << " seconds without Mutex Sync " << std::endl;
}

// Function to execute a job with synchronization (mutex)

void executeJobMutex(Constructor& task) {
    const char* taskType = task.getTaskType();
    int sleepTime = 2;  

    const char* taskName = getTaskName(task.getPages());
    auto start = std::chrono::system_clock::now();
    for (int i = 1; i <= task.getPages(); ++i) {
        {
            std::lock_guard<std::mutex> printLock(printMutex);  // Lock 
            std::cout << "User " << task.getUser() << (strcmp(taskType, "Print") == 0 ? " is printing " : " is scanning ")
                << "page " << i << " of " << task.getPages() << " pages.  Task length is: " << taskName
                << " . Remaining pages: " << task.getPages() - i << std::endl;

            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

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

// Syc with semaphore
void executeJobSemaphore(Constructor& task) {
    const char* taskType = task.getTaskType();

    auto start = std::chrono::system_clock::now();
    for (int i = 1; i <= task.getPages(); ++i) {
        int sleepTime = 2;  // Random sleep time for task
        const char* taskName = getTaskName(task.getPages());

        printSemaphore.acquire();  // Acquire semaphore
        std::cout << "User " << task.getUser() << (strcmp(taskType, "Print") == 0 ? " is printing " : " is scanning ")
            << "page " << i << " of " << task.getPages() << " pages. Task length is: " << taskName
            << " . Remaining pages: " << task.getPages() - i << std::endl;
        printSemaphore.release();  // Release semaphore

        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }

    auto end = std::chrono::system_clock::now();
    auto duration = end - start;
    auto durationInSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    printSemaphore.acquire();  // Acquire semaphore
    std::cout << "User " << task.getUser() << "'s " << taskType << " job completed."
        << " Completion time is " << durationInSeconds << " seconds with Semaphore Sync" << std::endl;
    printSemaphore.release();  // Release semaphore
}

int main() {
    std::vector<Constructor> tasks;
    std::vector<std::thread> threads;
    Constructor task;
    srand(time(0));
    int arrivalTime = 0;
    int nums_Limit = 10;
    int tasksPerUser = 10;

    // Create 10 tasks for each user
    for (int j = 0; j < tasksPerUser; ++j) {
        for (int k = 0; k < 5; ++k) {  // Assuming 5 unique users
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


    for (auto& i : tasks) {
        std::cout << "User: " << i.getUser() << ", TaskType: " << i.getTaskType() << ", Pages: " << i.getPages() << std::endl;
    }

   //Switch the function 
   
    for (auto& task : tasks) {
        threads.push_back(std::thread(executeJobMutex, std::ref(task)));
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}