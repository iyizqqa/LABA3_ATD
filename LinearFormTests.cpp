#include "LinearFormTests.h"

#include <complex>
#include <iostream>
#include <stdexcept>
#include <string>

#include "ImmutableArraySequence.h"
#include "LinearForm.h"
#include "MutableArraySequence.h"
#include "MutableListSequence.h"

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

static void TestLinearFormCreateFromArraySequence()
{
    int coefficients[] = {5, 2, 3};
    MutableArraySequence<int> sequence(coefficients, 3);

    LinearForm<int> form(&sequence);

    AssertLinearForm(form.GetCoefficientCount() == 3, "LinearForm: coefficient count failed");
    AssertLinearForm(form.GetVariableCount() == 2, "LinearForm: variable count failed");

    AssertLinearForm(form.GetCoefficient(0) == 5, "LinearForm: coefficient a0 failed");
    AssertLinearForm(form.GetCoefficient(1) == 2, "LinearForm: coefficient a1 failed");
    AssertLinearForm(form.GetCoefficient(2) == 3, "LinearForm: coefficient a2 failed");
}

static void TestLinearFormCreateFromListSequence()
{
    int coefficients[] = {5, 2, 3};
    MutableListSequence<int> sequence(coefficients, 3);

    LinearForm<int> form(&sequence);

    AssertLinearForm(form.GetCoefficient(0) == 5, "LinearForm: list source a0 failed");
    AssertLinearForm(form.GetCoefficient(1) == 2, "LinearForm: list source a1 failed");
    AssertLinearForm(form.GetCoefficient(2) == 3, "LinearForm: list source a2 failed");
}

static void TestLinearFormCopiesSourceData()
{
    int coefficients[] = {5, 2, 3};
    MutableArraySequence<int> sequence(coefficients, 3);

    LinearForm<int> form(&sequence);

    sequence.Append(100);

    AssertLinearForm(form.GetCoefficientCount() == 3, "LinearForm: source mutation changed coefficient count");
    AssertLinearForm(form.GetCoefficient(0) == 5, "LinearForm: source mutation changed a0");
    AssertLinearForm(form.GetCoefficient(1) == 2, "LinearForm: source mutation changed a1");
    AssertLinearForm(form.GetCoefficient(2) == 3, "LinearForm: source mutation changed a2");
}

static void TestLinearFormEvaluateInt()
{
    int coefficients[] = {5, 2, 3};
    MutableArraySequence<int> coefficientSequence(coefficients, 3);

    LinearForm<int> form(&coefficientSequence);

    int argumentsArray[] = {10, 4};
    MutableArraySequence<int> arguments(argumentsArray, 2);

    int result = form.Evaluate(arguments);

    AssertLinearForm(result == 37, "LinearForm: int evaluate failed");
}

static void TestLinearFormAdd()
{
    int firstCoefficients[] = {5, 2, 3};
    int secondCoefficients[] = {1, 7, 4};

    MutableArraySequence<int> firstSequence(firstCoefficients, 3);
    MutableArraySequence<int> secondSequence(secondCoefficients, 3);

    LinearForm<int> first(&firstSequence);
    LinearForm<int> second(&secondSequence);

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

    MutableArraySequence<int> firstSequence(firstCoefficients, 3);
    MutableArraySequence<int> secondSequence(secondCoefficients, 3);

    LinearForm<int> first(&firstSequence);
    LinearForm<int> second(&secondSequence);

    LinearForm<int> *result = first.Subtract(second);

    AssertLinearForm(result->GetCoefficient(0) == 4, "LinearForm: subtract a0 failed");
    AssertLinearForm(result->GetCoefficient(1) == -5, "LinearForm: subtract a1 failed");
    AssertLinearForm(result->GetCoefficient(2) == -1, "LinearForm: subtract a2 failed");

    delete result;
}

static void TestLinearFormMultiplyByScalar()
{
    int coefficients[] = {5, 2, 3};
    MutableArraySequence<int> sequence(coefficients, 3);

    LinearForm<int> form(&sequence);

    LinearForm<int> *result = form.MultiplyByScalar(10);

    AssertLinearForm(result->GetCoefficient(0) == 50, "LinearForm: scalar a0 failed");
    AssertLinearForm(result->GetCoefficient(1) == 20, "LinearForm: scalar a1 failed");
    AssertLinearForm(result->GetCoefficient(2) == 30, "LinearForm: scalar a2 failed");

    delete result;
}

static void TestLinearFormDouble()
{
    double coefficients[] = {1.5, 2.0, 0.5};
    MutableArraySequence<double> coefficientSequence(coefficients, 3);

    LinearForm<double> form(&coefficientSequence);

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

    MutableArraySequence<Complex> coefficientSequence(coefficients, 3);

    LinearForm<Complex> form(&coefficientSequence);

    Complex argumentsArray[] = {
        Complex(2.0, 0.0),
        Complex(1.0, 0.0)};

    MutableArraySequence<Complex> arguments(argumentsArray, 2);

    Complex result = form.Evaluate(arguments);
    Complex expected(5.0, 4.0);

    AssertLinearForm(result == expected, "LinearForm: complex evaluate failed");
}

static void TestLinearFormCopyConstructor()
{
    int coefficients[] = {5, 2, 3};
    MutableArraySequence<int> sequence(coefficients, 3);

    LinearForm<int> original(&sequence);
    LinearForm<int> copy(original);

    AssertLinearForm(copy.GetCoefficient(0) == 5, "LinearForm: copy a0 failed");
    AssertLinearForm(copy.GetCoefficient(1) == 2, "LinearForm: copy a1 failed");
    AssertLinearForm(copy.GetCoefficient(2) == 3, "LinearForm: copy a2 failed");
}

static void TestLinearFormAssignment()
{
    int firstCoefficients[] = {1, 2};
    int secondCoefficients[] = {5, 6, 7};

    MutableArraySequence<int> firstSequence(firstCoefficients, 2);
    MutableArraySequence<int> secondSequence(secondCoefficients, 3);

    LinearForm<int> first(&firstSequence);
    LinearForm<int> second(&secondSequence);

    first = second;

    AssertLinearForm(first.GetCoefficientCount() == 3, "LinearForm: assignment count failed");
    AssertLinearForm(first.GetCoefficient(0) == 5, "LinearForm: assignment a0 failed");
    AssertLinearForm(first.GetCoefficient(1) == 6, "LinearForm: assignment a1 failed");
    AssertLinearForm(first.GetCoefficient(2) == 7, "LinearForm: assignment a2 failed");
}

static void TestLinearFormWrongArgumentCount()
{
    int coefficients[] = {5, 2, 3};
    MutableArraySequence<int> coefficientSequence(coefficients, 3);

    LinearForm<int> form(&coefficientSequence);

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

    MutableArraySequence<int> firstSequence(firstCoefficients, 3);
    MutableArraySequence<int> secondSequence(secondCoefficients, 2);

    LinearForm<int> first(&firstSequence);
    LinearForm<int> second(&secondSequence);

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
            LinearForm<int> form(nullptr);
        },
        "LinearForm: null sequence pointer must throw");

    MutableArraySequence<int> emptySequence;

    AssertLinearFormThrows<std::invalid_argument>(
        [&emptySequence]()
        {
            LinearForm<int> form(&emptySequence);
        },
        "LinearForm: empty sequence must throw");
}

void RunLinearFormTests()
{
    TestLinearFormCreateFromArraySequence();
    TestLinearFormCreateFromListSequence();
    TestLinearFormCopiesSourceData();

    TestLinearFormEvaluateInt();
    TestLinearFormAdd();
    TestLinearFormSubtract();
    TestLinearFormMultiplyByScalar();

    TestLinearFormDouble();
    TestLinearFormComplex();

    TestLinearFormCopyConstructor();
    TestLinearFormAssignment();

    TestLinearFormWrongArgumentCount();
    TestLinearFormDifferentDimensions();
    TestLinearFormInvalidCreation();

    std::cout << "All LinearForm tests passed successfully.\n";
}