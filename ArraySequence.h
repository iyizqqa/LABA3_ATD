#pragma once

#include <functional>
#include <stdexcept>

#include "DynamicArray.h"
#include "Sequence.h"

template <class T>
class ArraySequence : public Sequence<T>
{
protected:
    DynamicArray<T> data_;

    virtual ArraySequence<T> *CloneArraySequence() const = 0;
    virtual ArraySequence<T> *Instance() = 0;
    virtual ArraySequence<T> *NewArrayInstance() const = 0;

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
    ArraySequence() = default;

    ArraySequence(const T *items, int count)
        : data_(items, count) {}

    explicit ArraySequence(const DynamicArray<T> &array)
        : data_(array) {}

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

            delete iterator;
            delete snapshot;
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

    IEnumerator<T> *GetEnumerator() const override
    {
        return data_.GetEnumerator();
    }

    const T &operator[](int index) const override
    {
        return Get(index);
    }
};