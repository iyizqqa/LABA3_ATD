#pragma once

#include "ArraySequence.h"

template <class T>
class MutableArraySequence final : public ArraySequence<T>
{
protected:
    ArraySequence<T> *CloneArraySequence() const override
    {
        return new MutableArraySequence<T>(*this);
    }

    ArraySequence<T> *Instance() override
    {
        return this;
    }

    ArraySequence<T> *NewArrayInstance() const override
    {
        return new MutableArraySequence<T>();
    }

public:
    MutableArraySequence() = default;

    MutableArraySequence(const T *items, int count)
        : ArraySequence<T>(items, count) {}

    explicit MutableArraySequence(const DynamicArray<T> &array)
        : ArraySequence<T>(array) {}

    MutableArraySequence(const MutableArraySequence<T> &other)
        : ArraySequence<T>(other) {}

    Sequence<T> *Clone() const override
    {
        return new MutableArraySequence<T>(*this);
    }
};