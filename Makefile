all:
	g++ -std=c++17 -Wall -Wextra -Werror main.cpp Tests.cpp LinearFormTests.cpp LinearContainersTests.cpp -o lab3

run:
	./lab3

tests:
	echo "1\n4\n6\n0\n" | ./lab3

clean:
	rm -f lab3
