#pragma once

#include <functional>
#include <stdexcept>

#include "IEnumerable.h"

template <class T>
class Sequence : public IEnumerable<T>
{
public:
    virtual ~Sequence() = default;

    virtual const T &GetFirst() const = 0;
    virtual const T &GetLast() const = 0;
    virtual const T &Get(int index) const = 0;
    virtual int GetLength() const = 0;

    virtual Sequence<T> *GetSubsequence(int startIndex, int endIndex) const = 0;

    virtual Sequence<T> *Append(const T &item) = 0;
    virtual Sequence<T> *Prepend(const T &item) = 0;
    virtual Sequence<T> *InsertAt(const T &item, int index) = 0;
    virtual Sequence<T> *Concat(const Sequence<T> &other) = 0;

    Sequence<T> *Map(const std::function<T(const T &)> &func) const
    {
        if (!func)
        {
            throw std::invalid_argument("Sequence: map func is null");
        }

        Sequence<T> *result = NewInstance();
        IEnumerator<T> *iterator = this->GetEnumerator();

        try
        {
            while (iterator->HasNext())
            {
                Sequence<T> *updated = result->Append(func(iterator->Next()));

                if (updated != result)
                {
                    delete result;
                    result = updated;
                }
            }

            delete iterator;
        }
        catch (...)
        {
            delete iterator;
            delete result;
            throw;
        }

        return result;
    }

    Sequence<T> *Where(const std::function<bool(const T &)> &predicate) const
    {
        if (!predicate)
        {
            throw std::invalid_argument("Sequence: where predicate is null");
        }

        Sequence<T> *result = NewInstance();
        IEnumerator<T> *iterator = this->GetEnumerator();

        try
        {
            while (iterator->HasNext())
            {
                const T &value = iterator->Next();

                if (predicate(value))
                {
                    Sequence<T> *updated = result->Append(value);

                    if (updated != result)
                    {
                        delete result;
                        result = updated;
                    }
                }
            }

            delete iterator;
        }
        catch (...)
        {
            delete iterator;
            delete result;
            throw;
        }

        return result;
    }

    T Reduce(const std::function<T(const T &, const T &)> &func, const T &initial) const
    {
        if (!func)
        {
            throw std::invalid_argument("Sequence: reduce func is null");
        }

        T accumulator = initial;
        IEnumerator<T> *iterator = this->GetEnumerator();

        try
        {
            while (iterator->HasNext())
            {
                accumulator = func(accumulator, iterator->Next());
            }

            delete iterator;
        }
        catch (...)
        {
            delete iterator;
            throw;
        }

        return accumulator;
    }

    virtual Sequence<T> *Clone() const = 0;
    virtual Sequence<T> *NewInstance() const = 0;
    virtual const T &operator[](int index) const = 0;
};