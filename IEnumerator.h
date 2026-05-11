#pragma once

template <class T>
class IEnumerator
{
public:
    virtual ~IEnumerator() = default;

    virtual bool HasNext() const = 0; // есть ли след эл-т
    virtual const T &Next() = 0;      // возвр-т след эл-т и сдвигает итератор вперед
    virtual void Reset() = 0;         // возвр-т итератор в начало
};
