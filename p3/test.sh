#!/bin/bash
FAIL=0

# Function to check the output of the program after it's run, including checking
# an output file, standard output, standard error and exit status.
checkResults() {
  TESTNO=$1
  ESTATUS=$2
  STATUS=$3

  # Make sure the program exited with the right exit status.
  if [ $STATUS -ne $ESTATUS ]; then
      echo "**** Test $TESTNO FAILED - Expected an exit status of $ESTATUS, bug got: $STATUS"
      FAIL=1
      return 1
  fi

  # if there's an expected output file, make sure the program also has one
  # that matches.  If there's no expected output file, then the program
  # shouldn't write an output file.
  if [ -f expected-$TESTNO.txt ]; then
      # Make sure the output file contains correct output.
      if ! diff -q expected-$TESTNO.txt output.txt >/dev/null 2>&1 ; then
	  echo "**** Test $TESTNO FAILED - output file didn't match expected."
	  FAIL=1
	  return 1
      fi
  else
      if [ -f output.txt ]; then
	  echo "**** Test $TESTNO FAILED - the program wrote an output file but it shouldn't for this test case."
	  FAIL=1
	  return 1
      fi
  fi

  # Make sure the output to stdout matches expected.
  if ! diff -q estdout-$TESTNO.txt stdout.txt >/dev/null 2>&1 ; then
      echo "**** Test $TESTNO FAILED - output to stdout didn't match expected"
      FAIL=1
      return 1
  fi

  # Make sure the output to stderr matches expected.
  if ! diff -q estderr-$TESTNO.txt stderr.txt >/dev/null 2>&1 ; then
      echo "**** Test $TESTNO FAILED - output to stderr didn't match expected"
      FAIL=1
      return 1
  fi

  echo "Test $TESTNO PASS"
  return 0
}

# Try to get a fresh compile of the project.
make clean
make
if [ $? -ne 0 ]; then
    echo "**** Make didn't run succesfully when trying to build your program."
    FAIL=1
fi

# Run individual tests.  Since they each require different arguments, it didn't make
# as much sense to write a single test function for this.
if [ -x cnp ] ; then
    rm -f output.txt stdout.txt stderr.txt
    echo "Test 01: ./cnp input-a.txt output.txt > stdout.txt 2> stderr.txt"
    ./cnp input-a.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 01 0 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 02: ./cnp cut 26 1 input-a.txt output.txt > stdout.txt 2> stderr.txt"
    ./cnp cut 26 1 input-a.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 02 0 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 03: ./cnp cut 1 1 input-a.txt output.txt > stdout.txt 2> stderr.txt"
    ./cnp cut 1 1 input-a.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 03 0 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 04: ./cnp cut 2 24 input-a.txt output.txt > stdout.txt 2> stderr.txt"
    ./cnp cut 2 24 input-a.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 04 0 $STATUS
    
    rm -f output.txt stdout.txt stderr.txt
    echo "Test 05: ./cnp cut 1 1 paste 26 input-a.txt output.txt > stdout.txt 2> stderr.txt"
    ./cnp cut 1 1 paste 26 input-a.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 05 0 $STATUS
    
    rm -f output.txt stdout.txt stderr.txt
    echo "Test 06: ./cnp cut 9 10 paste 1 input-a.txt output.txt > stdout.txt 2> stderr.txt"
    ./cnp cut 9 10 paste 1 input-a.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 06 0 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 07: ./cnp copy 1 1 paste 27 input-a.txt output.txt > stdout.txt 2> stderr.txt"
    ./cnp copy 1 1 paste 27 input-a.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 07 0 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 08: ./cnp copy 1 26 paste 27 input-a.txt output.txt > stdout.txt 2> stderr.txt"
    ./cnp copy 1 26 paste 27 input-a.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 08 0 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 09: ./cnp cut 13 5 copy 33 7 paste 1 input-b.txt output.txt > stdout.txt 2> stderr.txt"
    ./cnp cut 13 5 copy 33 7 paste 1 input-b.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 09 0 $STATUS
    
    rm -f output.txt stdout.txt stderr.txt
    echo "Test 10: ./cnp cut 21 9 paste 1 paste 48 input-c.txt output.txt > stdout.txt 2> stderr.txt"
    ./cnp cut 21 9 paste 1 paste 48 input-c.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 10 0 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 11: ./cnp cut 2 6 input-d.txt output.txt > stdout.txt 2> stderr.txt"
    ./cnp cut 2 6 input-d.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 11 0 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 12: ./cnp cut 12 6 input-d.txt output.txt > stdout.txt 2> stderr.txt"
    ./cnp cut 12 6 input-d.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 12 1 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 13: ./cnp copy 1 1 paste 1 input-e.txt output.txt > stdout.txt 2> stderr.txt"
    ./cnp copy 1 1 paste 1 input-e.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 13 1 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 14: ./cnp input-f.txt output.txt > stdout.txt 2> stderr.txt"
    ./cnp input-f.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 14 1 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 15: ./cnp copy 18 4 paste 43 - output.txt < input-b.txt > stdout.txt 2> stderr.txt"
    ./cnp copy 18 4 paste 43 - output.txt < input-b.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 15 0 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 16: ./cnp cut 35 1 cut 40 1 cut 45 1 input-c.txt - > stdout.txt 2> stderr.txt"
    ./cnp cut 35 1 cut 40 1 cut 45 1 input-c.txt - > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 16 0 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 17: ./cnp copy 38 7 paste 1 paste 1 paste 1 paste 1 paste 1 paste 1 paste 1 paste 1 paste 1 input-b.txt output.txt > stdout.txt 2> stderr.txt"
    ./cnp copy 38 7 paste 1 paste 1 paste 1 paste 1 paste 1 paste 1 paste 1 paste 1 paste 1 input-b.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 17 1 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 18: ./cnp paste 10 input-c.txt output.txt > stdout.txt 2> stderr.txt"
    ./cnp paste 10 input-c.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 18 1 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 19: ./cnp copy 24 4 input-d.txt output.txt > stdout.txt 2> stderr.txt"
    ./cnp copy 24 4 input-d.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 19 1 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 20: ./cnp copy 1 1 pastel 2 input-b.txt output.txt > stdout.txt 2> stderr.txt"
    ./cnp copy 1 1 pastel 2 input-b.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 20 1 $STATUS

    rm -f output.txt stdout.txt stderr.txt
    echo "Test 21: ./cnp copy -5 3 paste 2 input-c.txt output.txt > stdout.txt 2> stderr.txt"
    ./cnp copy -5 3 paste 2 input-c.txt output.txt > stdout.txt 2> stderr.txt
    STATUS=$?
    checkResults 21 1 $STATUS

else
    echo "**** Your program couldn't be tested since it didn't compile successfully."
    FAIL=1

fi

if [ $FAIL -ne 0 ]; then
  echo "**** There were failing tests"
  exit 1
else 
  echo "Tests successful"
  exit 0
fi
