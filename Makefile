all:
	g++ -std=c++17 -Wall -Wextra -Werror main.cpp Tests.cpp LinearFormTests.cpp -o lab3

run:
	./lab3

tests:
	echo "1" | ./lab3

clean:
	rm -f lab3
