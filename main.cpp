#include "mpes/worker.h"

#include <iostream>
#include <chrono>

typedef void (*Task)();

void hello_task() {
    std::cout << "Worker is processing the task!" << std::endl;
}

int main(void) {
    try
    {
        //Create the worker (Task type, Buffer size)
        mpes::Worker<Task, 10> worker;
        std::cout << "Worker started..." << std::endl;

        //Push a task into the queue
        worker.push_task(hello_task);

        //Wait for thread.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "See you" << std::endl;
    return(0);
}
