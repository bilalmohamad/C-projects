CFLAGS = -g -Wall -std=c99

deque: deque.o command.o list.o

deque.o: deque.c command.h list.h

deque.o: command.c command.h

list.o: list.c list.h

# Build a target executable with support for code coverage.
# (not as efficient as how we build the regular executable)
deque-cov: deque.c command.c command.h list.c list.h
	gcc -Wall -std=c99 -g -fprofile-arcs -ftest-coverage deque.c command.c list.c -o deque-cov

# Remove all temporary files
clean:
	rm -f *.o
	rm -f *.gcda *.gcno *.gcov
	rm -f deque deque-cov
