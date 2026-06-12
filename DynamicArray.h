#pragma once

#include <stdexcept>

#include "IEnumerable.h"

template <class T>
class DynamicArray : public IEnumerable<T>
{
private:
    T *data_ = nullptr;
    int size_ = 0;
    int capacity_ = 0;

    void CheckIndex(int index) const
    {
        if (index < 0 || index >= size_)
        {
            throw std::out_of_range("DynamicArray: index out of range");
        }
    }

    //класс вложен
    class DynamicArrayEnumerator final : public IEnumerator<T>
    {
    private:
        const DynamicArray<T> &array_; //ссылка на массив который обх итератор тк он не должен копировать 
        int index_ = 0;

    public:
        //конструктор итератора, explicit запрещает неявные преобразования, делает создание обьекта явным
        //нельзя случайно превратить DynamicArray<T> в DynamicArrayEnumerator
        explicit DynamicArrayEnumerator(const DynamicArray<T> &array)
            : array_(array) {}

        bool HasNext() const override
        {
            return index_ < array_.GetSize();
        }

        const T &Next() override
        {
            if (!HasNext())
            {
                throw std::out_of_range("DynamicArrayEnumerator: no next item");
            }

            return array_.Get(index_++);
        }

        void Reset() override
        {
            index_ = 0;
        }
    };

public:
    //конструктор по умолчанию создает пустой массив
    DynamicArray() = default;

    //конструктор по размеру 
    explicit DynamicArray(int size)
    {
        if (size < 0)
        {
            throw std::invalid_argument("DynamicArray: negative size");
        }

        if (size > 0)
        {
            data_ = new T[size]();
            size_ = size;
            capacity_ = size;
        }
    }

    //конструктор из обычного массива
    DynamicArray(const T *items, int count)
    {
        if (count < 0)
        {
            throw std::invalid_argument("DynamicArray: negative count");
        }

        if (count > 0 && items == nullptr)
        {
            throw std::invalid_argument("DynamicArray: null items");
        }

        if (count > 0)
        {
            data_ = new T[count];
            size_ = count;
            capacity_ = count;

            for (int i = 0; i < count; ++i)
            {
                data_[i] = items[i];
            }
        }
    }

    //копирующий конструктор внутри есть сырой указатель
    DynamicArray(const DynamicArray<T> &other)
    {
        if (other.capacity_ > 0)
        {
            data_ = new T[other.capacity_];
            size_ = other.size_;
            capacity_ = other.capacity_;

            for (int i = 0; i < size_; ++i)
            {
                data_[i] = other.data_[i];
            }
        }
    }

    //копирующий конструктор создает новый обьект 
    //а опертатор присваивания заменяет содержимое уже существующего обьекта
    DynamicArray<T> &operator=(const DynamicArray<T> &other)
    {
        if (this == &other)
        {
            return *this;
        }

        T *newData = nullptr;

        if (other.capacity_ > 0)
        {
            newData = new T[other.capacity_];

            try
            {
                for (int i = 0; i < other.size_; ++i)
                {
                    newData[i] = other.data_[i];
                }
            }
            catch (...)
            {
                delete[] newData;
                throw;
            }
        }

        delete[] data_;

        data_ = newData;
        size_ = other.size_;
        capacity_ = other.capacity_;

        return *this;
    }

    //деструктор
    ~DynamicArray()
    {
        delete[] data_;
    }

    void EnsureCapacity(int minCapacity)
    {
        if (minCapacity < 0)
        {
            throw std::invalid_argument("DynamicArray: negative capacity");
        }

        if (minCapacity <= capacity_)
        {
            return;
        }

        int newCapacity = capacity_ == 0 ? 1 : capacity_;

        while (newCapacity < minCapacity)
        {
            newCapacity = newCapacity < 1024 ? newCapacity * 2 : newCapacity + 1024;
        }

        T *newData = new T[newCapacity]();

        try
        {
            for (int i = 0; i < size_; ++i)
            {
                newData[i] = data_[i];
            }
        }
        catch (...)
        {
            delete[] newData;
            throw;
        }

        delete[] data_;

        data_ = newData;
        capacity_ = newCapacity;
    }

    //уменьшает капасити до сайз
    void ShrinkToFit()
    {
        if (size_ == capacity_)
        {
            return;
        }

        if (size_ == 0)
        {
            delete[] data_;
            data_ = nullptr;
            capacity_ = 0;
            return;
        }

        T *newData = new T[size_];

        try
        {
            for (int i = 0; i < size_; ++i)
            {
                newData[i] = data_[i];
            }
        }
        catch (...)
        {
            delete[] newData;
            throw;
        }

        delete[] data_;

        data_ = newData;
        capacity_ = size_;
    }

    //меняет логический размер массива
    void Resize(int newSize)
    {
        if (newSize < 0)
        {
            throw std::invalid_argument("DynamicArray: negative new size");
        }

        if (newSize == 0)
        {
            delete[] data_;
            data_ = nullptr;
            size_ = 0;
            capacity_ = 0;

            return;
        }
        
        EnsureCapacity(newSize);

        for (int i = size_; i < newSize; ++i)
        {
            data_[i] = T();
        }

        size_ = newSize;
    }

    int GetSize() const
    {
        return size_;
    }

    int GetCapacity() const
    {
        return capacity_;
    }

    const T &Get(int index) const
    {
        CheckIndex(index);
        return data_[index];
    }

    void Set(int index, const T &value)
    {
        CheckIndex(index);
        data_[index] = value;
    }

    //если обьект конст
    const T &operator[](int index) const
    {
        return Get(index);
    }

    //для неконстантного обьекта
    T &operator[](int index)
    {
        CheckIndex(index);
        return data_[index];
    }

    IEnumerator<T> *GetEnumerator() const override
    {
        return new DynamicArrayEnumerator(*this);
    }
};