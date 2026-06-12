#pragma once

#include <functional>
#include <stdexcept>

#include "DynamicArray.h"
#include "Sequence.h"

template <class T>
class ArraySequence : public Sequence<T>
{
protected:
    DynamicArray<T> data_; //это обьект в протектед чтобы наследникам удобно было

    //создает копию текущей аррсек
    virtual ArraySequence<T> *CloneArraySequence() const = 0;
    //мут/немут в мут инстанс возвр зис в иммут возвр клонаррсек
    virtual ArraySequence<T> *Instance() = 0;
    //создает пустую посл того же типа
    virtual ArraySequence<T> *NewArrayInstance() const = 0;

    //внутренний метод, меняет data_, нельзя напрямую иначе иммут посл можно было бы изм
    void AppendInternal(const T &item)
    {
        data_.Resize(data_.GetSize() + 1);
        data_.Set(data_.GetSize() - 1, item);
    }

    void InsertAtInternal(const T &item, int index)
    {
        if (index < 0 || index > data_.GetSize())
        {
            throw std::out_of_range("ArraySequence: insert index out of range");
        }

        int oldSize = data_.GetSize();

        data_.Resize(oldSize + 1);

        for (int i = oldSize; i > index; --i)
        {
            data_[i] = data_[i - 1];
        }

        data_[index] = item;
    }

public:
    //конструктор по умолчанию
    ArraySequence() = default;

    //конструктор из обычного массива
    //вызывает конструктор DynamicArray(const T* items, int count)
    ArraySequence(const T *items, int count)
        : data_(items, count) {}

    //создает сиквенс из существующего дайнемик арр
    //вызывает копирующтй конструктор дайнемик арр
    //explicit чтобы нельзя было случайно неявно преобразовать DynamicArray<T> в ArraySequence<T>
    explicit ArraySequence(const DynamicArray<T> &array)
        : data_(array) {}

    //кпирующий конструктор вызывает копирующий конструктор дайнемик арр
    ArraySequence(const ArraySequence<T> &other)
        : data_(other.data_) {}

    const T &GetFirst() const override
    {
        if (data_.GetSize() == 0)
        {
            throw std::out_of_range("ArraySequence: sequence is empty");
        }

        return data_.Get(0);
    }

    const T &GetLast() const override
    {
        if (data_.GetSize() == 0)
        {
            throw std::out_of_range("ArraySequence: sequence is empty");
        }

        return data_.Get(data_.GetSize() - 1);
    }

    const T &Get(int index) const override
    {
        return data_.Get(index);
    }

    int GetLength() const override
    {
        return data_.GetSize();
    }

    Sequence<T> *GetSubsequence(int startIndex, int endIndex) const override
    {
        if (startIndex < 0 ||
            endIndex < 0 ||
            startIndex >= GetLength() ||
            endIndex >= GetLength() ||
            startIndex > endIndex)
        {
            throw std::out_of_range("ArraySequence: subsequence indices out of range");
        }

        ArraySequence<T> *result = NewArrayInstance();

        try
        {
            for (int i = startIndex; i <= endIndex; ++i)
            {
                result->AppendInternal(data_.Get(i));
            }
        }
        catch (...)
        {
            delete result;
            throw;
        }

        return result;
    }

    Sequence<T> *Append(const T &item) override
    {
        ArraySequence<T> *result = Instance();

        try
        {
            result->AppendInternal(item);
        }
        catch (...)
        {
            if (result != this)
            {
                delete result;
            }
            throw;
        }

        return result;
    }

    Sequence<T> *Prepend(const T &item) override
    {
        return InsertAt(item, 0);
    }

    Sequence<T> *InsertAt(const T &item, int index) override
    {
        ArraySequence<T> *result = Instance();

        try
        {
            result->InsertAtInternal(item, index);
        }
        catch (...)
        {
            if (result != this)
            {
                delete result;
            }

            throw;
        }

        return result;
    }

    Sequence<T> *Concat(const Sequence<T> &other) override
    {
        ArraySequence<T> *result = Instance();

        const Sequence<T> *source = &other;
        Sequence<T> *snapshot = nullptr;

        if (&other == this)
        {
            snapshot = other.Clone();
            source = snapshot;
        }

        IEnumerator<T> *iterator = source->GetEnumerator();

        try
        {
            while (iterator->HasNext())
            {
                result->AppendInternal(iterator->Next());
            }
            //appenditernal потому что result уже выбран через Instance 
            //если вызвать append опять включится мут/иммут механизм и для иммут могут быть лищние копии
            delete iterator; //был создан через GetEnumerator()
            delete snapshot; //через Clone
        }
        catch (...)
        {
            delete iterator;
            delete snapshot;

            if (result != this)
            {
                delete result;
            }

            throw;
        }

        return result;
    }

    Sequence<T> *NewInstance() const override
    {
        return NewArrayInstance();
    }

    //делегируем создание итератора внутреннему DynamicArray, удаляет тт кто получил
    IEnumerator<T> *GetEnumerator() const override
    {
        return data_.GetEnumerator();
    }

    const T &operator[](int index) const override
    {
        return Get(index);
    }
};