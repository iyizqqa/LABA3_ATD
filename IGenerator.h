#pragma once

#include "Option.h"
#include "Ordinal.h"

template <class T>
class IGenerator
{
public:
    virtual ~IGenerator() = default;

    virtual T GetNext() = 0;
    virtual bool HasNext() const = 0;
    virtual Option<T> TryGetNext() = 0;

    virtual T GetByIndex(const Ordinal &index) const = 0;

    virtual int GetPosition() const = 0;
    virtual void Reset() = 0;

    virtual Ordinal GetLength() const = 0;
    virtual IGenerator<T> *Clone() const = 0;

    bool IsInfinite() const
    {
        return !GetLength().IsFinite();
    }
};