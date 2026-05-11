#include "LinearFormTests.h"

#include <complex>
#include <iostream>
#include <stdexcept>
#include <string>

#include "LinearForm.h"
#include "MutableArraySequence.h"

class LinearFormTestFailure : public std::runtime_error
{
public:
    explicit LinearFormTestFailure(const std::string &message)
        : std::runtime_error(message) {}
};

static void AssertLinearForm(bool condition, const std::string &message)
{
    if (!condition)
    {
        throw LinearFormTestFailure(message);
    }
}

template <class ExceptionType, class Func>
static void AssertLinearFormThrows(Func func, const std::string &message)
{
    try
    {
        func();
    }
    catch (const ExceptionType &)
    {
        return;
    }

    throw LinearFormTestFailure(message);
}

static void TestLinearFormCreateAndGet()
{
    int coefficients[] = {5, 2, 3};

    LinearForm<int> form(coefficients, 3);

    AssertLinearForm(form.GetCoefficientCount() == 3, "LinearForm: coefficient count failed");
    AssertLinearForm(form.GetVariableCount() == 2, "LinearForm: variable count failed");

    AssertLinearForm(form.GetCoefficient(0) == 5, "LinearForm: coefficient a0 failed");
    AssertLinearForm(form.GetCoefficient(1) == 2, "LinearForm: coefficient a1 failed");
    AssertLinearForm(form.GetCoefficient(2) == 3, "LinearForm: coefficient a2 failed");
}

static void TestLinearFormEvaluateInt()
{
    int coefficients[] = {5, 2, 3};

    LinearForm<int> form(coefficients, 3);

    int argumentsArray[] = {10, 4};
    MutableArraySequence<int> arguments(argumentsArray, 2);

    int result = form.Evaluate(arguments);

    AssertLinearForm(result == 37, "LinearForm: int evaluate failed");
}

static void TestLinearFormAdd()
{
    int firstCoefficients[] = {5, 2, 3};
    int secondCoefficients[] = {1, 7, 4};

    LinearForm<int> first(firstCoefficients, 3);
    LinearForm<int> second(secondCoefficients, 3);

    LinearForm<int> *result = first.Add(second);

    AssertLinearForm(result->GetCoefficient(0) == 6, "LinearForm: add a0 failed");
    AssertLinearForm(result->GetCoefficient(1) == 9, "LinearForm: add a1 failed");
    AssertLinearForm(result->GetCoefficient(2) == 7, "LinearForm: add a2 failed");

    delete result;
}

static void TestLinearFormSubtract()
{
    int firstCoefficients[] = {5, 2, 3};
    int secondCoefficients[] = {1, 7, 4};

    LinearForm<int> first(firstCoefficients, 3);
    LinearForm<int> second(secondCoefficients, 3);

    LinearForm<int> *result = first.Subtract(second);

    AssertLinearForm(result->GetCoefficient(0) == 4, "LinearForm: subtract a0 failed");
    AssertLinearForm(result->GetCoefficient(1) == -5, "LinearForm: subtract a1 failed");
    AssertLinearForm(result->GetCoefficient(2) == -1, "LinearForm: subtract a2 failed");

    delete result;
}

static void TestLinearFormMultiplyByScalar()
{
    int coefficients[] = {5, 2, 3};

    LinearForm<int> form(coefficients, 3);

    LinearForm<int> *result = form.MultiplyByScalar(10);

    AssertLinearForm(result->GetCoefficient(0) == 50, "LinearForm: scalar a0 failed");
    AssertLinearForm(result->GetCoefficient(1) == 20, "LinearForm: scalar a1 failed");
    AssertLinearForm(result->GetCoefficient(2) == 30, "LinearForm: scalar a2 failed");

    delete result;
}

static void TestLinearFormDouble()
{
    double coefficients[] = {1.5, 2.0, 0.5};

    LinearForm<double> form(coefficients, 3);

    double argumentsArray[] = {2.0, 4.0};
    MutableArraySequence<double> arguments(argumentsArray, 2);

    double result = form.Evaluate(arguments);

    AssertLinearForm(result == 7.5, "LinearForm: double evaluate failed");
}

static void TestLinearFormComplex()
{
    typedef std::complex<double> Complex;

    Complex coefficients[] = {
        Complex(1.0, 1.0),
        Complex(2.0, 0.0),
        Complex(0.0, 3.0)};

    LinearForm<Complex> form(coefficients, 3);

    Complex argumentsArray[] = {
        Complex(2.0, 0.0),
        Complex(1.0, 0.0)};

    MutableArraySequence<Complex> arguments(argumentsArray, 2);

    Complex result = form.Evaluate(arguments);
    Complex expected(5.0, 4.0);

    AssertLinearForm(result == expected, "LinearForm: complex evaluate failed");
}

static void TestLinearFormListStorage()
{
    int coefficients[] = {5, 2, 3};

    LinearForm<int> form(coefficients, 3, SequenceStorageType::List);

    int argumentsArray[] = {10, 4};
    MutableArraySequence<int> arguments(argumentsArray, 2);

    int result = form.Evaluate(arguments);

    AssertLinearForm(result == 37, "LinearForm: list storage evaluate failed");
}

static void TestLinearFormCopyConstructor()
{
    int coefficients[] = {5, 2, 3};

    LinearForm<int> original(coefficients, 3);
    LinearForm<int> copy(original);

    AssertLinearForm(copy.GetCoefficient(0) == 5, "LinearForm: copy a0 failed");
    AssertLinearForm(copy.GetCoefficient(1) == 2, "LinearForm: copy a1 failed");
    AssertLinearForm(copy.GetCoefficient(2) == 3, "LinearForm: copy a2 failed");
}

static void TestLinearFormAssignment()
{
    int firstCoefficients[] = {1, 2};
    int secondCoefficients[] = {5, 6, 7};

    LinearForm<int> first(firstCoefficients, 2);
    LinearForm<int> second(secondCoefficients, 3);

    first = second;

    AssertLinearForm(first.GetCoefficientCount() == 3, "LinearForm: assignment count failed");
    AssertLinearForm(first.GetCoefficient(0) == 5, "LinearForm: assignment a0 failed");
    AssertLinearForm(first.GetCoefficient(1) == 6, "LinearForm: assignment a1 failed");
    AssertLinearForm(first.GetCoefficient(2) == 7, "LinearForm: assignment a2 failed");
}

static void TestLinearFormStorageTypeAfterCopyAndOperation()
{
    int coefficients[] = {5, 2, 3};

    LinearForm<int> original(coefficients, 3, SequenceStorageType::List);
    LinearForm<int> copy(original);

    AssertLinearForm(
        copy.GetStorageType() == SequenceStorageType::List,
        "LinearForm: storage type lost after copy constructor");

    LinearForm<int> *scaled = original.MultiplyByScalar(2);

    AssertLinearForm(
        scaled->GetStorageType() == SequenceStorageType::List,
        "LinearForm: storage type lost after operation");

    delete scaled;
}

static void TestLinearFormWrongArgumentCount()
{
    int coefficients[] = {5, 2, 3};

    LinearForm<int> form(coefficients, 3);

    int wrongArgumentsArray[] = {10};
    MutableArraySequence<int> wrongArguments(wrongArgumentsArray, 1);

    AssertLinearFormThrows<std::invalid_argument>(
        [&form, &wrongArguments]()
        {
            form.Evaluate(wrongArguments);
        },
        "LinearForm: wrong argument count must throw");
}

static void TestLinearFormDifferentDimensions()
{
    int firstCoefficients[] = {5, 2, 3};
    int secondCoefficients[] = {1, 7};

    LinearForm<int> first(firstCoefficients, 3);
    LinearForm<int> second(secondCoefficients, 2);

    AssertLinearFormThrows<std::invalid_argument>(
        [&first, &second]()
        {
            LinearForm<int> *result = first.Add(second);
            delete result;
        },
        "LinearForm: add different dimensions must throw");

    AssertLinearFormThrows<std::invalid_argument>(
        [&first, &second]()
        {
            LinearForm<int> *result = first.Subtract(second);
            delete result;
        },
        "LinearForm: subtract different dimensions must throw");
}

static void TestLinearFormInvalidCreation()
{
    AssertLinearFormThrows<std::invalid_argument>(
        []()
        {
            LinearForm<int> form(nullptr, 1);
        },
        "LinearForm: null coefficients must throw");

    int coefficients[] = {1};

    AssertLinearFormThrows<std::invalid_argument>(
        [&coefficients]()
        {
            LinearForm<int> form(coefficients, 0);
        },
        "LinearForm: zero coefficient count must throw");
}

void RunLinearFormTests()
{
    TestLinearFormCreateAndGet();
    TestLinearFormEvaluateInt();

    TestLinearFormAdd();
    TestLinearFormSubtract();
    TestLinearFormMultiplyByScalar();

    TestLinearFormDouble();
    TestLinearFormComplex();

    TestLinearFormListStorage();
    TestLinearFormCopyConstructor();
    TestLinearFormAssignment();
    TestLinearFormStorageTypeAfterCopyAndOperation();

    TestLinearFormWrongArgumentCount();
    TestLinearFormDifferentDimensions();
    TestLinearFormInvalidCreation();

    std::cout << "All LinearForm tests passed successfully.\n";
}
