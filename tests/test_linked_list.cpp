#include <gtest/gtest.h>

#include "../LinkedList.h"
#include "../IEnumerator.h"

TEST(LinkedListTests, DefaultConstructorCreatesEmptyList)
{
    LinkedList<int> list;

    EXPECT_EQ(list.GetLength(), 0);
    EXPECT_THROW(list.GetFirst(), std::out_of_range);
    EXPECT_THROW(list.GetLast(), std::out_of_range);
}

TEST(LinkedListTests, ConstructorFromItemsCopiesValues)
{
    int items[] = {1, 2, 3};

    LinkedList<int> list(items, 3);

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.GetFirst(), 1);
    EXPECT_EQ(list.GetLast(), 3);

    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.Get(2), 3);
}

TEST(LinkedListTests, CopyConstructorCreatesIndependentCopy)
{
    int items[] = {1, 2, 3};
    LinkedList<int> original(items, 3);

    LinkedList<int> copy(original);

    original.RemoveFirst();
    original.Append(100);

    EXPECT_EQ(copy.GetLength(), 3);
    EXPECT_EQ(copy.Get(0), 1);
    EXPECT_EQ(copy.Get(1), 2);
    EXPECT_EQ(copy.Get(2), 3);

    EXPECT_EQ(original.GetLength(), 3);
    EXPECT_EQ(original.Get(0), 2);
    EXPECT_EQ(original.Get(1), 3);
    EXPECT_EQ(original.Get(2), 100);
}

TEST(LinkedListTests, AssignmentCreatesIndependentCopy)
{
    int firstItems[] = {1, 2, 3};
    int secondItems[] = {10, 20};

    LinkedList<int> first(firstItems, 3);
    LinkedList<int> second(secondItems, 2);

    first = second;

    second.RemoveFirst();
    second.Append(100);

    EXPECT_EQ(first.GetLength(), 2);
    EXPECT_EQ(first.Get(0), 10);
    EXPECT_EQ(first.Get(1), 20);

    EXPECT_EQ(second.GetLength(), 2);
    EXPECT_EQ(second.Get(0), 20);
    EXPECT_EQ(second.Get(1), 100);
}

TEST(LinkedListTests, SelfAssignmentKeepsListValid)
{
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    LinkedList<int> &sameList = list;
    list = sameList;

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.Get(2), 3);
}

TEST(LinkedListTests, AppendAddsToEnd)
{
    LinkedList<int> list;

    list.Append(1);
    list.Append(2);
    list.Append(3);

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.Get(2), 3);
}

TEST(LinkedListTests, PrependAddsToBeginning)
{
    LinkedList<int> list;

    list.Prepend(3);
    list.Prepend(2);
    list.Prepend(1);

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.Get(2), 3);
}

TEST(LinkedListTests, InsertAtInsertsInBeginningMiddleAndEnd)
{
    LinkedList<int> list;

    list.Append(1);
    list.Append(3);

    list.InsertAt(0, 0);
    list.InsertAt(2, 2);
    list.InsertAt(4, 4);

    EXPECT_EQ(list.GetLength(), 5);
    EXPECT_EQ(list.Get(0), 0);
    EXPECT_EQ(list.Get(1), 1);
    EXPECT_EQ(list.Get(2), 2);
    EXPECT_EQ(list.Get(3), 3);
    EXPECT_EQ(list.Get(4), 4);
}

TEST(LinkedListTests, RemoveOperationsReturnRemovedValues)
{
    int items[] = {1, 2, 3, 4};
    LinkedList<int> list(items, 4);

    EXPECT_EQ(list.RemoveFirst(), 1);
    EXPECT_EQ(list.RemoveLast(), 4);
    EXPECT_EQ(list.RemoveAt(1), 3);

    EXPECT_EQ(list.GetLength(), 1);
    EXPECT_EQ(list.Get(0), 2);
}

TEST(LinkedListTests, RemoveLastWorksForSingleElement)
{
    LinkedList<int> list;

    list.Append(10);

    EXPECT_EQ(list.RemoveLast(), 10);
    EXPECT_EQ(list.GetLength(), 0);
    EXPECT_THROW(list.GetFirst(), std::out_of_range);
}

TEST(LinkedListTests, GetSubListCreatesCorrectCopy)
{
    int items[] = {1, 2, 3, 4, 5};
    LinkedList<int> list(items, 5);

    LinkedList<int> *sub = list.GetSubList(1, 3);

    ASSERT_NE(sub, nullptr);

    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), 2);
    EXPECT_EQ(sub->Get(1), 3);
    EXPECT_EQ(sub->Get(2), 4);

    delete sub;
}

TEST(LinkedListTests, ConcatReturnsNewList)
{
    int firstItems[] = {1, 2};
    int secondItems[] = {3, 4};

    LinkedList<int> first(firstItems, 2);
    LinkedList<int> second(secondItems, 2);

    LinkedList<int> *result = first.Concat(second);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(first.GetLength(), 2);
    EXPECT_EQ(first.Get(0), 1);
    EXPECT_EQ(first.Get(1), 2);

    EXPECT_EQ(second.GetLength(), 2);
    EXPECT_EQ(second.Get(0), 3);
    EXPECT_EQ(second.Get(1), 4);

    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 2);
    EXPECT_EQ(result->Get(2), 3);
    EXPECT_EQ(result->Get(3), 4);

    delete result;
}

TEST(LinkedListTests, EnumeratorResetReturnsToBeginning)
{
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    IEnumerator<int> *iterator = list.GetEnumerator();

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

TEST(LinkedListTests, ThrowsOnInvalidArguments)
{
    EXPECT_THROW(LinkedList<int> list(nullptr, 3), std::invalid_argument);
    EXPECT_THROW(LinkedList<int> list(nullptr, -1), std::invalid_argument);
}

TEST(LinkedListTests, ThrowsOnBadIndex)
{
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    EXPECT_THROW(list.Get(-1), std::out_of_range);
    EXPECT_THROW(list.Get(3), std::out_of_range);
    EXPECT_THROW(list.InsertAt(10, -1), std::out_of_range);
    EXPECT_THROW(list.InsertAt(10, 4), std::out_of_range);
    EXPECT_THROW(list.RemoveAt(-1), std::out_of_range);
    EXPECT_THROW(list.RemoveAt(3), std::out_of_range);
    EXPECT_THROW(list.GetSubList(-1, 1), std::out_of_range);
    EXPECT_THROW(list.GetSubList(1, 3), std::out_of_range);
    EXPECT_THROW(list.GetSubList(2, 1), std::out_of_range);
}

TEST(LinkedListTests, ThrowsWhenRemovingFromEmptyList)
{
    LinkedList<int> list;

    EXPECT_THROW(list.RemoveFirst(), std::out_of_range);
    EXPECT_THROW(list.RemoveLast(), std::out_of_range);
    EXPECT_THROW(list.RemoveAt(0), std::out_of_range);
}