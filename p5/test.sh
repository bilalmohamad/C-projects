#!/bin/bash
FAIL=0

# Function to make sure the program gave the right output or error message
# for each test case.
checkResults() {
  TESTNO="$1"
  STATUS="$2"
  ESTATUS="$3"

  # Make sure the program exited with the right exit status
  if [ $STATUS -ne $ESTATUS ]; then
      echo "**** Test $TESTNO FAILED - incorrect exit status"
      FAIL=1
      return 1
  fi
  
  # Make sure any output to standard out looks right.
  if [ -f expected-$TESTNO.txt ] &&
	 ! diff -q expected-$TESTNO.txt output.txt >/dev/null 2>&1
  then
      echo "**** Test $TESTNO FAILED - output didn't match the expected output"
      FAIL=1
      return 1
  fi

  # If the program was supposed to print an error message, make sure it was the
  # right one.
  if [ -f estderr-$TESTNO.txt ] &&
	 ! diff -q estderr-$TESTNO.txt stderr.txt >/dev/null 2>&1
  then
      echo "**** Test $TESTNO FAILED - error message didn't match the expected message"
      FAIL=1
      return 1
  fi

  echo "Test $TESTNO PASS"
  return 0
}

# Remove any old executables.
rm -f testdriver hash

# First, let's run the unit tests.
echo "Running unit tests"
make testdriver

if [ -x testdriver ]; then
    ./testdriver
    if [ $? -ne 0 ]; then
	echo "**** Your program didn't pass all the test driver tests."
	FAIL=1
    fi
else
    echo "**** We couldn't build your test driver, so we couldn't run the unit tests."
    FAIL=1
fi


# Then, let's run the acceptance tests.
make clean
make

if [ -x hash ] ; then
  echo "Test 1: ./hash input-1.txt > output.txt 2> stderr.txt"
  ./hash input-1.txt > output.txt 2> stderr.txt
  STATUS=$?
  checkResults 1 $STATUS 0

  echo "Test 2: ./hash input-2.txt > output.txt 2> stderr.txt"
  ./hash input-2.txt > output.txt 2> stderr.txt
  STATUS=$?
  checkResults 2 $STATUS 0

  echo "Test 3: ./hash input-3.txt > output.txt 2> stderr.txt"
  ./hash input-3.txt > output.txt 2> stderr.txt
  STATUS=$?
  checkResults 3 $STATUS 0

  echo "Test 4: ./hash input-4.txt > output.txt 2> stderr.txt"
  ./hash input-4.txt > output.txt 2> stderr.txt
  STATUS=$?
  checkResults 4 $STATUS 0

  echo "Test 5: ./hash input-5.bin > output.txt 2> stderr.txt"
  ./hash input-5.bin > output.txt 2> stderr.txt
  STATUS=$?
  checkResults 5 $STATUS 0

  # Try bad command-line arguments.
  echo "Test 6: ./hash > output.txt 2> stderr.txt"
  ./hash > output.txt 2> stderr.txt
  STATUS=$?
  checkResults 6 $STATUS 1

  # Try an input file that doesn't exist.
  echo "Test 7: ./hash no-input-file.txt > output.txt 2> stderr.txt"
  ./hash no-input-file.txt > output.txt 2> stderr.txt
  STATUS=$?
  checkResults 7 $STATUS 1

  echo "Test 8: ./hash -hmac \"shortkey\" input-1.txt > output.txt 2> stderr.txt"
  ./hash -hmac "shortkey" input-1.txt > output.txt 2> stderr.txt
  STATUS=$?
  checkResults 8 $STATUS 0

  echo "Test 9: ./hash -hmac \"muchlongerkeythatneedssomemorememorybutnottoomuch\" input-2.txt > output.txt 2> stderr.txt"
  ./hash -hmac "muchlongerkeythatneedssomemorememorybutnottoomuch" input-2.txt > output.txt 2> stderr.txt
  STATUS=$?
  checkResults 9 $STATUS 0

  echo "Test 10: ./hash -hmac \"a key with some spaces\" input-3.txt > output.txt 2> stderr.txt"
  ./hash -hmac "a key with some spaces" input-3.txt > output.txt 2> stderr.txt
  STATUS=$?
  checkResults 10 $STATUS 0

  echo "Test 11: ./hash -hmac \"\" input-4.txt > output.txt 2> stderr.txt"
  ./hash -hmac "" input-4.txt > output.txt 2> stderr.txt
  STATUS=$?
  checkResults 11 $STATUS 0

  echo "Test 12: ./hash -hmac \"somekey\" input-5.bin > output.txt 2> stderr.txt"
  ./hash -hmac "somekey" input-5.bin > output.txt 2> stderr.txt
  STATUS=$?
  checkResults 12 $STATUS 0

else
    echo "**** Your program didn't compile successfully, so we couldn't test it."
    FAIL=1
fi

if [ $FAIL -ne 0 ]; then
  echo "FAILING TESTS!"
  exit 13
else 
  echo "Tests successful"
  exit 0
fi
