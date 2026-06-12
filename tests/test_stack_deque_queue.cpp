#include <gtest/gtest.h>

#include "../Deque.h"
#include "../Queue.h"
#include "../Stack.h"

TEST(StackTests, NewStackIsEmpty)
{
    Stack<int> stack;

    EXPECT_TRUE(stack.IsEmpty());
    EXPECT_EQ(stack.GetCount(), 0);
}

TEST(StackTests, PushPopUsesLifo)
{
    Stack<int> stack;

    stack.Push(1);
    stack.Push(2);
    stack.Push(3);

    EXPECT_FALSE(stack.IsEmpty());
    EXPECT_EQ(stack.GetCount(), 3);
    EXPECT_EQ(stack.Peek(), 3);

    EXPECT_EQ(stack.Pop(), 3);
    EXPECT_EQ(stack.Pop(), 2);
    EXPECT_EQ(stack.Pop(), 1);

    EXPECT_TRUE(stack.IsEmpty());
    EXPECT_EQ(stack.GetCount(), 0);
}

TEST(StackTests, InterleavedPushPop)
{
    Stack<int> stack;

    stack.Push(1);
    stack.Push(2);

    EXPECT_EQ(stack.Pop(), 2);

    stack.Push(3);

    EXPECT_EQ(stack.Pop(), 3);
    EXPECT_EQ(stack.Pop(), 1);
    EXPECT_TRUE(stack.IsEmpty());
}

TEST(StackTests, ThrowsWhenEmpty)
{
    Stack<int> stack;

    EXPECT_THROW(stack.Pop(), std::out_of_range);
    EXPECT_THROW(stack.Peek(), std::out_of_range);
}

TEST(QueueTests, NewQueueIsEmpty)
{
    Queue<int> queue;

    EXPECT_TRUE(queue.IsEmpty());
    EXPECT_EQ(queue.GetCount(), 0);
}

TEST(QueueTests, EnqueueDequeueUsesFifo)
{
    Queue<int> queue;

    queue.Enqueue(1);
    queue.Enqueue(2);
    queue.Enqueue(3);

    EXPECT_FALSE(queue.IsEmpty());
    EXPECT_EQ(queue.GetCount(), 3);
    EXPECT_EQ(queue.Peek(), 1);

    EXPECT_EQ(queue.Dequeue(), 1);
    EXPECT_EQ(queue.Dequeue(), 2);
    EXPECT_EQ(queue.Dequeue(), 3);

    EXPECT_TRUE(queue.IsEmpty());
    EXPECT_EQ(queue.GetCount(), 0);
}

TEST(QueueTests, InterleavedEnqueueDequeue)
{
    Queue<int> queue;

    queue.Enqueue(1);
    queue.Enqueue(2);

    EXPECT_EQ(queue.Dequeue(), 1);

    queue.Enqueue(3);

    EXPECT_EQ(queue.Dequeue(), 2);
    EXPECT_EQ(queue.Dequeue(), 3);
    EXPECT_TRUE(queue.IsEmpty());
}

TEST(QueueTests, ThrowsWhenEmpty)
{
    Queue<int> queue;

    EXPECT_THROW(queue.Dequeue(), std::out_of_range);
    EXPECT_THROW(queue.Peek(), std::out_of_range);
}

TEST(DequeTests, NewDequeIsEmpty)
{
    Deque<int> deque;

    EXPECT_TRUE(deque.IsEmpty());
    EXPECT_EQ(deque.GetCount(), 0);
}

TEST(DequeTests, PushPopBothEnds)
{
    Deque<int> deque;

    deque.PushBack(2);
    deque.PushFront(1);
    deque.PushBack(3);
    deque.PushFront(0);

    EXPECT_FALSE(deque.IsEmpty());
    EXPECT_EQ(deque.GetCount(), 4);
    EXPECT_EQ(deque.PeekFront(), 0);
    EXPECT_EQ(deque.PeekBack(), 3);

    EXPECT_EQ(deque.PopFront(), 0);
    EXPECT_EQ(deque.PopBack(), 3);
    EXPECT_EQ(deque.PopFront(), 1);
    EXPECT_EQ(deque.PopBack(), 2);

    EXPECT_TRUE(deque.IsEmpty());
    EXPECT_EQ(deque.GetCount(), 0);
}

TEST(DequeTests, InterleavedOperations)
{
    Deque<int> deque;

    deque.PushBack(1);
    deque.PushFront(0);
    deque.PushBack(2);

    EXPECT_EQ(deque.PopBack(), 2);

    deque.PushFront(-1);

    EXPECT_EQ(deque.PopFront(), -1);
    EXPECT_EQ(deque.PopFront(), 0);
    EXPECT_EQ(deque.PopBack(), 1);
    EXPECT_TRUE(deque.IsEmpty());
}

TEST(DequeTests, ThrowsWhenEmpty)
{
    Deque<int> deque;

    EXPECT_THROW(deque.PopFront(), std::out_of_range);
    EXPECT_THROW(deque.PopBack(), std::out_of_range);
    EXPECT_THROW(deque.PeekFront(), std::out_of_range);
    EXPECT_THROW(deque.PeekBack(), std::out_of_range);
}