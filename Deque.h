#pragma once

#include <stdexcept>

#include "MutableListSequence.h"

template <class T>
class Deque
{
private:
    MutableListSequence<T> data_;

    void CheckNotEmpty() const
    {
        if (data_.GetLength() == 0)
        {
            throw std::out_of_range("Deque: deque is empty");
        }
    }

public:
    Deque() = default;

    void PushFront(const T &item)
    {
        data_.Prepend(item);
    }

    void PushBack(const T &item)
    {
        data_.Append(item);
    }

    T PopFront()
    {
        CheckNotEmpty();

        return data_.RemoveFirst();
    }

    //O(n) потому что односвязный список не хранит указатель на предыдущий узел
    T PopBack()
    {
        CheckNotEmpty();

        return data_.RemoveLast();
    }

    const T &PeekFront() const
    {
        CheckNotEmpty();

        return data_.GetFirst();
    }

    const T &PeekBack() const
    {
        CheckNotEmpty();

        return data_.GetLast();
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


//data_ обычный обьект, нет сырого указателя, деструктор не нужен
//Когда уничтожается Stack, автоматически уничтожается data_
//Когда уничтожается data_, автоматически уничтожается его LinkedList
//Когда уничтожается LinkedList, его деструктор удаляет узлы

//~Stack
//автоматически ~MutableListSequence
//автоматически ~ListSequence
//автоматически ~LinkedList
//Clear()
//delete узлы

