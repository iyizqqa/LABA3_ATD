#pragma once

#include <stdexcept>

#include "MutableListSequence.h"

template <class T>
class Queue
{
private:
    MutableListSequence<T> data_;

    void CheckNotEmpty() const
    {
        if (data_.GetLength() == 0)
        {
            throw std::out_of_range("Queue: queue is empty");
        }
    }

public:
    Queue() = default;

    void Enqueue(const T &item)
    {
        data_.Append(item);
    }

    T Dequeue()
    {
        CheckNotEmpty();

        return data_.RemoveFirst();
    }

    const T &Peek() const
    {
        CheckNotEmpty();

        return data_.GetFirst();
    }

    int GetCount() const
    {
        return data_.GetLength();
    }

    bool IsEmpty() const
    {
        return data_.GetLength() == 0;
    }
};
