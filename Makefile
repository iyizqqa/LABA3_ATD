all: program

program: main.o
	g++ main.o -o lab3

main.o: main.cpp
	g++ -std=c++17 -Wall -Wextra -Werror main.cpp -c

tests: test_dynamic_array.o test_linked_list.o test_sequences.o test_linear_form.o test_stack_deque_queue.o
	g++ test_dynamic_array.o test_linked_list.o test_sequences.o test_linear_form.o test_stack_deque_queue.o -o lab3_tests -L/opt/homebrew/lib -lgtest -lgtest_main -pthread

test_dynamic_array.o: tests/test_dynamic_array.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -I/opt/homebrew/include tests/test_dynamic_array.cpp -c

test_linked_list.o: tests/test_linked_list.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -I/opt/homebrew/include tests/test_linked_list.cpp -c

test_sequences.o: tests/test_sequences.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -I/opt/homebrew/include tests/test_sequences.cpp -c

test_linear_form.o: tests/test_linear_form.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -I/opt/homebrew/include tests/test_linear_form.cpp -c

test_stack_deque_queue.o: tests/test_stack_deque_queue.cpp
	g++ -std=c++17 -Wall -Wextra -Werror -I/opt/homebrew/include tests/test_stack_deque_queue.cpp -c

run:
	./lab3

test:
	./lab3_tests

clean:
	rm -f lab3 lab3_tests *.o