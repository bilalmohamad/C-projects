#!/bin/bash
FAIL=0

# Function to run the program against a test case and check
# its output and exit status for correct behavior
test_fwsim() {
  TESTNO=$1

  rm -f output.txt

  if [ $TESTNO -ge 12 ]; then
      echo "Test $TESTNO: ./fwsim -r rules-$TESTNO.txt < input-$TESTNO.txt > output.txt 2>&1"
      ./fwsim -r rules-$TESTNO.txt < input-$TESTNO.txt > output.txt 2>&1
  else
      echo "Test $TESTNO: ./fwsim < input-$TESTNO.txt > output.txt 2>&1"
      ./fwsim < input-$TESTNO.txt > output.txt 2>&1
  fi
  STATUS=$?

  # Make sure the program exited successfully
  if [ $STATUS -ne 0 ]; then
      echo "**** Test $TESTNO FAILED - incorrect exit status"
      FAIL=1
      return 1
  fi
  
  # Make sure any output to standard out looks right.
  if ! diff -q expected-$TESTNO.txt output.txt >/dev/null 2>&1
  then
      echo "**** Test $TESTNO FAILED - output didn't match the expected output"
      FAIL=1
      return 1
  fi

  echo "Test $TESTNO PASS"
  return 0
}

# make a fresh copy of the target programs
make clean
make

if [ -x fwsim ] ; then
    test_fwsim 01
    test_fwsim 02
    test_fwsim 03
    test_fwsim 04
    test_fwsim 05
    test_fwsim 06
    test_fwsim 07
    test_fwsim 08
    test_fwsim 09
    test_fwsim 10
    test_fwsim 11
    test_fwsim 12
    test_fwsim 13
    test_fwsim 14
    test_fwsim 15
    test_fwsim 16
    test_fwsim 17
    test_fwsim 18
    test_fwsim 19
    test_fwsim 20
    test_fwsim 21
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
