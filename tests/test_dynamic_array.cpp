#include <gtest/gtest.h>

#include "../DynamicArray.h"
#include "../IEnumerator.h"

TEST(DynamicArrayTests, DefaultConstructorCreatesEmptyArray)
{
    DynamicArray<int> array;

    EXPECT_EQ(array.GetSize(), 0);
    EXPECT_EQ(array.GetCapacity(), 0);
}

TEST(DynamicArrayTests, SizeConstructorCreatesArrayWithDefaultValues)
{
    DynamicArray<int> array(3);

    EXPECT_EQ(array.GetSize(), 3);
    EXPECT_GE(array.GetCapacity(), 3);

    EXPECT_EQ(array.Get(0), 0);
    EXPECT_EQ(array.Get(1), 0);
    EXPECT_EQ(array.Get(2), 0);
}

TEST(DynamicArrayTests, ConstructorFromItemsCopiesValues)
{
    int items[] = {1, 2, 3};

    DynamicArray<int> array(items, 3);

    EXPECT_EQ(array.GetSize(), 3);
    EXPECT_EQ(array.GetCapacity(), 3);

    EXPECT_EQ(array.Get(0), 1);
    EXPECT_EQ(array.Get(1), 2);
    EXPECT_EQ(array.Get(2), 3);
}

TEST(DynamicArrayTests, CopyConstructorCreatesIndependentCopy)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> original(items, 3);

    DynamicArray<int> copy(original);

    original.Set(0, 100);

    EXPECT_EQ(copy.GetSize(), 3);
    EXPECT_EQ(copy.Get(0), 1);
    EXPECT_EQ(copy.Get(1), 2);
    EXPECT_EQ(copy.Get(2), 3);

    EXPECT_EQ(original.Get(0), 100);
}

TEST(DynamicArrayTests, AssignmentCreatesIndependentCopy)
{
    int firstItems[] = {1, 2, 3};
    int secondItems[] = {10, 20};

    DynamicArray<int> first(firstItems, 3);
    DynamicArray<int> second(secondItems, 2);

    first = second;

    second.Set(0, 100);

    EXPECT_EQ(first.GetSize(), 2);
    EXPECT_EQ(first.Get(0), 10);
    EXPECT_EQ(first.Get(1), 20);

    EXPECT_EQ(second.Get(0), 100);
}

TEST(DynamicArrayTests, SelfAssignmentKeepsArrayValid)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 3);

    DynamicArray<int> &sameArray = array;
    array = sameArray;

    EXPECT_EQ(array.GetSize(), 3);
    EXPECT_EQ(array.Get(0), 1);
    EXPECT_EQ(array.Get(1), 2);
    EXPECT_EQ(array.Get(2), 3);
}

TEST(DynamicArrayTests, SetAndOperatorAccessWork)
{
    DynamicArray<int> array(3);

    array.Set(0, 10);
    array.Set(1, 20);
    array[2] = 30;

    EXPECT_EQ(array.Get(0), 10);
    EXPECT_EQ(array.Get(1), 20);
    EXPECT_EQ(array.Get(2), 30);

    EXPECT_EQ(array[0], 10);
    EXPECT_EQ(array[1], 20);
    EXPECT_EQ(array[2], 30);
}

TEST(DynamicArrayTests, ResizeUpKeepsOldValues)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 3);

    array.Resize(5);

    EXPECT_EQ(array.GetSize(), 5);

    EXPECT_EQ(array.Get(0), 1);
    EXPECT_EQ(array.Get(1), 2);
    EXPECT_EQ(array.Get(2), 3);
    EXPECT_EQ(array.Get(3), 0);
    EXPECT_EQ(array.Get(4), 0);
}

TEST(DynamicArrayTests, ResizeDownKeepsPrefix)
{
    int items[] = {1, 2, 3, 4};
    DynamicArray<int> array(items, 4);

    array.Resize(2);

    EXPECT_EQ(array.GetSize(), 2);

    EXPECT_EQ(array.Get(0), 1);
    EXPECT_EQ(array.Get(1), 2);

    EXPECT_THROW(array.Get(2), std::out_of_range);
}

TEST(DynamicArrayTests, ResizeToZeroMakesArrayEmpty)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 3);

    array.Resize(0);

    EXPECT_EQ(array.GetSize(), 0);
    EXPECT_EQ(array.GetCapacity(), 0);

    EXPECT_THROW(array.Get(0), std::out_of_range);
}

TEST(DynamicArrayTests, EnsureCapacityDoesNotLoseValues)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 3);

    array.EnsureCapacity(10);

    EXPECT_EQ(array.GetSize(), 3);
    EXPECT_GE(array.GetCapacity(), 10);

    EXPECT_EQ(array.Get(0), 1);
    EXPECT_EQ(array.Get(1), 2);
    EXPECT_EQ(array.Get(2), 3);
}

TEST(DynamicArrayTests, ShrinkToFitMakesCapacityEqualSize)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 3);

    array.EnsureCapacity(10);
    ASSERT_GE(array.GetCapacity(), 10);

    array.ShrinkToFit();

    EXPECT_EQ(array.GetSize(), 3);
    EXPECT_EQ(array.GetCapacity(), 3);

    EXPECT_EQ(array.Get(0), 1);
    EXPECT_EQ(array.Get(1), 2);
    EXPECT_EQ(array.Get(2), 3);
}

TEST(DynamicArrayTests, EnumeratorTraversesValues)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 3);

    IEnumerator<int> *iterator = array.GetEnumerator();

    ASSERT_NE(iterator, nullptr);

    ASSERT_TRUE(iterator->HasNext());
    EXPECT_EQ(iterator->Next(), 1);

    ASSERT_TRUE(iterator->HasNext());
    EXPECT_EQ(iterator->Next(), 2);

    ASSERT_TRUE(iterator->HasNext());
    EXPECT_EQ(iterator->Next(), 3);

    EXPECT_FALSE(iterator->HasNext());

    delete iterator;
}

TEST(DynamicArrayTests, EnumeratorResetReturnsToBeginning)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 3);

    IEnumerator<int> *iterator = array.GetEnumerator();

    ASSERT_NE(iterator, nullptr);

    ASSERT_TRUE(iterator->HasNext());
    EXPECT_EQ(iterator->Next(), 1);

    ASSERT_TRUE(iterator->HasNext());
    EXPECT_EQ(iterator->Next(), 2);

    iterator->Reset();

    ASSERT_TRUE(iterator->HasNext());
    EXPECT_EQ(iterator->Next(), 1);

    delete iterator;
}

TEST(DynamicArrayTests, ThrowsOnInvalidArguments)
{
    EXPECT_THROW(DynamicArray<int> array(-1), std::invalid_argument);
    EXPECT_THROW(DynamicArray<int> array(nullptr, 3), std::invalid_argument);
}

TEST(DynamicArrayTests, ThrowsOnBadIndex)
{
    int items[] = {1, 2, 3};
    DynamicArray<int> array(items, 3);

    EXPECT_THROW(array.Get(-1), std::out_of_range);
    EXPECT_THROW(array.Get(3), std::out_of_range);
    EXPECT_THROW(array.Set(-1, 10), std::out_of_range);
    EXPECT_THROW(array.Set(3, 10), std::out_of_range);
    EXPECT_THROW(array[-1], std::out_of_range);
}