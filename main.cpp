#include <iostream>
#include <stdexcept>

#include "IEnumerator.h"
#include "LinearForm.h"
#include "LinearFormTests.h"
#include "MutableArraySequence.h"
#include "MutableListSequence.h"
#include "Sequence.h"
#include "Tests.h"

#include "Stack.h"
#include "Queue.h"
#include "Deque.h"
#include "LinearContainersTests.h"

void PrintSequence(const Sequence<int> &sequence)
{
    std::cout << "[";

    IEnumerator<int> *iterator = sequence.GetEnumerator();
    bool first = true;

    try
    {
        while (iterator->HasNext())
        {
            if (!first)
            {
                std::cout << ", ";
            }

            std::cout << iterator->Next();
            first = false;
        }

        delete iterator;
    }
    catch (...)
    {
        delete iterator;
        throw;
    }

    std::cout << "]\n";
}

void ArraySequenceMenu()
{
    MutableArraySequence<int> sequence;

    bool running = true;

    while (running)
    {
        std::cout << "\nMutableArraySequence menu\n"
                  << "1. Append\n"
                  << "2. Prepend\n"
                  << "3. InsertAt\n"
                  << "4. Print\n"
                  << "5. GetSubsequence\n"
                  << "6. Map x*x\n"
                  << "7. Where even\n"
                  << "8. Reduce sum\n"
                  << "0. Back\n"
                  << "Choose: ";

        int command;

        if (!(std::cin >> command))
        {
            return;
        }

        try
        {
            if (command == 1)
            {
                int value;

                std::cout << "Value: ";
                std::cin >> value;

                sequence.Append(value);
            }
            else if (command == 2)
            {
                int value;

                std::cout << "Value: ";
                std::cin >> value;

                sequence.Prepend(value);
            }
            else if (command == 3)
            {
                int value;
                int index;

                std::cout << "Value and index: ";
                std::cin >> value >> index;

                sequence.InsertAt(value, index);
            }
            else if (command == 4)
            {
                PrintSequence(sequence);
            }
            else if (command == 5)
            {
                int startIndex;
                int endIndex;

                std::cout << "Start and end index: ";
                std::cin >> startIndex >> endIndex;

                Sequence<int> *sub = sequence.GetSubsequence(startIndex, endIndex);

                PrintSequence(*sub);

                delete sub;
            }
            else if (command == 6)
            {
                Sequence<int> *mapped = sequence.Map([](const int &value)
                                                     { return value * value; });

                PrintSequence(*mapped);

                delete mapped;
            }
            else if (command == 7)
            {
                Sequence<int> *filtered = sequence.Where([](const int &value)
                                                         { return value % 2 == 0; });

                PrintSequence(*filtered);

                delete filtered;
            }
            else if (command == 8)
            {
                int result = sequence.Reduce(
                    [](const int &left, const int &right)
                    {
                        return left + right;
                    },
                    0);

                std::cout << result << "\n";
            }
            else if (command == 0)
            {
                running = false;
            }
            else
            {
                std::cout << "Unknown command\n";
            }
        }
        catch (const std::exception &error)
        {
            std::cout << "Error: " << error.what() << "\n";
        }
    }
}

void ListSequenceMenu()
{
    MutableListSequence<int> sequence;

    bool running = true;

    while (running)
    {
        std::cout << "\nMutableListSequence menu\n"
                  << "1. Append\n"
                  << "2. Prepend\n"
                  << "3. InsertAt\n"
                  << "4. Print\n"
                  << "5. GetSubsequence\n"
                  << "6. Map x*x\n"
                  << "7. Where even\n"
                  << "8. Reduce sum\n"
                  << "0. Back\n"
                  << "Choose: ";

        int command;

        if (!(std::cin >> command))
        {
            return;
        }

        try
        {
            if (command == 1)
            {
                int value;

                std::cout << "Value: ";
                std::cin >> value;

                sequence.Append(value);
            }
            else if (command == 2)
            {
                int value;

                std::cout << "Value: ";
                std::cin >> value;

                sequence.Prepend(value);
            }
            else if (command == 3)
            {
                int value;
                int index;

                std::cout << "Value and index: ";
                std::cin >> value >> index;

                sequence.InsertAt(value, index);
            }
            else if (command == 4)
            {
                PrintSequence(sequence);
            }
            else if (command == 5)
            {
                int startIndex;
                int endIndex;

                std::cout << "Start and end index: ";
                std::cin >> startIndex >> endIndex;

                Sequence<int> *sub = sequence.GetSubsequence(startIndex, endIndex);

                PrintSequence(*sub);

                delete sub;
            }
            else if (command == 6)
            {
                Sequence<int> *mapped = sequence.Map([](const int &value)
                                                     { return value * value; });

                PrintSequence(*mapped);

                delete mapped;
            }
            else if (command == 7)
            {
                Sequence<int> *filtered = sequence.Where([](const int &value)
                                                         { return value % 2 == 0; });

                PrintSequence(*filtered);

                delete filtered;
            }
            else if (command == 8)
            {
                int result = sequence.Reduce(
                    [](const int &left, const int &right)
                    {
                        return left + right;
                    },
                    0);

                std::cout << result << "\n";
            }
            else if (command == 0)
            {
                running = false;
            }
            else
            {
                std::cout << "Unknown command\n";
            }
        }
        catch (const std::exception &error)
        {
            std::cout << "Error: " << error.what() << "\n";
        }
    }
}

void PrintLinearForm(const LinearForm<double> &form)
{
    std::cout << form.GetCoefficient(0);

    for (int i = 1; i < form.GetCoefficientCount(); ++i)
    {
        std::cout << " + " << form.GetCoefficient(i) << "*x" << i;
    }

    std::cout << "\n";
}

LinearForm<double> *ReadLinearFormDouble()
{
    int coefficientCount;

    std::cout << "Enter coefficient count. For a0 + a1*x1 + ... + an*xn enter n + 1: ";
    std::cin >> coefficientCount;

    if (coefficientCount <= 0)
    {
        throw std::invalid_argument("Coefficient count must be positive");
    }

    double *coefficients = new double[coefficientCount];

    try
    {
        for (int i = 0; i < coefficientCount; ++i)
        {
            std::cout << "a" << i << ": ";
            std::cin >> coefficients[i];
        }

        int sourceChoice;

        std::cout << "Source sequence type: 1 - ArraySequence, 2 - ListSequence: ";
        std::cin >> sourceChoice;

        LinearForm<double> *form = nullptr;

        if (sourceChoice == 2)
        {
            MutableListSequence<double> source(coefficients, coefficientCount);
            form = new LinearForm<double>(&source);
        }
        else
        {
            MutableArraySequence<double> source(coefficients, coefficientCount);
            form = new LinearForm<double>(&source);
        }

        delete[] coefficients;

        return form;
    }
    catch (...)
    {
        delete[] coefficients;
        throw;
    }
}

void LinearFormMenu()
{
    LinearForm<double> *form = nullptr;

    bool running = true;

    while (running)
    {
        std::cout << "\nLinear form menu\n"
                  << "1. Create form\n"
                  << "2. Print form\n"
                  << "3. Evaluate form\n"
                  << "4. Add another form\n"
                  << "5. Subtract another form\n"
                  << "6. Multiply by scalar\n"
                  << "0. Back\n"
                  << "Choose: ";

        int command;

        if (!(std::cin >> command))
        {
            delete form;
            return;
        }

        try
        {
            if (command == 1)
            {
                LinearForm<double> *newForm = ReadLinearFormDouble();

                delete form;
                form = newForm;
            }
            else if (command == 2)
            {
                if (form == nullptr)
                {
                    std::cout << "Create form first\n";
                }
                else
                {
                    PrintLinearForm(*form);
                }
            }
            else if (command == 3)
            {
                if (form == nullptr)
                {
                    std::cout << "Create form first\n";
                }
                else
                {
                    int variableCount = form->GetVariableCount();
                    double *argumentsArray = new double[variableCount];

                    try
                    {
                        for (int i = 0; i < variableCount; ++i)
                        {
                            std::cout << "x" << (i + 1) << ": ";
                            std::cin >> argumentsArray[i];
                        }

                        MutableArraySequence<double> arguments(argumentsArray, variableCount);

                        double result = form->Evaluate(arguments);

                        delete[] argumentsArray;

                        std::cout << "Result: " << result << "\n";
                    }
                    catch (...)
                    {
                        delete[] argumentsArray;
                        throw;
                    }
                }
            }
            else if (command == 4)
            {
                if (form == nullptr)
                {
                    std::cout << "Create form first\n";
                }
                else
                {
                    std::cout << "Enter second form:\n";

                    LinearForm<double> *other = ReadLinearFormDouble();
                    LinearForm<double> *result = form->Add(*other);

                    std::cout << "Result: ";
                    PrintLinearForm(*result);

                    delete other;
                    delete result;
                }
            }
            else if (command == 5)
            {
                if (form == nullptr)
                {
                    std::cout << "Create form first\n";
                }
                else
                {
                    std::cout << "Enter second form:\n";

                    LinearForm<double> *other = ReadLinearFormDouble();
                    LinearForm<double> *result = form->Subtract(*other);

                    std::cout << "Result: ";
                    PrintLinearForm(*result);

                    delete other;
                    delete result;
                }
            }
            else if (command == 6)
            {
                if (form == nullptr)
                {
                    std::cout << "Create form first\n";
                }
                else
                {
                    double scalar;

                    std::cout << "Scalar: ";
                    std::cin >> scalar;

                    LinearForm<double> *result = form->MultiplyByScalar(scalar);

                    delete form;
                    form = result;
                }
            }
            else if (command == 0)
            {
                running = false;
            }
            else
            {
                std::cout << "Unknown command\n";
            }
        }
        catch (const std::exception &error)
        {
            std::cout << "Error: " << error.what() << "\n";
        }
    }

    delete form;
}

void StackMenu()
{
    Stack<int> stack;

    bool running = true;

    while (running)
    {
        std::cout << "\nStack menu\n"
                  << "1. Push\n"
                  << "2. Pop\n"
                  << "3. Peek\n"
                  << "4. Count\n"
                  << "5. Is empty\n"
                  << "0. Back\n"
                  << "Choose: ";

        int command;

        if (!(std::cin >> command))
        {
            return;
        }

        try
        {
            if (command == 1)
            {
                int value;

                std::cout << "Value: ";
                std::cin >> value;

                stack.Push(value);
            }
            else if (command == 2)
            {
                std::cout << "Popped: " << stack.Pop() << "\n";
            }
            else if (command == 3)
            {
                std::cout << "Top: " << stack.Peek() << "\n";
            }
            else if (command == 4)
            {
                std::cout << "Count: " << stack.GetCount() << "\n";
            }
            else if (command == 5)
            {
                std::cout << (stack.IsEmpty() ? "Empty\n" : "Not empty\n");
            }
            else if (command == 0)
            {
                running = false;
            }
            else
            {
                std::cout << "Unknown command\n";
            }
        }
        catch (const std::exception &error)
        {
            std::cout << "Error: " << error.what() << "\n";
        }
    }
}

void QueueMenu()
{
    Queue<int> queue;

    bool running = true;

    while (running)
    {
        std::cout << "\nQueue menu\n"
                  << "1. Enqueue\n"
                  << "2. Dequeue\n"
                  << "3. Peek\n"
                  << "4. Count\n"
                  << "5. Is empty\n"
                  << "0. Back\n"
                  << "Choose: ";

        int command;

        if (!(std::cin >> command))
        {
            return;
        }

        try
        {
            if (command == 1)
            {
                int value;

                std::cout << "Value: ";
                std::cin >> value;

                queue.Enqueue(value);
            }
            else if (command == 2)
            {
                std::cout << "Dequeued: " << queue.Dequeue() << "\n";
            }
            else if (command == 3)
            {
                std::cout << "Front: " << queue.Peek() << "\n";
            }
            else if (command == 4)
            {
                std::cout << "Count: " << queue.GetCount() << "\n";
            }
            else if (command == 5)
            {
                std::cout << (queue.IsEmpty() ? "Empty\n" : "Not empty\n");
            }
            else if (command == 0)
            {
                running = false;
            }
            else
            {
                std::cout << "Unknown command\n";
            }
        }
        catch (const std::exception &error)
        {
            std::cout << "Error: " << error.what() << "\n";
        }
    }
}

void DequeMenu()
{
    Deque<int> deque;

    bool running = true;

    while (running)
    {
        std::cout << "\nDeque menu\n"
                  << "1. Push front\n"
                  << "2. Push back\n"
                  << "3. Pop front\n"
                  << "4. Pop back\n"
                  << "5. Peek front\n"
                  << "6. Peek back\n"
                  << "7. Count\n"
                  << "8. Is empty\n"
                  << "0. Back\n"
                  << "Choose: ";

        int command;

        if (!(std::cin >> command))
        {
            return;
        }

        try
        {
            if (command == 1)
            {
                int value;

                std::cout << "Value: ";
                std::cin >> value;

                deque.PushFront(value);
            }
            else if (command == 2)
            {
                int value;

                std::cout << "Value: ";
                std::cin >> value;

                deque.PushBack(value);
            }
            else if (command == 3)
            {
                std::cout << "Popped front: " << deque.PopFront() << "\n";
            }
            else if (command == 4)
            {
                std::cout << "Popped back: " << deque.PopBack() << "\n";
            }
            else if (command == 5)
            {
                std::cout << "Front: " << deque.PeekFront() << "\n";
            }
            else if (command == 6)
            {
                std::cout << "Back: " << deque.PeekBack() << "\n";
            }
            else if (command == 7)
            {
                std::cout << "Count: " << deque.GetCount() << "\n";
            }
            else if (command == 8)
            {
                std::cout << (deque.IsEmpty() ? "Empty\n" : "Not empty\n");
            }
            else if (command == 0)
            {
                running = false;
            }
            else
            {
                std::cout << "Unknown command\n";
            }
        }
        catch (const std::exception &error)
        {
            std::cout << "Error: " << error.what() << "\n";
        }
    }
}

int main()
{
    bool running = true;

    while (running)
    {
        std::cout << "\nLab 3 UI\n"
                  << "1. Run Lab 2 tests\n"
                  << "2. Work with MutableArraySequence\n"
                  << "3. Work with MutableListSequence\n"
                  << "4. Run LinearForm tests\n"
                  << "5. Work with linear form\n"
                  << "6. Run Stack/Queue/Deque tests\n"
                  << "7. Work with stack\n"
                  << "8. Work with queue\n"
                  << "9. Work with deque\n"
                  << "0. Exit\n"
                  << "Choose: ";

        int command;

        if (!(std::cin >> command))
        {
            return 0;
        }

        try
        {
            if (command == 1)
            {
                RunAllTests();
            }
            else if (command == 2)
            {
                ArraySequenceMenu();
            }
            else if (command == 3)
            {
                ListSequenceMenu();
            }
            else if (command == 4)
            {
                RunLinearFormTests();
            }
            else if (command == 5)
            {
                LinearFormMenu();
            }
            else if (command == 6)
            {
                RunLinearContainersTests();
            }
            else if (command == 7)
            {
                StackMenu();
            }
            else if (command == 8)
            {
                QueueMenu();
            }
            else if (command == 9)
            {
                DequeMenu();
            }
            else if (command == 0)
            {
                running = false;
            }
            else
            {
                std::cout << "Unknown command\n";
            }
        }
        catch (const std::exception &error)
        {
            std::cout << "Error: " << error.what() << "\n";
        }
    }

    return 0;
}