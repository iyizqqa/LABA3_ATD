#pragma once

#include <stdexcept>

#include "MutableListSequence.h"

template <class T>
class Stack
{
private:
    MutableListSequence<T> data_;

    void CheckNotEmpty() const
    {
        if (data_.GetLength() == 0)
        {
            throw std::out_of_range("Stack: stack is empty");
        }
    }

public:
    Stack() = default;

    void Push(const T &item)
    {
        data_.Prepend(item);
    }

    T Pop()
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
