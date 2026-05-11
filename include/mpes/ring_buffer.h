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

#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <cstddef>

namespace mpes
{

template <typename T, std::size_t T_size>
class RingBuffer
{
public:
    RingBuffer()
    {
        //...
    }

    bool push(T p_item)
    {
        if(this->count == T_size)
        {
            //Buffer is full, cannot push().
            return(false);
        }

        this->buffer[this->head] = p_item;

        this->head = (this->head + 1) % T_size;
        this->count++;

        return(true);
    }

    T pop()
    {
        if(this->count == 0)
        {
            //Buffer is empty, nothing to pop().
            return T();
        }

        T v_item = buffer[this->tail];

        this->tail = (this->tail + 1) % T_size;
        this->count--;

        return(v_item);
    }

    bool empty() const
    {
        return(this->count == 0);
    }

    bool full() const
    {
        return(this->count == T_size);
    }

    std::size_t size() const
    {
        return(this->count);
    }

    ~RingBuffer()
    {
        //...
    }

private:
    T buffer[T_size];
    std::size_t head = 0;
    std::size_t tail = 0;
    std::size_t count = 0;
};

} // namespace mpes

#endif // RING_BUFFER_H_