//
// A buggy program to print out prime numbers.
// 
// (This one gets stuck in an infinite loop, with an error so obvious,
// the compiler warns us about it)
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LIMIT 100

/*
 * Return true if the given value is prime.
 */
bool isPrime( long val )
{
  // Check all smaller values as potential factors.
  for ( long j = 2; j * j <= val; j + 1 )
    // Is this a factor?
    if ( val % j == 0 )
      return false;
  
  return true;
}

/*
 * Starting point for this program.  Test a sequence of values for
 * prime-ness.
 */
int main( void )
{
  // Test values starting from 2 for prime-ness
  for ( long i = 2; i <= LIMIT; i++ ) {
    // If it's prime, print it out.
    if ( isPrime( i ) ) {
      printf( "%ld\n", i );
    }
  }
  
  return 0;
}
