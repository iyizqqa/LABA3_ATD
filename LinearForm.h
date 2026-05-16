#pragma once

#include <stdexcept>

#include "IEnumerator.h"
#include "ImmutableArraySequence.h"
#include "Sequence.h"

template <class T>
class ILinearForm
{
public:
    virtual ~ILinearForm() = default;

    virtual int GetCoefficientCount() const = 0;
    virtual int GetVariableCount() const = 0;
    virtual const T &GetCoefficient(int index) const = 0;

    virtual IEnumerator<T> *GetCoefficientEnumerator() const = 0;

    virtual ILinearForm<T> *Add(const ILinearForm<T> &other) const = 0;
    virtual ILinearForm<T> *Subtract(const ILinearForm<T> &other) const = 0;
    virtual ILinearForm<T> *MultiplyByScalar(const T &scalar) const = 0;

    virtual T Evaluate(const Sequence<T> &arguments) const = 0;

    virtual ILinearForm<T> *Clone() const = 0;
};

template <class T>
class LinearForm final : public ILinearForm<T>
{
private:
    Sequence<T> *coefficients_ = nullptr;

    void CheckStorage() const
    {
        if (coefficients_ == nullptr)
        {
            throw std::logic_error("LinearForm: coefficients storage is null");
        }

        if (coefficients_->GetLength() == 0)
        {
            throw std::logic_error("LinearForm: coefficient count is zero");
        }
    }

    void CheckSameDimension(const ILinearForm<T> &other) const
    {
        if (GetCoefficientCount() != other.GetCoefficientCount())
        {
            throw std::invalid_argument("LinearForm: different dimensions");
        }
    }

    static void DeleteIterator(IEnumerator<T> *&iterator)
    {
        delete iterator;
        iterator = nullptr;
    }

    static Sequence<T> *CopyToImmutableArraySequence(const Sequence<T> *source)
    {
        if (source == nullptr)
        {
            throw std::invalid_argument("LinearForm: source sequence pointer is null");
        }

        int count = source->GetLength();

        if (count <= 0)
        {
            throw std::invalid_argument("LinearForm: coefficient count must be positive");
        }

        T *buffer = new T[count];
        IEnumerator<T> *iterator = nullptr;

        try
        {
            iterator = source->GetEnumerator();

            int index = 0;

            while (iterator->HasNext())
            {
                buffer[index] = iterator->Next();
                ++index;
            }

            DeleteIterator(iterator);

            Sequence<T> *result = new ImmutableArraySequence<T>(buffer, count);

            delete[] buffer;

            return result;
        }
        catch (...)
        {
            DeleteIterator(iterator);
            delete[] buffer;
            throw;
        }
    }

    LinearForm<T> *CreateResultFromArray(const T *items, int count) const
    {
        if (items == nullptr)
        {
            throw std::invalid_argument("LinearForm: result coefficients pointer is null");
        }

        if (count <= 0)
        {
            throw std::invalid_argument("LinearForm: result coefficient count must be positive");
        }

        ImmutableArraySequence<T> sequence(items, count);

        return new LinearForm<T>(&sequence);
    }

public:
    explicit LinearForm(const Sequence<T> *coefficients)
    {
        coefficients_ = CopyToImmutableArraySequence(coefficients);
    }

    LinearForm(const LinearForm<T> &other)
    {
        coefficients_ = CopyToImmutableArraySequence(other.coefficients_);
    }

    LinearForm<T> &operator=(const LinearForm<T> &other)
    {
        if (this == &other)
        {
            return *this;
        }

        LinearForm<T> copy(other);
        Swap(copy);

        return *this;
    }

    ~LinearForm()
    {
        delete coefficients_;
    }

private:
    void Swap(LinearForm<T> &other)
    {
        Sequence<T> *temp = coefficients_;
        coefficients_ = other.coefficients_;
        other.coefficients_ = temp;
    }

public:
    int GetCoefficientCount() const override
    {
        CheckStorage();
        return coefficients_->GetLength();
    }

    int GetVariableCount() const override
    {
        return GetCoefficientCount() - 1;
    }

    const T &GetCoefficient(int index) const override
    {
        CheckStorage();
        return coefficients_->Get(index);
    }

    IEnumerator<T> *GetCoefficientEnumerator() const override
    {
        CheckStorage();
        return coefficients_->GetEnumerator();
    }

    LinearForm<T> *Add(const ILinearForm<T> &other) const override
    {
        CheckSameDimension(other);

        int count = GetCoefficientCount();
        T *resultCoefficients = new T[count];

        IEnumerator<T> *leftIterator = nullptr;
        IEnumerator<T> *rightIterator = nullptr;

        try
        {
            leftIterator = GetCoefficientEnumerator();
            rightIterator = other.GetCoefficientEnumerator();

            int index = 0;

            while (leftIterator->HasNext())
            {
                resultCoefficients[index] = leftIterator->Next() + rightIterator->Next();
                ++index;
            }

            DeleteIterator(leftIterator);
            DeleteIterator(rightIterator);

            LinearForm<T> *result = CreateResultFromArray(resultCoefficients, count);

            delete[] resultCoefficients;

            return result;
        }
        catch (...)
        {
            DeleteIterator(leftIterator);
            DeleteIterator(rightIterator);
            delete[] resultCoefficients;
            throw;
        }
    }

    LinearForm<T> *Subtract(const ILinearForm<T> &other) const override
    {
        CheckSameDimension(other);

        int count = GetCoefficientCount();
        T *resultCoefficients = new T[count];

        IEnumerator<T> *leftIterator = nullptr;
        IEnumerator<T> *rightIterator = nullptr;

        try
        {
            leftIterator = GetCoefficientEnumerator();
            rightIterator = other.GetCoefficientEnumerator();

            int index = 0;

            while (leftIterator->HasNext())
            {
                resultCoefficients[index] = leftIterator->Next() - rightIterator->Next();
                ++index;
            }

            DeleteIterator(leftIterator);
            DeleteIterator(rightIterator);

            LinearForm<T> *result = CreateResultFromArray(resultCoefficients, count);

            delete[] resultCoefficients;

            return result;
        }
        catch (...)
        {
            DeleteIterator(leftIterator);
            DeleteIterator(rightIterator);
            delete[] resultCoefficients;
            throw;
        }
    }

    LinearForm<T> *MultiplyByScalar(const T &scalar) const override
    {
        int count = GetCoefficientCount();
        T *resultCoefficients = new T[count];

        IEnumerator<T> *iterator = nullptr;

        try
        {
            iterator = GetCoefficientEnumerator();

            int index = 0;

            while (iterator->HasNext())
            {
                resultCoefficients[index] = iterator->Next() * scalar;
                ++index;
            }

            DeleteIterator(iterator);

            LinearForm<T> *result = CreateResultFromArray(resultCoefficients, count);

            delete[] resultCoefficients;

            return result;
        }
        catch (...)
        {
            DeleteIterator(iterator);
            delete[] resultCoefficients;
            throw;
        }
    }

    T Evaluate(const Sequence<T> &arguments) const override
    {
        if (arguments.GetLength() != GetVariableCount())
        {
            throw std::invalid_argument("LinearForm: wrong number of arguments");
        }

        IEnumerator<T> *coefficientIterator = nullptr;
        IEnumerator<T> *argumentIterator = nullptr;

        try
        {
            coefficientIterator = GetCoefficientEnumerator();
            argumentIterator = arguments.GetEnumerator();

            T result = coefficientIterator->Next();

            while (coefficientIterator->HasNext())
            {
                result = result + coefficientIterator->Next() * argumentIterator->Next();
            }

            DeleteIterator(coefficientIterator);
            DeleteIterator(argumentIterator);

            return result;
        }
        catch (...)
        {
            DeleteIterator(coefficientIterator);
            DeleteIterator(argumentIterator);
            throw;
        }
    }

    LinearForm<T> *Clone() const override
    {
        return new LinearForm<T>(*this);
    }
};