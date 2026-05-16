#include "LinearContainersTests.h"

#include <iostream>
#include <stdexcept>
#include <string>

#include "Deque.h"
#include "Queue.h"
#include "Stack.h"

class LinearContainersTestFailure : public std::runtime_error
{
public:
    explicit LinearContainersTestFailure(const std::string &message)
        : std::runtime_error(message) {}
};

static void AssertLinearContainers(bool condition, const std::string &message)
{
    if (!condition)
    {
        throw LinearContainersTestFailure(message);
    }
}

template <class ExceptionType, class Func>
static void AssertLinearContainersThrows(Func func, const std::string &message)
{
    try
    {
        func();
    }
    catch (const ExceptionType &)
    {
        return;
    }

    throw LinearContainersTestFailure(message);
}

static void TestStackPushPop()
{
    Stack<int> stack;

    AssertLinearContainers(stack.IsEmpty(), "Stack: new stack must be empty");
    AssertLinearContainers(stack.GetCount() == 0, "Stack: new stack count must be zero");

    stack.Push(1);
    stack.Push(2);
    stack.Push(3);

    AssertLinearContainers(!stack.IsEmpty(), "Stack: stack must not be empty after push");
    AssertLinearContainers(stack.GetCount() == 3, "Stack: count after push failed");
    AssertLinearContainers(stack.Peek() == 3, "Stack: peek failed");

    AssertLinearContainers(stack.Pop() == 3, "Stack: first pop failed");
    AssertLinearContainers(stack.Pop() == 2, "Stack: second pop failed");
    AssertLinearContainers(stack.Pop() == 1, "Stack: third pop failed");

    AssertLinearContainers(stack.IsEmpty(), "Stack: stack must be empty after pops");
}

static void TestStackEmpty()
{
    Stack<int> stack;

    AssertLinearContainersThrows<std::out_of_range>(
        [&stack]()
        {
            stack.Pop();
        },
        "Stack: pop from empty stack must throw");

    AssertLinearContainersThrows<std::out_of_range>(
        [&stack]()
        {
            stack.Peek();
        },
        "Stack: peek from empty stack must throw");
}

static void TestQueueEnqueueDequeue()
{
    Queue<int> queue;

    AssertLinearContainers(queue.IsEmpty(), "Queue: new queue must be empty");
    AssertLinearContainers(queue.GetCount() == 0, "Queue: new queue count must be zero");

    queue.Enqueue(1);
    queue.Enqueue(2);
    queue.Enqueue(3);

    AssertLinearContainers(!queue.IsEmpty(), "Queue: queue must not be empty after enqueue");
    AssertLinearContainers(queue.GetCount() == 3, "Queue: count after enqueue failed");
    AssertLinearContainers(queue.Peek() == 1, "Queue: peek failed");

    AssertLinearContainers(queue.Dequeue() == 1, "Queue: first dequeue failed");
    AssertLinearContainers(queue.Dequeue() == 2, "Queue: second dequeue failed");
    AssertLinearContainers(queue.Dequeue() == 3, "Queue: third dequeue failed");

    AssertLinearContainers(queue.IsEmpty(), "Queue: queue must be empty after dequeues");
}

static void TestQueueEmpty()
{
    Queue<int> queue;

    AssertLinearContainersThrows<std::out_of_range>(
        [&queue]()
        {
            queue.Dequeue();
        },
        "Queue: dequeue from empty queue must throw");

    AssertLinearContainersThrows<std::out_of_range>(
        [&queue]()
        {
            queue.Peek();
        },
        "Queue: peek from empty queue must throw");
}

static void TestDequePushPop()
{
    Deque<int> deque;

    AssertLinearContainers(deque.IsEmpty(), "Deque: new deque must be empty");
    AssertLinearContainers(deque.GetCount() == 0, "Deque: new deque count must be zero");

    deque.PushBack(2);
    deque.PushFront(1);
    deque.PushBack(3);
    deque.PushFront(0);

    AssertLinearContainers(deque.GetCount() == 4, "Deque: count after pushes failed");
    AssertLinearContainers(deque.PeekFront() == 0, "Deque: peek front failed");
    AssertLinearContainers(deque.PeekBack() == 3, "Deque: peek back failed");

    AssertLinearContainers(deque.PopFront() == 0, "Deque: pop front failed");
    AssertLinearContainers(deque.PopBack() == 3, "Deque: pop back failed");
    AssertLinearContainers(deque.PopFront() == 1, "Deque: second pop front failed");
    AssertLinearContainers(deque.PopBack() == 2, "Deque: second pop back failed");

    AssertLinearContainers(deque.IsEmpty(), "Deque: deque must be empty after pops");
}

static void TestDequeEmpty()
{
    Deque<int> deque;

    AssertLinearContainersThrows<std::out_of_range>(
        [&deque]()
        {
            deque.PopFront();
        },
        "Deque: pop front from empty deque must throw");

    AssertLinearContainersThrows<std::out_of_range>(
        [&deque]()
        {
            deque.PopBack();
        },
        "Deque: pop back from empty deque must throw");

    AssertLinearContainersThrows<std::out_of_range>(
        [&deque]()
        {
            deque.PeekFront();
        },
        "Deque: peek front from empty deque must throw");

    AssertLinearContainersThrows<std::out_of_range>(
        [&deque]()
        {
            deque.PeekBack();
        },
        "Deque: peek back from empty deque must throw");
}

void RunLinearContainersTests()
{
    TestStackPushPop();
    TestStackEmpty();

    TestQueueEnqueueDequeue();
    TestQueueEmpty();

    TestDequePushPop();
    TestDequeEmpty();

    std::cout << "All Stack, Queue and Deque tests passed successfully.\n";
}
