#pragma once

#include <stdexcept>

#include "IEnumerable.h"

template <class T>
class LinkedList : public IEnumerable<T>
{
private:
    struct Node
    {
        T value;
        Node *next;

        explicit Node(const T &value, Node *next = nullptr)
            : value(value), next(next) {}
    };

    Node *head_ = nullptr;
    Node *tail_ = nullptr;
    int length_ = 0;

    void CheckNotEmpty() const
    {
        if (length_ == 0)
        {
            throw std::out_of_range("LinkedList: list is empty");
        }
    }

    void CheckIndex(int index) const
    {
        if (index < 0 || index >= length_)
        {
            throw std::out_of_range("LinkedList: index out of range");
        }
    }

    Node *GetNode(int index)
    {
        CheckIndex(index);

        Node *current = head_;

        for (int i = 0; i < index; ++i)
        {
            current = current->next;
        }

        return current;
    }

    const Node *GetNode(int index) const
    {
        CheckIndex(index);

        const Node *current = head_;

        for (int i = 0; i < index; ++i)
        {
            current = current->next;
        }

        return current;
    }

    class LinkedListEnumerator final : public IEnumerator<T>
    {
    private:
        const LinkedList<T> &list_;
        const Node *current_;

    public:
        explicit LinkedListEnumerator(const LinkedList<T> &list)
            : list_(list), current_(list.head_) {}

        bool HasNext() const override
        {
            return current_ != nullptr;
        }

        const T &Next() override
        {
            if (current_ == nullptr)
            {
                throw std::out_of_range("LinkedListEnumerator: no next item");
            }

            const T &value = current_->value;
            current_ = current_->next;

            return value;
        }

        void Reset() override
        {
            current_ = list_.head_;
        }
    };

public:
    LinkedList() = default;

    LinkedList(const T *items, int count)
    {
        if (count < 0)
        {
            throw std::invalid_argument("LinkedList: negative count");
        }

        if (count > 0 && items == nullptr)
        {
            throw std::invalid_argument("LinkedList: null items");
        }

        for (int i = 0; i < count; ++i)
        {
            Append(items[i]);
        }
    }

    LinkedList(const LinkedList<T> &other)
    {
        IEnumerator<T> *iterator = other.GetEnumerator();

        try
        {
            while (iterator->HasNext())
            {
                Append(iterator->Next());
            }

            delete iterator;
        }
        catch (...)
        {
            delete iterator;
            Clear();
            throw;
        }
    }

    LinkedList<T> &operator=(const LinkedList<T> &other)
    {
        if (this == &other)
        {
            return *this;
        }

        LinkedList<T> copy(other);
        Swap(copy);

        return *this;
    }

    ~LinkedList()
    {
        Clear();
    }

    void Swap(LinkedList<T> &other)
    {
        Node *tempHead = head_;
        head_ = other.head_;
        other.head_ = tempHead;

        Node *tempTail = tail_;
        tail_ = other.tail_;
        other.tail_ = tempTail;

        int tempLength = length_;
        length_ = other.length_;
        other.length_ = tempLength;
    }

    void Clear()
    {
        Node *current = head_;

        while (current != nullptr)
        {
            Node *next = current->next;
            delete current;
            current = next;
        }

        head_ = nullptr;
        tail_ = nullptr;
        length_ = 0;
    }

    int GetLength() const
    {
        return length_;
    }

    const T &GetFirst() const
    {
        CheckNotEmpty();
        return head_->value;
    }

    const T &GetLast() const
    {
        CheckNotEmpty();
        return tail_->value;
    }

    const T &Get(int index) const
    {
        return GetNode(index)->value;
    }

    void Append(const T &item)
    {
        Node *node = new Node(item);

        if (length_ == 0)
        {
            head_ = node;
            tail_ = node;
        }
        else
        {
            tail_->next = node;
            tail_ = node;
        }

        ++length_;
    }

    void Prepend(const T &item)
    {
        Node *node = new Node(item, head_);

        head_ = node;

        if (length_ == 0)
        {
            tail_ = node;
        }

        ++length_;
    }

    void InsertAt(const T &item, int index)
    {
        if (index < 0 || index > length_)
        {
            throw std::out_of_range("LinkedList: insert index out of range");
        }

        if (index == 0)
        {
            Prepend(item);
            return;
        }

        if (index == length_)
        {
            Append(item);
            return;
        }

        Node *previous = GetNode(index - 1);
        previous->next = new Node(item, previous->next);

        ++length_;
    }

    LinkedList<T> *GetSubList(int startIndex, int endIndex) const
    {
        if (startIndex < 0 ||
            endIndex < 0 ||
            startIndex >= length_ ||
            endIndex >= length_ ||
            startIndex > endIndex)
        {
            throw std::out_of_range("LinkedList: sublist indices out of range");
        }

        LinkedList<T> *result = new LinkedList<T>();
        const Node *current = GetNode(startIndex);

        try
        {
            for (int i = startIndex; i <= endIndex; ++i)
            {
                result->Append(current->value);
                current = current->next;
            }
        }
        catch (...)
        {
            delete result;
            throw;
        }

        return result;
    }

    LinkedList<T> *Concat(const LinkedList<T> &other) const
    {
        LinkedList<T> *result = new LinkedList<T>(*this);
        IEnumerator<T> *iterator = other.GetEnumerator();

        try
        {
            while (iterator->HasNext())
            {
                result->Append(iterator->Next());
            }

            delete iterator;
        }
        catch (...)
        {
            delete iterator;
            delete result;
            throw;
        }

        return result;
    }

    IEnumerator<T> *GetEnumerator() const override
    {
        return new LinkedListEnumerator(*this);
    }
};