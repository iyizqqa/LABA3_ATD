#pragma once

#include <functional>
#include <stdexcept>

#include "IEnumerable.h"

template <class T>
class Sequence : public IEnumerable<T>
{
public:
    virtual ~Sequence() = default; //Чтобы при delete ptr корректно вызвались деструкторы наследников, деструктор базового класса должен быть виртуальным

    //методы доступа к элементам, задают базовый интерфейс
    //переопределяются в наследниках, ссылка чтобы не копировать элемент, 
    //const чтобы пользователь не мог изменить внутренний элемент посл-ти через этот метод
    virtual const T &GetFirst() const = 0;
    virtual const T &GetLast() const = 0;
    virtual const T &Get(int index) const = 0;
    virtual int GetLength() const = 0;

    //результат новая посл созданная динамически, рил тип зависит от наследника
    //методы возвращающие Sequence<T>*, создают новый обьект в динамической памяти
    //ответственность за удаление резултата лежит на вызывающем коде
    virtual Sequence<T> *GetSubsequence(int startIndex, int endIndex) const = 0;

    //возвращают Sequence<T>* чтобы удобно было работать с mutable/immutable
    //mutable меняет текущий обьект и возвращает this 
    //immutable создает новый обьект и возвращает указатель на него
    //передаем по ссылке чтобы не копировать 
    virtual Sequence<T> *Append(const T &item) = 0;
    virtual Sequence<T> *Prepend(const T &item) = 0;
    virtual Sequence<T> *InsertAt(const T &item, int index) = 0;
    virtual Sequence<T> *Concat(const Sequence<T> &other) = 0;

    //применяет функцию к каждому элементу и возвращает новую последовательность
    Sequence<T> *Map(const std::function<T(const T &)> &func) const
    {
        if (!func)
        {
            throw std::invalid_argument("Sequence: map func is null");
        }

        //создает пустую посл того же типа
        Sequence<T> *result = NewInstance();
        //создается итератор для обхода текущей посл
        IEnumerator<T> *iterator = this->GetEnumerator();

        try
        {
            while (iterator->HasNext())
            {
                Sequence<T> *updated = result->Append(func(iterator->Next()));

                //Append в mutable-реализациях возвращает тот же объект, а в immutable-реализациях создаёт новый. 
                //Сравнение позволяет корректно обработать оба случая и не потерять память.
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

    //фильтрует элементы 
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
                    //нужно чтобы работать с мут/иммут
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

    //сворачивает послед в одно знач, рез свертки одно значение
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

    //создает копию текущ посл, создается новый обьект в динамической памяти
    // а рил тип зависит от наследника
    virtual Sequence<T> *Clone() const = 0;
    //используется в мап и вере
    virtual Sequence<T> *NewInstance() const = 0;
    virtual const T &operator[](int index) const = 0;
};