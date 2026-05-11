#include "mpes/ring_buffer.h"

#include <iostream>

int main(void) {
    mpes::RingBuffer<int, 10> rb;

    if(rb.push(42))
    {
        std::cout << "Pushed 42 successfully!" << std::endl;
    }

    std::cout << "count: " << rb.size() << std::endl;
    std::cout << "empty?: " << rb.empty() << std::endl;

    int val = rb.pop();

    std::cout << "Popped: " << val << std::endl;

    std::cout << "count: " << rb.size() << std::endl;
    std::cout << "empty?: " << rb.empty() << std::endl;

    std::cout << "See you" << std::endl;
    return(0);
}
