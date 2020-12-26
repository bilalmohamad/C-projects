#!/bin/bash

# Make a version of the program that measures code coverage.
make clean
make deque-cov
if [ $? -ne 0 ] || [ ! -f deque-cov ]; then
  echo "**** Couldn't compile with coverage support."
  exit 1
fi

# Run the program with every input file that has the right
# filename pattern.
for f in input-*.txt
do
  echo "Running with input: $f"
  ./deque-cov < $f >/dev/null
done

gcov list.c
gcov command.c
gcov deque.c
