#!/bin/bash
FAIL=0

# make a fresh copy of the target program
make clean
make
if [ $? -ne 0 ]; then
  echo "**** Make (compilation) FAILED"
  FAIL=1
fi

# Function to run the program against a (successful) test case.
runtest() {
  TESTNO=$1

  rm -f output.txt

  echo "Test $TEST_NO: ./deque < input-$TESTNO.txt > output.txt"
  ./deque < input-$TESTNO.txt > output.txt
  STATUS=$?

  # Make sure the program exited successfully (it always should)
  if [ $STATUS -ne 0 ]; then
      echo "**** Test failed - incorrect exit status. Expected: 0 Got: $STATUS"
      FAIL=1
      return 1
  fi

  # Make sure the output matches the expected output.
  diff -q expected-$TESTNO.txt output.txt >/dev/null 2>&1
  if [ $? -ne 0 ]; then
      echo "**** Test FAILED - output doesn't match expected"
      FAIL=1
      return 1
  fi

  echo "Test $TESTNO PASS"
  return 0
}

# Run successfule test cases
runtest 01
runtest 02
runtest 03
runtest 04
runtest 05
runtest 06
runtest 07
runtest 08
runtest 09
runtest 10
runtest 11
runtest 12
runtest 13

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  exit 0
fi
