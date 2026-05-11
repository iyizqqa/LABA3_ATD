#include <iostream>

#include "Tests.h"
#include "LinearForm.h"
#include "LinearFormTests.h"
#include "MutableArraySequence.h"

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

        int storageChoice;

        std::cout << "Storage type: 1 - ArraySequence, 2 - ListSequence: ";
        std::cin >> storageChoice;

        SequenceStorageType storageType = SequenceStorageType::Array;

        if (storageChoice == 2)
        {
            storageType = SequenceStorageType::List;
        }

        LinearForm<double> *form = new LinearForm<double>(
            coefficients,
            coefficientCount,
            storageType);

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
    LinearForm<double> *form = new LinearForm<double>();

    bool running = true;

    while (running)
    {
        std::cout << "\nLinearForm<double> menu\n"
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
                PrintLinearForm(*form);
            }
            else if (command == 3)
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
            else if (command == 4)
            {
                std::cout << "Enter second form:\n";

                LinearForm<double> *other = ReadLinearFormDouble();
                LinearForm<double> *result = form->Add(*other);

                std::cout << "Result: ";
                PrintLinearForm(*result);

                delete other;
                delete result;
            }
            else if (command == 5)
            {
                std::cout << "Enter second form:\n";

                LinearForm<double> *other = ReadLinearFormDouble();
                LinearForm<double> *result = form->Subtract(*other);

                std::cout << "Result: ";
                PrintLinearForm(*result);

                delete other;
                delete result;
            }
            else if (command == 6)
            {
                double scalar;

                std::cout << "Scalar: ";
                std::cin >> scalar;

                LinearForm<double> *result = form->MultiplyByScalar(scalar);

                delete form;
                form = result;
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

int main()
{
    bool running = true;

    while (running)
    {
        std::cout << "\nLab 3 UI\n"
                  << "1. Run tests\n"
                  << "2. Work with MutableArraySequence\n"
                  << "3. Work with MutableListSequence\n"
                  << "4. Run LinearForm tests\n"
                  << "5. Work with LinearForm<double>\n"
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