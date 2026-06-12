#pragma once

#include <stdexcept>

#include "IEnumerator.h"
#include "ImmutableArraySequence.h"
#include "Sequence.h"

template <class T>
class ILinearForm
{
public:
    virtual ~ILinearForm() = default; //виртуальный деструктор потому что обьект наследника может удаляться через указатель на базовый интерфейс

    virtual int GetCoefficientCount() const = 0;
    virtual int GetVariableCount() const = 0;
    virtual const T &GetCoefficient(int index) const = 0;

    virtual IEnumerator<T> *GetCoefficientEnumerator() const = 0; //итератор создается динамически и возвращается через базовый интерфейс, удаляет тот кто получил итератор

    //возвращаем указатель потому что создается новая линейная форма
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
    Sequence<T> *coefficients_ = nullptr; //храним коэф как посл внутри реально будет иммутаррсек, конструктор принимает любую сек
    
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

    //помощник для безопасного удаления итератора, статик тк не работает с полями обьекта, только с переданным указателем
    //ссылка на указатель - передаем сам указатель по ссылке, чтобы внутри можно былло сделать nullptr
    //удаляет обьект итератора из динамической памяти, обнуляет указатель
    static void DeleteIterator(IEnumerator<T> *&iterator)
    {
        delete iterator;
        iterator = nullptr;
    }

    //static потому что не использует текущ обьект линформ, достаточно входного сорс
    static Sequence<T> *CopyToImmutableArraySequence(const Sequence<T> &source)
    {
        int count = source.GetLength();

        if (count <= 0)
        {
            throw std::invalid_argument("LinearForm: coefficient count must be positive");
        }

        //динамический массив коэф, иммутаррсек создается из обычного массива
        T *buffer = new T[count];
        IEnumerator<T> *iterator = nullptr;

        try
        {
            iterator = source.GetEnumerator();

            int index = 0;

            //копируем все коэф из сорса в буфер
            while (iterator->HasNext())
            {
                buffer[index] = iterator->Next();
                ++index;
            }

            //GetEnumerator() вернул указатель на динам обьект
            DeleteIterator(iterator);

            //создаем иммутаррсек
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

    //операции сначала получают рез в обычном массиве но публичный конструктор формы принимает не Т* а const Sequence<T>*
    //поэтому делаем переход T* - ImmutArrSeq<T> - LinF<T>
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
        
        //создаем локальный обьект на стеке, живет до конца функции
        ImmutableArraySequence<T> sequence(items, count);

        //создаем форму 
        //конструктор LinearForm сразу копирует данные из переданной последовательности, он не сохраняет адрес sequence
        //конструктор вызовет копитуиммутарр и сделает свою внутреннюю копию
        //resultCoefficients -> local ImmutableArraySequence -> internal ImmutableArraySequence в LinearForm
        //есть дополнительное копирование, но все формы создаются через один конструктор от Sequence<T>*, внутреннее хранение всегда приводится к ImmutableArraySequence
        return new LinearForm<T>(sequence);
    }

    //меняет местаи указатели двух форм для оператора присваивания
    void Swap(LinearForm<T> &other)
    {
        Sequence<T> *temp = coefficients_;
        coefficients_ = other.coefficients_;
        other.coefficients_ = temp;
    }

public:
    //принимает любую посл коэф
    //explicit запрещает неявное преобразование Sequence<T>* в LinearForm<T>
    //без explicit компилятор мог бы где-то автоматически создать LinearForm из указателя
    explicit LinearForm(const Sequence<T> &coefficients)
    {
        coefficients_ = CopyToImmutableArraySequence(coefficients);
    }

    //копирующий конструктор, внутри есть сырой указатель Sequence<T>* coefficients_
    //Если копировать поверхностно, два объекта будут указывать на одну память
    //Это приведёт к двойному удалению
    //Поэтому нужно глубокое копирование
    LinearForm(const LinearForm<T> &other)
    {
        coefficients_ = CopyToImmutableArraySequence(*other.coefficients_);
    }

    LinearForm<T> &operator=(const LinearForm<T> &other)
    {
        if (this == &other)
        {
            return *this;
        }

        //сначала берем безопасную копию other потом меняем указатель текущий обьект с копией
        //после выхода из функции copy уничтожится и удалит старые коэф текущего обьекта
        LinearForm<T> copy(other);
        Swap(copy);

        return *this;
    }

    //удаляет внутреннюю посл коэф 
    //coefficients_ указывает на объект созданный через new ImmutableArraySequence<T>(...)
    //удаление через Sequence<T>* корректно потому что у Sequence<T> должен быть виртуальный деструктор
    //Реальный объект — ImmutableArraySequence<T>
    //Удаляем через базовый указатель Sequence<T>*
    //Это требует виртуального деструктора в базовом классе
    ~LinearForm()
    {
        delete coefficients_;
    }

    //возвращает колво коэф override переопределяет метод из ILinearForm
    int GetCoefficientCount() const override
    {
        CheckStorage();
        return coefficients_->GetLength();
    }

    //колво переменных на один меньше колва коэф
    int GetVariableCount() const override
    {
        return GetCoefficientCount() - 1;
    }

    //возвращает коэф по индексу
    const T &GetCoefficient(int index) const override
    {
        CheckStorage();
        return coefficients_->Get(index);
    }

    //возвращает итератор по коэф, удаляет тот кто вызвал, в операциях через DeleteIterator
    IEnumerator<T> *GetCoefficientEnumerator() const override
    {
        CheckStorage();
        return coefficients_->GetEnumerator();
    }

    LinearForm<T> *Add(const ILinearForm<T> &other) const override
    {
        CheckSameDimension(other);

        //создаем массив результата, временный динамический массив, размер известен во время выполнения, затем нужно удалить
        int count = GetCoefficientCount();
        T *resultCoefficients = new T[count];

        IEnumerator<T> *leftIterator = nullptr; //идет по текущей форме
        IEnumerator<T> *rightIterator = nullptr; //по other

        try
        {
            //получаем итераторы
            leftIterator = GetCoefficientEnumerator();
            rightIterator = other.GetCoefficientEnumerator();

            int index = 0;

            //идем по коэф и складываем попарно, правый итератор закончится одновременно с левым
            while (leftIterator->HasNext())
            {
                resultCoefficients[index] = leftIterator->Next() + rightIterator->Next();
                ++index;
            }

            DeleteIterator(leftIterator);
            DeleteIterator(rightIterator);

            //создаем новую линейную форму из массива коэф
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

            //а0 идет отдельно
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

    //создает копию текущей формы, вызывается копирующий конструктор, возвразает указатель на новый обьект
    //вызывающий код должен удалить
    LinearForm<T> *Clone() const override
    {
        return new LinearForm<T>(*this);
    }
};