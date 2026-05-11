#include "Tests.h"

#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>

#include "DynamicArray.h"
#include "LinkedList.h"
#include "MutableArraySequence.h"
#include "ImmutableArraySequence.h"
#include "MutableListSequence.h"
#include "ImmutableListSequence.h"

class TestFailure : public std::runtime_error
{
public:
    explicit TestFailure(const std::string &message)
        : std::runtime_error(message) {}
};

static void Assert(bool condition, const std::string &message)
{
    if (!condition)
    {
        throw TestFailure(message);
    }
}

template <class ExceptionType, class Func>
static void AssertThrows(Func func, const std::string &message)
{
    try
    {
        func();
    }
    catch (const ExceptionType &)
    {
        return;
    }

    throw TestFailure(message);
}

template <class T>
static void AssertEnumerableEquals(
    const IEnumerable<T> &enumerable,
    const T *expected,
    int count,
    const std::string &message)
{
    IEnumerator<T> *iterator = enumerable.GetEnumerator();
    int index = 0;

    try
    {
        while (iterator->HasNext())
        {
            Assert(index < count, message + " (too many elements)");

            Assert(
                iterator->Next() == expected[index],
                message + " (element mismatch at index " + std::to_string(index) + ")");

            ++index;
        }

        delete iterator;
    }
    catch (...)
    {
        delete iterator;
        throw;
    }

    Assert(index == count, message + " (too few elements)");
}

template <class T>
static void AssertSequenceEquals(
    const Sequence<T> &sequence,
    const T *expected,
    int count,
    const std::string &message)
{
    Assert(sequence.GetLength() == count, message + " (length mismatch)");
    AssertEnumerableEquals(sequence, expected, count, message);
}

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

static void TestDynamicArray()
{
    DynamicArray<int> array(3);

    Assert(array.GetSize() == 3, "DynamicArray: size constructor failed");
    Assert(array.Get(0) == 0, "DynamicArray: default value failed");

    array.Set(1, 10);

    Assert(array.Get(1) == 10, "DynamicArray: Set/Get failed");

    array.EnsureCapacity(20);

    Assert(array.GetCapacity() >= 20, "DynamicArray: EnsureCapacity failed");
    Assert(array.Get(1) == 10, "DynamicArray: value lost after EnsureCapacity");

    array.Resize(4);
    array.Set(3, 30);

    int expected[] = {0, 10, 0, 30};

    AssertEnumerableEquals(array, expected, 4, "DynamicArray: iterator failed");

    array.Resize(1);

    Assert(array.GetSize() == 1, "DynamicArray: Resize down failed");

    AssertThrows<std::invalid_argument>(
        []()
        {
            DynamicArray<int> invalid(-1);
        },
        "DynamicArray: negative size must throw");

    AssertThrows<std::invalid_argument>(
        []()
        {
            DynamicArray<int> invalid(nullptr, 1);
        },
        "DynamicArray: null items must throw");

    AssertThrows<std::out_of_range>(
        [&array]()
        {
            array.Get(10);
        },
        "DynamicArray: invalid Get must throw");

    AssertThrows<std::out_of_range>(
        [&array]()
        {
            array.Set(-1, 1);
        },
        "DynamicArray: invalid Set must throw");
}

static void TestDynamicArrayResizeToZero()
{
    DynamicArray<int> array(5);

    array.Set(0, 10);
    array.Set(1, 20);

    array.Resize(0);

    Assert(array.GetSize() == 0, "DynamicArray: Resize(0) size failed");
    Assert(array.GetCapacity() == 0, "DynamicArray: Resize(0) capacity failed");

    AssertThrows<std::out_of_range>(
        [&array]()
        {
            array.Get(0);
        },
        "DynamicArray: Get after Resize(0) must throw");
}

static void TestDynamicArrayShrinkToFit()
{
    DynamicArray<int> array(10);

    array.Set(0, 1);
    array.Set(1, 2);
    array.Set(2, 3);

    array.Resize(3);

    Assert(array.GetSize() == 3, "DynamicArray: Resize before ShrinkToFit failed");
    Assert(array.GetCapacity() >= 3, "DynamicArray: capacity before ShrinkToFit is invalid");

    array.ShrinkToFit();

    Assert(array.GetSize() == 3, "DynamicArray: ShrinkToFit changed size");
    Assert(array.GetCapacity() == 3, "DynamicArray: ShrinkToFit capacity failed");

    Assert(array.Get(0) == 1, "DynamicArray: ShrinkToFit lost first element");
    Assert(array.Get(1) == 2, "DynamicArray: ShrinkToFit lost second element");
    Assert(array.Get(2) == 3, "DynamicArray: ShrinkToFit lost third element");
}

static void TestLinkedList()
{
    int items[] = {1, 2, 3};

    LinkedList<int> list(items, 3);

    Assert(list.GetLength() == 3, "LinkedList: constructor failed");
    Assert(list.GetFirst() == 1, "LinkedList: GetFirst failed");
    Assert(list.GetLast() == 3, "LinkedList: GetLast failed");

    list.Prepend(0);
    list.Append(4);
    list.InsertAt(99, 2);

    int expected[] = {0, 1, 99, 2, 3, 4};

    AssertEnumerableEquals(list, expected, 6, "LinkedList: iterator/mutations failed");

    LinkedList<int> *sub = list.GetSubList(1, 4);

    int expectedSub[] = {1, 99, 2, 3};

    AssertEnumerableEquals(*sub, expectedSub, 4, "LinkedList: GetSubList failed");

    delete sub;

    LinkedList<int> empty;

    AssertThrows<std::out_of_range>(
        [&empty]()
        {
            empty.GetFirst();
        },
        "LinkedList: empty GetFirst must throw");

    AssertThrows<std::out_of_range>(
        [&empty]()
        {
            empty.GetLast();
        },
        "LinkedList: empty GetLast must throw");

    AssertThrows<std::invalid_argument>(
        []()
        {
            LinkedList<int> invalid(nullptr, 1);
        },
        "LinkedList: null items must throw");

    AssertThrows<std::out_of_range>(
        [&list]()
        {
            list.Get(100);
        },
        "LinkedList: invalid Get must throw");

    AssertThrows<std::out_of_range>(
        [&list]()
        {
            list.GetSubList(4, 1);
        },
        "LinkedList: invalid sublist must throw");
}

static void TestMutableArraySequence()
{
    int items[] = {1, 2, 3};

    MutableArraySequence<int> sequence(items, 3);

    Sequence<int> *same = sequence.Append(4);

    Assert(same == &sequence, "MutableArraySequence: Append must return this");

    sequence.Prepend(0);
    sequence.InsertAt(99, 2);

    int expected[] = {0, 1, 99, 2, 3, 4};

    AssertSequenceEquals(sequence, expected, 6, "MutableArraySequence: mutations failed");

    Sequence<int> *mapped = sequence.Map(Square);

    int expectedMap[] = {0, 1, 9801, 4, 9, 16};

    AssertSequenceEquals(*mapped, expectedMap, 6, "MutableArraySequence: Map failed");

    delete mapped;

    Sequence<int> *filtered = sequence.Where(IsEven);

    int expectedWhere[] = {0, 2, 4};

    AssertSequenceEquals(*filtered, expectedWhere, 3, "MutableArraySequence: Where failed");

    delete filtered;

    Assert(sequence.Reduce(Sum, 0) == 109, "MutableArraySequence: Reduce failed");

    Sequence<int> *sub = sequence.GetSubsequence(1, 3);

    int expectedSub[] = {1, 99, 2};

    AssertSequenceEquals(*sub, expectedSub, 3, "MutableArraySequence: GetSubsequence failed");

    delete sub;

    AssertThrows<std::out_of_range>(
        [&sequence]()
        {
            sequence.InsertAt(1, -1);
        },
        "MutableArraySequence: negative InsertAt must throw");

    const std::function<int(const int &)> emptyMap;

    AssertThrows<std::invalid_argument>(
        [&sequence, &emptyMap]()
        {
            Sequence<int> *result = sequence.Map(emptyMap);
            delete result;
        },
        "MutableArraySequence: null Map must throw");
}

static void TestImmutableArraySequence()
{
    int items[] = {1, 2, 3};

    ImmutableArraySequence<int> sequence(items, 3);

    Sequence<int> *copy = sequence.Append(4);

    Assert(copy != &sequence, "ImmutableArraySequence: Append must return new object");

    int original[] = {1, 2, 3};
    int changed[] = {1, 2, 3, 4};

    AssertSequenceEquals(sequence, original, 3, "ImmutableArraySequence: source changed");
    AssertSequenceEquals(*copy, changed, 4, "ImmutableArraySequence: result incorrect");

    delete copy;

    Sequence<int> *inserted = sequence.InsertAt(99, 1);

    int insertedExpected[] = {1, 99, 2, 3};

    AssertSequenceEquals(sequence, original, 3, "ImmutableArraySequence: InsertAt changed source");
    AssertSequenceEquals(*inserted, insertedExpected, 4, "ImmutableArraySequence: InsertAt result incorrect");

    delete inserted;
}

static void TestMutableListSequence()
{
    int items[] = {1, 2, 3};

    MutableListSequence<int> sequence(items, 3);

    Sequence<int> *same = sequence.Append(4);

    Assert(same == &sequence, "MutableListSequence: Append must return this");

    sequence.Prepend(0);
    sequence.InsertAt(99, 2);

    int expected[] = {0, 1, 99, 2, 3, 4};

    AssertSequenceEquals(sequence, expected, 6, "MutableListSequence: mutations failed");

    Sequence<int> *mapped = sequence.Map(Square);

    int expectedMap[] = {0, 1, 9801, 4, 9, 16};

    AssertSequenceEquals(*mapped, expectedMap, 6, "MutableListSequence: Map failed");

    delete mapped;

    Sequence<int> *filtered = sequence.Where(IsEven);

    int expectedWhere[] = {0, 2, 4};

    AssertSequenceEquals(*filtered, expectedWhere, 3, "MutableListSequence: Where failed");

    delete filtered;

    Assert(sequence.Reduce(Sum, 0) == 109, "MutableListSequence: Reduce failed");
}

static void TestImmutableListSequence()
{
    int items[] = {1, 2, 3};

    ImmutableListSequence<int> sequence(items, 3);

    Sequence<int> *copy = sequence.Prepend(0);

    Assert(copy != &sequence, "ImmutableListSequence: Prepend must return new object");

    int original[] = {1, 2, 3};
    int changed[] = {0, 1, 2, 3};

    AssertSequenceEquals(sequence, original, 3, "ImmutableListSequence: source changed");
    AssertSequenceEquals(*copy, changed, 4, "ImmutableListSequence: result incorrect");

    delete copy;
}

static void TestEmptySequences()
{
    MutableArraySequence<int> arraySequence;
    MutableListSequence<int> listSequence;

    AssertThrows<std::out_of_range>(
        [&arraySequence]()
        {
            arraySequence.GetFirst();
        },
        "ArraySequence: empty GetFirst must throw");

    AssertThrows<std::out_of_range>(
        [&arraySequence]()
        {
            arraySequence.GetLast();
        },
        "ArraySequence: empty GetLast must throw");

    AssertThrows<std::out_of_range>(
        [&listSequence]()
        {
            listSequence.GetFirst();
        },
        "ListSequence: empty GetFirst must throw");

    AssertThrows<std::out_of_range>(
        [&listSequence]()
        {
            listSequence.GetLast();
        },
        "ListSequence: empty GetLast must throw");

    Assert(arraySequence.Reduce(Sum, 10) == 10, "ArraySequence: Reduce on empty failed");
    Assert(listSequence.Reduce(Sum, 10) == 10, "ListSequence: Reduce on empty failed");
}

static void TestIteratorReset()
{
    int items[] = {1, 2, 3};

    MutableArraySequence<int> arraySequence(items, 3);
    MutableListSequence<int> listSequence(items, 3);

    IEnumerator<int> *arrayIterator = arraySequence.GetEnumerator();

    Assert(arrayIterator->Next() == 1, "Array iterator: first Next failed");
    Assert(arrayIterator->Next() == 2, "Array iterator: second Next failed");

    arrayIterator->Reset();

    Assert(arrayIterator->Next() == 1, "Array iterator: Reset failed");

    delete arrayIterator;

    IEnumerator<int> *listIterator = listSequence.GetEnumerator();

    Assert(listIterator->Next() == 1, "List iterator: first Next failed");
    Assert(listIterator->Next() == 2, "List iterator: second Next failed");

    listIterator->Reset();

    Assert(listIterator->Next() == 1, "List iterator: Reset failed");

    delete listIterator;
}

static void TestConcatSelf()
{
    int items[] = {1, 2, 3};

    MutableArraySequence<int> arraySequence(items, 3);
    arraySequence.Concat(arraySequence);

    int expectedArray[] = {1, 2, 3, 1, 2, 3};

    AssertSequenceEquals(arraySequence, expectedArray, 6, "MutableArraySequence: self concat failed");

    MutableListSequence<int> listSequence(items, 3);
    listSequence.Concat(listSequence);

    int expectedList[] = {1, 2, 3, 1, 2, 3};

    AssertSequenceEquals(listSequence, expectedList, 6, "MutableListSequence: self concat failed");
}

static void TestNewInstance()
{
    int items[] = {1, 2, 3};

    MutableArraySequence<int> mutableArray(items, 3);
    Sequence<int> *emptyMutableArray = mutableArray.NewInstance();

    Assert(emptyMutableArray->GetLength() == 0, "MutableArraySequence: NewInstance must be empty");

    Sequence<int> *sameArray = emptyMutableArray->Append(10);

    Assert(sameArray == emptyMutableArray, "MutableArraySequence: NewInstance must preserve mutable behavior");

    int expectedMutableArray[] = {10};

    AssertSequenceEquals(*emptyMutableArray, expectedMutableArray, 1, "MutableArraySequence: NewInstance Append failed");

    delete emptyMutableArray;

    ImmutableArraySequence<int> immutableArray(items, 3);
    Sequence<int> *emptyImmutableArray = immutableArray.NewInstance();

    Assert(emptyImmutableArray->GetLength() == 0, "ImmutableArraySequence: NewInstance must be empty");

    Sequence<int> *changedArray = emptyImmutableArray->Append(10);

    Assert(changedArray != emptyImmutableArray, "ImmutableArraySequence: NewInstance must preserve immutable behavior");
    Assert(emptyImmutableArray->GetLength() == 0, "ImmutableArraySequence: Append changed source");

    int expectedImmutableArray[] = {10};

    AssertSequenceEquals(*changedArray, expectedImmutableArray, 1, "ImmutableArraySequence: NewInstance Append result failed");

    delete emptyImmutableArray;
    delete changedArray;

    MutableListSequence<int> mutableList(items, 3);
    Sequence<int> *emptyMutableList = mutableList.NewInstance();

    Assert(emptyMutableList->GetLength() == 0, "MutableListSequence: NewInstance must be empty");

    Sequence<int> *sameList = emptyMutableList->Append(10);

    Assert(sameList == emptyMutableList, "MutableListSequence: NewInstance must preserve mutable behavior");

    int expectedMutableList[] = {10};

    AssertSequenceEquals(*emptyMutableList, expectedMutableList, 1, "MutableListSequence: NewInstance Append failed");

    delete emptyMutableList;

    ImmutableListSequence<int> immutableList(items, 3);
    Sequence<int> *emptyImmutableList = immutableList.NewInstance();

    Assert(emptyImmutableList->GetLength() == 0, "ImmutableListSequence: NewInstance must be empty");

    Sequence<int> *changedList = emptyImmutableList->Append(10);

    Assert(changedList != emptyImmutableList, "ImmutableListSequence: NewInstance must preserve immutable behavior");
    Assert(emptyImmutableList->GetLength() == 0, "ImmutableListSequence: Append changed source");

    int expectedImmutableList[] = {10};

    AssertSequenceEquals(*changedList, expectedImmutableList, 1, "ImmutableListSequence: NewInstance Append result failed");

    delete emptyImmutableList;
    delete changedList;
}

static void TestImmutableMapWhere()
{
    int items[] = {1, 2, 3, 4};

    ImmutableArraySequence<int> arraySequence(items, 4);

    Sequence<int> *mappedArray = arraySequence.Map(Square);

    int expectedMapped[] = {1, 4, 9, 16};

    AssertSequenceEquals(*mappedArray, expectedMapped, 4, "ImmutableArraySequence: common Map failed");

    int original[] = {1, 2, 3, 4};

    AssertSequenceEquals(arraySequence, original, 4, "ImmutableArraySequence: Map changed source");

    delete mappedArray;

    ImmutableListSequence<int> listSequence(items, 4);

    Sequence<int> *filteredList = listSequence.Where(IsEven);

    int expectedFiltered[] = {2, 4};

    AssertSequenceEquals(*filteredList, expectedFiltered, 2, "ImmutableListSequence: common Where failed");
    AssertSequenceEquals(listSequence, original, 4, "ImmutableListSequence: Where changed source");

    delete filteredList;
}

void RunAllTests()
{
    TestDynamicArray();
    TestDynamicArrayResizeToZero();
    TestDynamicArrayShrinkToFit();

    TestLinkedList();

    TestMutableArraySequence();
    TestImmutableArraySequence();

    TestMutableListSequence();
    TestImmutableListSequence();

    TestEmptySequences();
    TestIteratorReset();
    TestConcatSelf();

    TestNewInstance();
    TestImmutableMapWhere();

    std::cout << "All tests passed successfully.\n";
}
