#pragma once

#include "ListSequence.h"

template <class T>
class MutableListSequence final : public ListSequence<T>
{
protected:
    ListSequence<T> *CloneListSequence() const override
    {
        return new MutableListSequence<T>(*this);
    }

    ListSequence<T> *Instance() override
    {
        return this;
    }

    ListSequence<T> *NewListInstance() const override
    {
        return new MutableListSequence<T>();
    }

public:
    MutableListSequence() = default;

    MutableListSequence(const T *items, int count)
        : ListSequence<T>(items, count) {}

    explicit MutableListSequence(const LinkedList<T> &list)
        : ListSequence<T>(list) {}

    MutableListSequence(const MutableListSequence<T> &other)
        : ListSequence<T>(other) {}

    Sequence<T> *Clone() const override
    {
        return new MutableListSequence<T>(*this);
    }
};