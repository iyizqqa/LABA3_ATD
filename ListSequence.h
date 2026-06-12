#pragma once

#include <functional>
#include <stdexcept>

#include "LinkedList.h"
#include "Sequence.h"

template <class T>
class ListSequence : public Sequence<T>
{
protected:
    LinkedList<T> data_; //хранится как объект, потому что ListSequence напрямую владеет списком. LinkedList не является абстрактным, поэтому указатель здесь не нужен

    //создает копию текущей лист сек
    virtual ListSequence<T> *CloneListSequence() const = 0;
    //для мут иммут либо зис для мут длм копия для иммут
    virtual ListSequence<T> *Instance() = 0;
    //пустая посл нужного типа
    virtual ListSequence<T> *NewListInstance() const = 0;

    //internal методы меняют хранилище
    //напрямую не меняют внтуренний список 
    //может вызвать только сам ListSequence либо наследники
    void AppendInternal(const T &item)
    {
        data_.Append(item);
    }

    void PrependInternal(const T &item)
    {
        data_.Prepend(item);
    }

    void InsertAtInternal(const T &item, int index)
    {
        data_.InsertAt(item, index);
    }

    T RemoveFirstInternal()
    {
        return data_.RemoveFirst();
    }

    T RemoveLastInternal()
    {
        return data_.RemoveLast();
    }

    T RemoveAtInternal(int index)
    {
        return data_.RemoveAt(index);
    }

public:
    //конструктор по умолчанию
    ListSequence() = default;

    //конструктор из обычного массива
    ListSequence(const T *items, int count)
        : data_(items, count) {}

    //конструктор из линкедлист
    //вызывает копирующий конструктор LinkedList
    //explicit нужен, чтобы конструктор с одним аргументом не использовался для неявных преобразований
    explicit ListSequence(const LinkedList<T> &list)
        : data_(list) {}

    //копирующий конструктор
    //data_(other.data_) вызывает копирующий конструктор LinkedList
    ListSequence(const ListSequence<T> &other)
        : data_(other.data_) {}

    const T &GetFirst() const override
    {
        return data_.GetFirst();
    }

    const T &GetLast() const override
    {
        return data_.GetLast();
    }

    const T &Get(int index) const override
    {
        return data_.Get(index);
    }

    int GetLength() const override
    {
        return data_.GetLength();
    }

    Sequence<T> *GetSubsequence(int startIndex, int endIndex) const override
    {
        LinkedList<T> *subList = data_.GetSubList(startIndex, endIndex);
        ListSequence<T> *result = nullptr;

        try
        {
            result = NewListInstance();
            result->data_ = *subList;//subList указатель *subList сам объект LinkedList
            //присваиваем внутреннему списку результата список subList.
            //LinkedList<T>::operator= вызывается
            delete subList;
        }
        catch (...)
        {
            delete subList;
            delete result;
            throw;
        }

        return result;
    }

    Sequence<T> *Append(const T &item) override
    {
        ListSequence<T> *result = Instance();

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
        ListSequence<T> *result = Instance();

        try
        {
            result->PrependInternal(item);
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

    Sequence<T> *InsertAt(const T &item, int index) override
    {
        ListSequence<T> *result = Instance();

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
        ListSequence<T> *result = Instance();

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
        return NewListInstance();
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