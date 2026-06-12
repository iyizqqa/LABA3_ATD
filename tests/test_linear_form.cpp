#include <gtest/gtest.h>

#include <complex>

#include "../LinearForm.h"
#include "../MutableArraySequence.h"
#include "../MutableListSequence.h"
#include "../ImmutableListSequence.h"

TEST(LinearFormTests, CreatesFromArraySequence)
{
    int coefficients[] = {5, 2, 3};
    MutableArraySequence<int> sequence(coefficients, 3);

    LinearForm<int> form(sequence);

    EXPECT_EQ(form.GetCoefficientCount(), 3);
    EXPECT_EQ(form.GetVariableCount(), 2);
    EXPECT_EQ(form.GetCoefficient(0), 5);
    EXPECT_EQ(form.GetCoefficient(1), 2);
    EXPECT_EQ(form.GetCoefficient(2), 3);
}

TEST(LinearFormTests, CreatesFromListSequence)
{
    int coefficients[] = {5, 2, 3};
    ImmutableListSequence<int> sequence(coefficients, 3);

    LinearForm<int> form(sequence);

    EXPECT_EQ(form.GetCoefficientCount(), 3);
    EXPECT_EQ(form.GetVariableCount(), 2);
    EXPECT_EQ(form.GetCoefficient(0), 5);
    EXPECT_EQ(form.GetCoefficient(1), 2);
    EXPECT_EQ(form.GetCoefficient(2), 3);
}

TEST(LinearFormTests, CopiesSourceData)
{
    int coefficients[] = {5, 2, 3};
    MutableArraySequence<int> source(coefficients, 3);

    LinearForm<int> form(source);

    source.Append(100);

    EXPECT_EQ(form.GetCoefficientCount(), 3);
    EXPECT_EQ(form.GetCoefficient(0), 5);
    EXPECT_EQ(form.GetCoefficient(1), 2);
    EXPECT_EQ(form.GetCoefficient(2), 3);
}

TEST(LinearFormTests, ConstantFormHasZeroVariables)
{
    int coefficients[] = {42};

    MutableArraySequence<int> coefficientSequence(coefficients, 1);
    MutableArraySequence<int> arguments;

    LinearForm<int> form(coefficientSequence);

    EXPECT_EQ(form.GetCoefficientCount(), 1);
    EXPECT_EQ(form.GetVariableCount(), 0);
    EXPECT_EQ(form.Evaluate(arguments), 42);
}

TEST(LinearFormTests, EvaluatesInt)
{
    int coefficients[] = {5, 2, 3};
    int argumentsArray[] = {10, 4};

    MutableArraySequence<int> coefficientSequence(coefficients, 3);
    MutableArraySequence<int> arguments(argumentsArray, 2);

    LinearForm<int> form(coefficientSequence);

    EXPECT_EQ(form.Evaluate(arguments), 37);
}

TEST(LinearFormTests, EvaluatesDouble)
{
    double coefficients[] = {1.5, 2.0, 0.5};
    double argumentsArray[] = {2.0, 4.0};

    MutableArraySequence<double> coefficientSequence(coefficients, 3);
    MutableArraySequence<double> arguments(argumentsArray, 2);

    LinearForm<double> form(coefficientSequence);

    EXPECT_DOUBLE_EQ(form.Evaluate(arguments), 7.5);
}

TEST(LinearFormTests, EvaluatesComplex)
{
    using Complex = std::complex<double>;

    Complex coefficients[] = {
        Complex(1.0, 1.0),
        Complex(2.0, 0.0),
        Complex(0.0, 3.0)};

    Complex argumentsArray[] = {
        Complex(2.0, 0.0),
        Complex(1.0, 0.0)};

    MutableArraySequence<Complex> coefficientSequence(coefficients, 3);
    MutableArraySequence<Complex> arguments(argumentsArray, 2);

    LinearForm<Complex> form(coefficientSequence);

    EXPECT_EQ(form.Evaluate(arguments), Complex(5.0, 4.0));
}

TEST(LinearFormTests, AddsForms)
{
    int firstCoefficients[] = {5, 2, 3};
    int secondCoefficients[] = {1, 7, 4};

    MutableArraySequence<int> firstSequence(firstCoefficients, 3);
    MutableArraySequence<int> secondSequence(secondCoefficients, 3);

    LinearForm<int> first(firstSequence);
    LinearForm<int> second(secondSequence);

    LinearForm<int> *result = first.Add(second);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->GetCoefficient(0), 6);
    EXPECT_EQ(result->GetCoefficient(1), 9);
    EXPECT_EQ(result->GetCoefficient(2), 7);

    delete result;
}

TEST(LinearFormTests, SubtractsForms)
{
    int firstCoefficients[] = {5, 2, 3};
    int secondCoefficients[] = {1, 7, 4};

    MutableArraySequence<int> firstSequence(firstCoefficients, 3);
    MutableArraySequence<int> secondSequence(secondCoefficients, 3);

    LinearForm<int> first(firstSequence);
    LinearForm<int> second(secondSequence);

    LinearForm<int> *result = first.Subtract(second);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->GetCoefficient(0), 4);
    EXPECT_EQ(result->GetCoefficient(1), -5);
    EXPECT_EQ(result->GetCoefficient(2), -1);

    delete result;
}

TEST(LinearFormTests, MultipliesByScalar)
{
    int coefficients[] = {5, 2, 3};
    MutableArraySequence<int> sequence(coefficients, 3);

    LinearForm<int> form(sequence);

    LinearForm<int> *result = form.MultiplyByScalar(10);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->GetCoefficient(0), 50);
    EXPECT_EQ(result->GetCoefficient(1), 20);
    EXPECT_EQ(result->GetCoefficient(2), 30);

    delete result;
}

TEST(LinearFormTests, CopyConstructorCreatesIndependentCopy)
{
    int coefficients[] = {5, 2, 3};
    MutableArraySequence<int> sequence(coefficients, 3);

    LinearForm<int> original(sequence);
    LinearForm<int> copy(original);

    LinearForm<int> *changedOriginal = original.MultiplyByScalar(10);

    EXPECT_EQ(copy.GetCoefficientCount(), 3);
    EXPECT_EQ(copy.GetCoefficient(0), 5);
    EXPECT_EQ(copy.GetCoefficient(1), 2);
    EXPECT_EQ(copy.GetCoefficient(2), 3);

    delete changedOriginal;
}

TEST(LinearFormTests, AssignmentCopiesValues)
{
    int firstCoefficients[] = {1, 2};
    int secondCoefficients[] = {5, 6, 7};

    MutableArraySequence<int> firstSequence(firstCoefficients, 2);
    MutableArraySequence<int> secondSequence(secondCoefficients, 3);

    LinearForm<int> first(firstSequence);
    LinearForm<int> second(secondSequence);

    first = second;

    EXPECT_EQ(first.GetCoefficientCount(), 3);
    EXPECT_EQ(first.GetCoefficient(0), 5);
    EXPECT_EQ(first.GetCoefficient(1), 6);
    EXPECT_EQ(first.GetCoefficient(2), 7);
}

TEST(LinearFormTests, SelfAssignmentKeepsObjectValid)
{
    int coefficients[] = {5, 2, 3};
    MutableArraySequence<int> sequence(coefficients, 3);

    LinearForm<int> form(sequence);

    LinearForm<int> &sameForm = form;
    form = sameForm;

    EXPECT_EQ(form.GetCoefficientCount(), 3);
    EXPECT_EQ(form.GetCoefficient(0), 5);
    EXPECT_EQ(form.GetCoefficient(1), 2);
    EXPECT_EQ(form.GetCoefficient(2), 3);
}

TEST(LinearFormTests, CloneCreatesIndependentCopy)
{
    int coefficients[] = {5, 2, 3};
    MutableArraySequence<int> sequence(coefficients, 3);

    LinearForm<int> form(sequence);

    ILinearForm<int> *copy = form.Clone();

    ASSERT_NE(copy, nullptr);

    EXPECT_EQ(copy->GetCoefficientCount(), 3);
    EXPECT_EQ(copy->GetCoefficient(0), 5);
    EXPECT_EQ(copy->GetCoefficient(1), 2);
    EXPECT_EQ(copy->GetCoefficient(2), 3);

    LinearForm<int> *changed = form.MultiplyByScalar(10);

    EXPECT_EQ(copy->GetCoefficient(0), 5);
    EXPECT_EQ(copy->GetCoefficient(1), 2);
    EXPECT_EQ(copy->GetCoefficient(2), 3);

    delete changed;
    delete copy;
}

TEST(LinearFormTests, ThrowsOnEmptyCoefficientSequence)
{
    MutableArraySequence<int> emptySequence;

    EXPECT_THROW(
        {
            LinearForm<int> form(emptySequence);
        },
        std::invalid_argument);
}

TEST(LinearFormTests, ThrowsOnWrongArgumentCount)
{
    int coefficients[] = {5, 2, 3};
    int argumentsArray[] = {10};

    MutableArraySequence<int> coefficientSequence(coefficients, 3);
    MutableArraySequence<int> arguments(argumentsArray, 1);

    LinearForm<int> form(coefficientSequence);

    EXPECT_THROW(form.Evaluate(arguments), std::invalid_argument);
}

TEST(LinearFormTests, ThrowsOnDifferentDimensions)
{
    int firstCoefficients[] = {5, 2, 3};
    int secondCoefficients[] = {1, 7};

    MutableArraySequence<int> firstSequence(firstCoefficients, 3);
    MutableArraySequence<int> secondSequence(secondCoefficients, 2);

    LinearForm<int> first(firstSequence);
    LinearForm<int> second(secondSequence);

    EXPECT_THROW(first.Add(second), std::invalid_argument);
    EXPECT_THROW(first.Subtract(second), std::invalid_argument);
}