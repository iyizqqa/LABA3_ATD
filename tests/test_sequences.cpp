#include <gtest/gtest.h>

#include <functional>

#include "../ImmutableArraySequence.h"
#include "../ImmutableListSequence.h"
#include "../MutableArraySequence.h"
#include "../MutableListSequence.h"

static int Square(const int &value)
{
    return value * value;
}

static bool IsEven(const int &value)
{
    return value % 2 == 0;
}

static int Sum(const int &left, const int &right)
{
    return left + right;
}

TEST(MutableArraySequenceTests, AppendPrependInsertModifySameObject)
{
    int items[] = {1, 2, 3};
    MutableArraySequence<int> sequence(items, 3);

    Sequence<int> *afterAppend = sequence.Append(4);
    EXPECT_EQ(afterAppend, &sequence);

    Sequence<int> *afterPrepend = sequence.Prepend(0);
    EXPECT_EQ(afterPrepend, &sequence);

    Sequence<int> *afterInsert = sequence.InsertAt(100, 2);
    EXPECT_EQ(afterInsert, &sequence);

    EXPECT_EQ(sequence.GetLength(), 6);
    EXPECT_EQ(sequence.Get(0), 0);
    EXPECT_EQ(sequence.Get(1), 1);
    EXPECT_EQ(sequence.Get(2), 100);
    EXPECT_EQ(sequence.Get(3), 2);
    EXPECT_EQ(sequence.Get(4), 3);
    EXPECT_EQ(sequence.Get(5), 4);
}

TEST(ImmutableArraySequenceTests, AppendPrependInsertReturnNewObjects)
{
    int items[] = {1, 2, 3};
    ImmutableArraySequence<int> sequence(items, 3);

    Sequence<int> *appended = sequence.Append(4);
    Sequence<int> *prepended = sequence.Prepend(0);
    Sequence<int> *inserted = sequence.InsertAt(100, 1);

    EXPECT_NE(appended, &sequence);
    EXPECT_NE(prepended, &sequence);
    EXPECT_NE(inserted, &sequence);

    EXPECT_EQ(sequence.GetLength(), 3);
    EXPECT_EQ(sequence.Get(0), 1);
    EXPECT_EQ(sequence.Get(1), 2);
    EXPECT_EQ(sequence.Get(2), 3);

    EXPECT_EQ(appended->GetLength(), 4);
    EXPECT_EQ(appended->Get(0), 1);
    EXPECT_EQ(appended->Get(1), 2);
    EXPECT_EQ(appended->Get(2), 3);
    EXPECT_EQ(appended->Get(3), 4);

    EXPECT_EQ(prepended->GetLength(), 4);
    EXPECT_EQ(prepended->Get(0), 0);
    EXPECT_EQ(prepended->Get(1), 1);
    EXPECT_EQ(prepended->Get(2), 2);
    EXPECT_EQ(prepended->Get(3), 3);

    EXPECT_EQ(inserted->GetLength(), 4);
    EXPECT_EQ(inserted->Get(0), 1);
    EXPECT_EQ(inserted->Get(1), 100);
    EXPECT_EQ(inserted->Get(2), 2);
    EXPECT_EQ(inserted->Get(3), 3);

    delete appended;
    delete prepended;
    delete inserted;
}

TEST(MutableListSequenceTests, AppendPrependInsertModifySameObject)
{
    int items[] = {1, 2, 3};
    MutableListSequence<int> sequence(items, 3);

    Sequence<int> *afterAppend = sequence.Append(4);
    EXPECT_EQ(afterAppend, &sequence);

    Sequence<int> *afterPrepend = sequence.Prepend(0);
    EXPECT_EQ(afterPrepend, &sequence);

    Sequence<int> *afterInsert = sequence.InsertAt(100, 2);
    EXPECT_EQ(afterInsert, &sequence);

    EXPECT_EQ(sequence.GetLength(), 6);
    EXPECT_EQ(sequence.Get(0), 0);
    EXPECT_EQ(sequence.Get(1), 1);
    EXPECT_EQ(sequence.Get(2), 100);
    EXPECT_EQ(sequence.Get(3), 2);
    EXPECT_EQ(sequence.Get(4), 3);
    EXPECT_EQ(sequence.Get(5), 4);
}

TEST(ImmutableListSequenceTests, AppendPrependInsertReturnNewObjects)
{
    int items[] = {1, 2, 3};
    ImmutableListSequence<int> sequence(items, 3);

    Sequence<int> *appended = sequence.Append(4);
    Sequence<int> *prepended = sequence.Prepend(0);
    Sequence<int> *inserted = sequence.InsertAt(100, 1);

    EXPECT_NE(appended, &sequence);
    EXPECT_NE(prepended, &sequence);
    EXPECT_NE(inserted, &sequence);

    EXPECT_EQ(sequence.GetLength(), 3);
    EXPECT_EQ(sequence.Get(0), 1);
    EXPECT_EQ(sequence.Get(1), 2);
    EXPECT_EQ(sequence.Get(2), 3);

    EXPECT_EQ(appended->GetLength(), 4);
    EXPECT_EQ(appended->Get(0), 1);
    EXPECT_EQ(appended->Get(1), 2);
    EXPECT_EQ(appended->Get(2), 3);
    EXPECT_EQ(appended->Get(3), 4);

    EXPECT_EQ(prepended->GetLength(), 4);
    EXPECT_EQ(prepended->Get(0), 0);
    EXPECT_EQ(prepended->Get(1), 1);
    EXPECT_EQ(prepended->Get(2), 2);
    EXPECT_EQ(prepended->Get(3), 3);

    EXPECT_EQ(inserted->GetLength(), 4);
    EXPECT_EQ(inserted->Get(0), 1);
    EXPECT_EQ(inserted->Get(1), 100);
    EXPECT_EQ(inserted->Get(2), 2);
    EXPECT_EQ(inserted->Get(3), 3);

    delete appended;
    delete prepended;
    delete inserted;
}

TEST(SequenceTests, CloneCreatesIndependentMutableArrayCopy)
{
    int items[] = {1, 2, 3};
    MutableArraySequence<int> sequence(items, 3);

    Sequence<int> *clone = sequence.Clone();

    ASSERT_NE(clone, nullptr);
    EXPECT_NE(clone, &sequence);

    EXPECT_EQ(clone->GetLength(), 3);
    EXPECT_EQ(clone->Get(0), 1);
    EXPECT_EQ(clone->Get(1), 2);
    EXPECT_EQ(clone->Get(2), 3);

    sequence.Append(4);

    EXPECT_EQ(clone->GetLength(), 3);
    EXPECT_EQ(clone->Get(0), 1);
    EXPECT_EQ(clone->Get(1), 2);
    EXPECT_EQ(clone->Get(2), 3);

    delete clone;
}

TEST(SequenceTests, CloneCreatesIndependentImmutableListCopy)
{
    int items[] = {1, 2, 3};
    ImmutableListSequence<int> sequence(items, 3);

    Sequence<int> *clone = sequence.Clone();

    ASSERT_NE(clone, nullptr);
    EXPECT_NE(clone, &sequence);

    EXPECT_EQ(clone->GetLength(), 3);
    EXPECT_EQ(clone->Get(0), 1);
    EXPECT_EQ(clone->Get(1), 2);
    EXPECT_EQ(clone->Get(2), 3);

    Sequence<int> *changed = sequence.Append(4);

    EXPECT_EQ(clone->GetLength(), 3);
    EXPECT_EQ(clone->Get(0), 1);
    EXPECT_EQ(clone->Get(1), 2);
    EXPECT_EQ(clone->Get(2), 3);

    delete changed;
    delete clone;
}

TEST(SequenceTests, GetSubsequenceWorksForArrayAndList)
{
    int items[] = {1, 2, 3, 4, 5};

    MutableArraySequence<int> arraySequence(items, 5);
    MutableListSequence<int> listSequence(items, 5);

    Sequence<int> *arraySub = arraySequence.GetSubsequence(1, 3);
    Sequence<int> *listSub = listSequence.GetSubsequence(1, 3);

    ASSERT_NE(arraySub, nullptr);
    ASSERT_NE(listSub, nullptr);

    EXPECT_EQ(arraySub->GetLength(), 3);
    EXPECT_EQ(arraySub->Get(0), 2);
    EXPECT_EQ(arraySub->Get(1), 3);
    EXPECT_EQ(arraySub->Get(2), 4);

    EXPECT_EQ(listSub->GetLength(), 3);
    EXPECT_EQ(listSub->Get(0), 2);
    EXPECT_EQ(listSub->Get(1), 3);
    EXPECT_EQ(listSub->Get(2), 4);

    delete arraySub;
    delete listSub;
}

TEST(SequenceTests, ConcatWorksForDifferentSequenceImplementations)
{
    int arrayItems[] = {1, 2};
    int listItems[] = {3, 4};

    MutableArraySequence<int> arraySequence(arrayItems, 2);
    MutableListSequence<int> listSequence(listItems, 2);

    Sequence<int> *result = arraySequence.Concat(listSequence);

    EXPECT_EQ(result, &arraySequence);

    EXPECT_EQ(arraySequence.GetLength(), 4);
    EXPECT_EQ(arraySequence.Get(0), 1);
    EXPECT_EQ(arraySequence.Get(1), 2);
    EXPECT_EQ(arraySequence.Get(2), 3);
    EXPECT_EQ(arraySequence.Get(3), 4);
}

TEST(SequenceTests, ImmutableConcatReturnsNewObjectAndDoesNotChangeSource)
{
    int firstItems[] = {1, 2};
    int secondItems[] = {3, 4};

    ImmutableArraySequence<int> first(firstItems, 2);
    MutableListSequence<int> second(secondItems, 2);

    Sequence<int> *result = first.Concat(second);

    ASSERT_NE(result, nullptr);
    EXPECT_NE(result, &first);

    EXPECT_EQ(first.GetLength(), 2);
    EXPECT_EQ(first.Get(0), 1);
    EXPECT_EQ(first.Get(1), 2);

    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 2);
    EXPECT_EQ(result->Get(2), 3);
    EXPECT_EQ(result->Get(3), 4);

    delete result;
}

TEST(SequenceTests, ConcatSelfWorksForMutableArrayAndList)
{
    int items[] = {1, 2, 3};

    MutableArraySequence<int> arraySequence(items, 3);
    MutableListSequence<int> listSequence(items, 3);

    arraySequence.Concat(arraySequence);
    listSequence.Concat(listSequence);

    EXPECT_EQ(arraySequence.GetLength(), 6);
    EXPECT_EQ(arraySequence.Get(0), 1);
    EXPECT_EQ(arraySequence.Get(1), 2);
    EXPECT_EQ(arraySequence.Get(2), 3);
    EXPECT_EQ(arraySequence.Get(3), 1);
    EXPECT_EQ(arraySequence.Get(4), 2);
    EXPECT_EQ(arraySequence.Get(5), 3);

    EXPECT_EQ(listSequence.GetLength(), 6);
    EXPECT_EQ(listSequence.Get(0), 1);
    EXPECT_EQ(listSequence.Get(1), 2);
    EXPECT_EQ(listSequence.Get(2), 3);
    EXPECT_EQ(listSequence.Get(3), 1);
    EXPECT_EQ(listSequence.Get(4), 2);
    EXPECT_EQ(listSequence.Get(5), 3);
}

TEST(SequenceTests, MapWhereReduceWork)
{
    int items[] = {1, 2, 3, 4};
    MutableArraySequence<int> sequence(items, 4);

    Sequence<int> *mapped = sequence.Map(Square);
    Sequence<int> *filtered = sequence.Where(IsEven);
    int reduced = sequence.Reduce(Sum, 0);

    ASSERT_NE(mapped, nullptr);
    ASSERT_NE(filtered, nullptr);

    EXPECT_EQ(mapped->GetLength(), 4);
    EXPECT_EQ(mapped->Get(0), 1);
    EXPECT_EQ(mapped->Get(1), 4);
    EXPECT_EQ(mapped->Get(2), 9);
    EXPECT_EQ(mapped->Get(3), 16);

    EXPECT_EQ(filtered->GetLength(), 2);
    EXPECT_EQ(filtered->Get(0), 2);
    EXPECT_EQ(filtered->Get(1), 4);

    EXPECT_EQ(reduced, 10);

    delete mapped;
    delete filtered;
}

TEST(SequenceTests, ImmutableMapWhereDoNotChangeSource)
{
    int items[] = {1, 2, 3, 4};
    ImmutableListSequence<int> sequence(items, 4);

    Sequence<int> *mapped = sequence.Map(Square);
    Sequence<int> *filtered = sequence.Where(IsEven);

    ASSERT_NE(mapped, nullptr);
    ASSERT_NE(filtered, nullptr);

    EXPECT_EQ(sequence.GetLength(), 4);
    EXPECT_EQ(sequence.Get(0), 1);
    EXPECT_EQ(sequence.Get(1), 2);
    EXPECT_EQ(sequence.Get(2), 3);
    EXPECT_EQ(sequence.Get(3), 4);

    EXPECT_EQ(mapped->GetLength(), 4);
    EXPECT_EQ(mapped->Get(0), 1);
    EXPECT_EQ(mapped->Get(1), 4);
    EXPECT_EQ(mapped->Get(2), 9);
    EXPECT_EQ(mapped->Get(3), 16);

    EXPECT_EQ(filtered->GetLength(), 2);
    EXPECT_EQ(filtered->Get(0), 2);
    EXPECT_EQ(filtered->Get(1), 4);

    delete mapped;
    delete filtered;
}

TEST(SequenceTests, ReduceOnEmptySequenceReturnsInitialValue)
{
    MutableArraySequence<int> sequence;

    EXPECT_EQ(sequence.Reduce(Sum, 100), 100);
}

TEST(SequenceTests, NewInstancePreservesMutableBehavior)
{
    int items[] = {1, 2, 3};
    MutableArraySequence<int> mutableArray(items, 3);

    Sequence<int> *empty = mutableArray.NewInstance();

    ASSERT_NE(empty, nullptr);

    Sequence<int> *changed = empty->Append(10);

    EXPECT_EQ(changed, empty);

    EXPECT_EQ(empty->GetLength(), 1);
    EXPECT_EQ(empty->Get(0), 10);

    delete empty;
}

TEST(SequenceTests, NewInstancePreservesImmutableBehavior)
{
    int items[] = {1, 2, 3};
    ImmutableArraySequence<int> immutableArray(items, 3);

    Sequence<int> *empty = immutableArray.NewInstance();

    ASSERT_NE(empty, nullptr);

    Sequence<int> *changed = empty->Append(10);

    ASSERT_NE(changed, nullptr);
    EXPECT_NE(changed, empty);

    EXPECT_EQ(empty->GetLength(), 0);

    EXPECT_EQ(changed->GetLength(), 1);
    EXPECT_EQ(changed->Get(0), 10);

    delete empty;
    delete changed;
}

TEST(SequenceTests, ThrowsOnBadIndices)
{
    int items[] = {1, 2, 3};

    MutableArraySequence<int> arraySequence(items, 3);
    MutableListSequence<int> listSequence(items, 3);

    EXPECT_THROW(arraySequence.Get(-1), std::out_of_range);
    EXPECT_THROW(arraySequence.Get(3), std::out_of_range);
    EXPECT_THROW(arraySequence.InsertAt(10, -1), std::out_of_range);
    EXPECT_THROW(arraySequence.InsertAt(10, 4), std::out_of_range);
    EXPECT_THROW(arraySequence.GetSubsequence(2, 1), std::out_of_range);

    EXPECT_THROW(listSequence.Get(-1), std::out_of_range);
    EXPECT_THROW(listSequence.Get(3), std::out_of_range);
    EXPECT_THROW(listSequence.InsertAt(10, -1), std::out_of_range);
    EXPECT_THROW(listSequence.InsertAt(10, 4), std::out_of_range);
    EXPECT_THROW(listSequence.GetSubsequence(2, 1), std::out_of_range);
}

TEST(SequenceTests, ThrowsOnEmptyGetFirstGetLast)
{
    MutableArraySequence<int> arraySequence;
    MutableListSequence<int> listSequence;

    EXPECT_THROW(arraySequence.GetFirst(), std::out_of_range);
    EXPECT_THROW(arraySequence.GetLast(), std::out_of_range);

    EXPECT_THROW(listSequence.GetFirst(), std::out_of_range);
    EXPECT_THROW(listSequence.GetLast(), std::out_of_range);
}

TEST(SequenceTests, ThrowsOnNullFunctions)
{
    int items[] = {1, 2, 3};
    MutableArraySequence<int> sequence(items, 3);

    std::function<int(const int &)> nullMapper;
    std::function<bool(const int &)> nullPredicate;
    std::function<int(const int &, const int &)> nullReducer;

    EXPECT_THROW(sequence.Map(nullMapper), std::invalid_argument);
    EXPECT_THROW(sequence.Where(nullPredicate), std::invalid_argument);
    EXPECT_THROW(sequence.Reduce(nullReducer, 0), std::invalid_argument);
}