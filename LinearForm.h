#pragma once

#include <stdexcept>

#include "ArraySequence.h"
#include "IEnumerator.h"
#include "ListSequence.h"
#include "MutableArraySequence.h"
#include "MutableListSequence.h"
#include "Sequence.h"

enum class SequenceStorageType
{
    Array,
    List
};

template <class T>
class ILinearForm
{
public:
    virtual ~ILinearForm() = default;

    virtual SequenceStorageType GetStorageType() const = 0;

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
    SequenceStorageType storageType_ = SequenceStorageType::Array;

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

    static SequenceStorageType DetectStorageType(const Sequence<T> &sequence)
    {
        if (dynamic_cast<const ArraySequence<T> *>(&sequence) != nullptr)
        {
            return SequenceStorageType::Array;
        }

        if (dynamic_cast<const ListSequence<T> *>(&sequence) != nullptr)
        {
            return SequenceStorageType::List;
        }

        throw std::invalid_argument("LinearForm: unknown sequence storage type");
    }

    static Sequence<T> *CreateSequence(
        const T *coefficients,
        int count,
        SequenceStorageType storageType)
    {
        if (count <= 0)
        {
            throw std::invalid_argument("LinearForm: coefficient count must be positive");
        }

        if (coefficients == nullptr)
        {
            throw std::invalid_argument("LinearForm: coefficients pointer is null");
        }

        if (storageType == SequenceStorageType::Array)
        {
            return new MutableArraySequence<T>(coefficients, count);
        }

        return new MutableListSequence<T>(coefficients, count);
    }

    static void DeleteIterator(IEnumerator<T> *&iterator)
    {
        delete iterator;
        iterator = nullptr;
    }

public:
    LinearForm() //конструктор по умолчанию
    {
        T zero = T(); //создает нулевую линейную форму

        coefficients_ = new MutableArraySequence<T>(&zero, 1);
        storageType_ = SequenceStorageType::Array;
    }

    //конструктор из массива коэф
    LinearForm(
        const T *coefficients,
        int count,
        SequenceStorageType storageType = SequenceStorageType::Array)
    {
        coefficients_ = CreateSequence(coefficients, count, storageType);
        storageType_ = storageType;
    }

    //конструктор из сиквенс
    explicit LinearForm(const Sequence<T> &coefficients)
    {
        if (coefficients.GetLength() <= 0)
        {
            throw std::invalid_argument("LinearForm: coefficient count must be positive");
        }

        coefficients_ = coefficients.Clone();
        storageType_ = DetectStorageType(coefficients);
    }

    //копирующий конструктор
    LinearForm(const LinearForm<T> &other)
    {
        coefficients_ = other.coefficients_->Clone();
        storageType_ = other.storageType_;
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

    void Swap(LinearForm<T> &other)
    {
        Sequence<T> *tempCoefficients = coefficients_;
        coefficients_ = other.coefficients_;
        other.coefficients_ = tempCoefficients;

        SequenceStorageType tempStorageType = storageType_;
        storageType_ = other.storageType_;
        other.storageType_ = tempStorageType;
    }

    SequenceStorageType GetStorageType() const override
    {
        return storageType_;
    }

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

            LinearForm<T> *result = new LinearForm<T>(
                resultCoefficients,
                count,
                storageType_);

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

            LinearForm<T> *result = new LinearForm<T>(
                resultCoefficients,
                count,
                storageType_);

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

            LinearForm<T> *result = new LinearForm<T>(
                resultCoefficients,
                count,
                storageType_);

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