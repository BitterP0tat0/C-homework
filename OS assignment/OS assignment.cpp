#include <iostream>
#include "Constructor.h"  


int main() {
    Constructor test;  
    test.initializeTask("Alice", 1, "Task A", 2,"Task B",3, "Task C",5, 2.5, 1.0, 0);
    test.printTask();  

    return 0;  
}
