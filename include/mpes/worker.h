/*
 * Copyright © Collin R. Hansen
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 or 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WORKER_H_
#define WORKER_H_

//https://canonical.com/mir/docs/stable/contributing/reference/cppguide/#header-file-dependencies
//Evaluate whether the ringbuffer memeber can just receive a move() unique pointer of a ringbuffer created elsewhere?
//If so, the class should be forwarded as per the style guide.
#include "mpes/ring_buffer.h"
#include <cstddef>
#include <sys/eventfd.h>
#include <unistd.h>
#include <thread>
#include <stdexcept>

namespace mpes {

template <typename T, std::size_t T_size>
class Worker
{
public:
    Worker() : event_file_descriptor{eventfd(0, EFD_SEMAPHORE | EFD_NONBLOCK)},
        running{true}
    {
        //If eventfd failed, we do not want to proceed or create a thread.
        if(this->event_file_descriptor < 0)
        {
            throw std::runtime_error("Failed to create eventfd.");
        }

        this->thread_handle = std::thread(&Worker::loop, this);
    }

    void loop()
    {
        while(this->running)
        {
            //Buffer to supply the kernel's counter parameter.
            uint64_t v_counter;

            //Kernel event driven wait pattenr.
            ssize_t v_signal = read(this->event_file_descriptor, &v_counter, sizeof(uint64_t));

            //Ensure that the read succeeded and the thread was not just woken by a signal.
            if(v_signal != sizeof(uint64_t))
            {
                continue;
            }

            //Perform all tasks in the ring buffer while awake.
            while(!this->task_queue.empty())
            {
                T v_task = this->task_queue.pop();
                v_task();
            }
        }
    }

    ~Worker()
    {
        //Set running to false.
        this->running = false;

        //If you don't write() to wake the thread up here, IT WILL HANG ON THE KERNEL!!!
        uint64_t wake_up_signal = 1;
        write(event_file_descriptor, &wake_up_signal, sizeof(uint64_t));

        //Join the main thread.
        if(this->thread_handle.joinable())
        {
            this->thread_handle.join();
        }

        //Close the file descriptor.
        if(this->event_file_descriptor >= 0)
        {
            close(this->event_file_descriptor);
        }
    }

    int event_file_descriptor;
    mpes::RingBuffer<T, T_size> task_queue;
    std::thread thread_handle;
    bool running;
};

} // namespace mpes

#endif // WORKER_H_