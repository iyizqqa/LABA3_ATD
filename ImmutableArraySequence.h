#pragma once

#include "ArraySequence.h"

template <class T>
class ImmutableArraySequence final : public ArraySequence<T>
{
protected:
    ArraySequence<T> *CloneArraySequence() const override
    {
        return new ImmutableArraySequence<T>(*this);
    }

    ArraySequence<T> *Instance() override
    {
        return CloneArraySequence();
    }

    ArraySequence<T> *NewArrayInstance() const override
    {
        return new ImmutableArraySequence<T>();
    }

public:
    ImmutableArraySequence() = default;

    ImmutableArraySequence(const T *items, int count)
        : ArraySequence<T>(items, count) {}

    explicit ImmutableArraySequence(const DynamicArray<T> &array)
        : ArraySequence<T>(array) {}

    ImmutableArraySequence(const ImmutableArraySequence<T> &other)
        : ArraySequence<T>(other) {}

    Sequence<T> *Clone() const override
    {
        return new ImmutableArraySequence<T>(*this);
    }
};
