/**
    @file testdriver.c
    @author David Sturgill (dbsturgi)

    This is a test driver for code in the buffer and md5 components.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "buffer.h"
#include "md5.h"

/** Total number or tests we tried. */
static int totalTests = 0;

/** Number of test cases passed. */
static int passedTests = 0;

/** Macro to check the condition on a test case, keep counts of passed/failed tests
    and report a message if the test fails. */
#define TestCase( conditional ) {\
  totalTests += 1; \
  if ( conditional ) { \
    passedTests += 1; \
  } else { \
    printf( "**** Failed unit test on line %d of %s\n", __LINE__, __FILE__ );    \
  } \
}

// These functions aren't really part of the public interface for the md5
// component, but, to test them we compile without marking them as static.
// Then, they're visible from other components, but we still need a prototype
// if we're going to be able to call them.
unsigned int fVersion0( unsigned int b, unsigned int c, unsigned int d );
unsigned int fVersion1( unsigned int b, unsigned int c, unsigned int d );
unsigned int fVersion2(  unsigned int b, unsigned int c, unsigned int d );
unsigned int fVersion3(  unsigned int b, unsigned int c, unsigned int d );
int gVersion0( int idx );
int gVersion1( int idx );
int gVersion2( int idx );
int gVersion3( int idx );
unsigned int rotateLeft( unsigned int value, int s );
void md5Iteration( unsigned int data[ 16 ], unsigned int *a, unsigned int *b,
                   unsigned int *c, unsigned int *d, int i );

int main()
{
  // As you finish parts of your implementation, move this directive down past
  // the blocks of code below.  That will enable tests of various functions you're
  // expected to implement.

  // Tests for buffer component.

  // Test makeBuffer()
  {
    Buffer *b = makeBuffer();

    TestCase( b != NULL );
    TestCase( b->len == 0 );
    TestCase( b->cap == 3 );

    freeBuffer( b );
  }


  // Test appendBuffer()
  {
    Buffer *b = makeBuffer();

    // Insert characters and make sure the buffer gets updated.
    appendBuffer( b, 'w' );
    TestCase( b->len == 1 );
    TestCase( b->data[ 0 ] == 'w' );

    appendBuffer( b, 'x' );
    TestCase( b->len == 2 );
    TestCase( b->data[ 1 ] == 'x' );

    appendBuffer( b, 'y' );
    TestCase( b->len == 3 );
    TestCase( b->data[ 2 ] == 'y' );

    // Force the buffer capacity to grow.
    appendBuffer( b, 'z' );
    TestCase( b->len == 4 );
    TestCase( b->data[ 3 ] == 'z' );
    TestCase( b->cap >= 4 );

    // Try adding a bunch of characters.
    for ( int i = 'a'; i <= 'z'; i++ )
      appendBuffer( b, i );
    TestCase( b->len == 30 );
    TestCase( b->data[ 29 ] == 'z' );
    TestCase( b->cap >= 30 );

    freeBuffer( b );
  }

  // Test readFile()
  {
    {
      Buffer *b = readFile( "input-1.txt" );
      TestCase( b != NULL );

      TestCase( b->len == 28 );
      // Make sure the buffer contains the right sequence of bytes.
      TestCase( memcmp( b->data, "This is a short input file.\n", 28 ) == 0 );
      TestCase( b->cap >= 28 );

      freeBuffer( b );
    }

    {
      Buffer *b = readFile( "input-3.txt" );

      // Make sure it looks like we got the whole input file.
      TestCase( b->len == 909 );
      TestCase( b->data[ 907 ] == '.' );
      TestCase( b->cap >= 909 );

      freeBuffer( b );
    }

    {
      // Try the binary input file.
      Buffer *b = readFile( "input-5.bin" );

      // Make sure it looks like we got the whole input file.
      TestCase( b->len == 11328 );
      TestCase( b->data[ 11327 ] == 0x1b );
      TestCase( b->cap >= 11328 );

      freeBuffer( b );
    }

    {
      // Try a file that doesn't exist.
      Buffer *b = readFile( "no-input-file.txt" );
      TestCase( b == NULL );
    }
  }

  // Tests for the md5 component

  // Test the four versions of the F function;
  {
    {
      unsigned int b = 0x00000001;
      unsigned int c = 0x00000003;
      unsigned int d = 0x00000007;
      unsigned int result;

      result = fVersion0( b, c, d );
      TestCase( result == 0x00000007 );

      result = fVersion1( b, c, d );
      TestCase( result == 0x00000001 );

      result = fVersion2( b, c, d );
      TestCase( result == 0x00000005 );

      result = fVersion3( b, c, d );
      TestCase( result == 0xFFFFFFFA );
    }

    {
      unsigned int b = 0xAB659F92;
      unsigned int c = 0xE4C6A120;
      unsigned int d = 0xC0D29093;
      unsigned int result;

      result = fVersion0( b, c, d );
      TestCase( result == 0xE0D68101 );

      result = fVersion1( b, c, d );
      TestCase( result == 0xA444B1B2 );

      result = fVersion2( b, c, d );
      TestCase( result == 0x8F71AE21 );

      result = fVersion3( b, c, d );
      TestCase( result == 0x5BAB5EDE );
    }
  }

  // Test the four versions of the G function;
  {
    {
      int result;

      result = gVersion0( 7 );
      TestCase( result == 7 );

      result = gVersion0( 9 );
      TestCase( result == 9 );

      result = gVersion1( 12 );
      TestCase( result == 13 );

      result = gVersion1( 0 );
      TestCase( result == 1 );

      result = gVersion2( 4 );
      TestCase( result == 1 );

      result = gVersion2( 14 );
      TestCase( result == 15 );

      result = gVersion3( 8 );
      TestCase( result == 8 );

      result = gVersion3( 10 );
      TestCase( result == 6 );
    }
  }

  // Test the rotateLeft() function.
  {
    unsigned int result;

    result = rotateLeft( 0x00000001, 1 );
    TestCase( result == 0x00000002 );

    result = rotateLeft( 0x80000000, 1 );
    TestCase( result == 0x00000001 );


    result = rotateLeft( 0x6223818F, 3 );
    TestCase( result == 0x111C0C7B );


    result = rotateLeft( 0x9C913C27, 11 );
    TestCase( result == 0x89E13CE4 );
  }

  // void initState( MD5State *state );

  // Test the initState() function.
  {
    MD5State state;

    initState( &state );
    TestCase( state.A == 0x67452301 );
    TestCase( state.B == 0xefcdab89 );
    TestCase( state.C == 0x98badcfe );
    TestCase( state.D == 0x10325476 );
  }

  // Test the padBuffer() function.
  {
    {
      Buffer *b = makeBuffer();

      // Put some characters in the buffer.
      char *str = "testing 1, 2, 3...";
      for ( int i = 0; str[ i ]; i++ )
        appendBuffer( b, str[ i ] );
      padBuffer( b );

      TestCase( b->len == 64 );

      unsigned char expected[ 64 ] =
        { 0x74, 0x65, 0x73, 0x74, 0x69, 0x6E, 0x67, 0x20,
          0x31, 0x2C, 0x20, 0x32, 0x2C, 0x20, 0x33, 0x2E,
          0x2E, 0x2E, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

      TestCase( memcmp( b->data, expected, 64 ) == 0 );
      freeBuffer( b );
    }

    {
      Buffer *b = makeBuffer();

      // Put some characters in the buffer.
      char *str = "This is a longer test string, chosen to be exactly 64 bytes long";
      for ( int i = 0; str[ i ]; i++ )
        appendBuffer( b, str[ i ] );
      padBuffer( b );

      TestCase( b->len == 128 );

      unsigned char expected[ 128 ] =
        { 0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20,
          0x61, 0x20, 0x6C, 0x6F, 0x6E, 0x67, 0x65, 0x72,
          0x20, 0x74, 0x65, 0x73, 0x74, 0x20, 0x73, 0x74,
          0x72, 0x69, 0x6E, 0x67, 0x2C, 0x20, 0x63, 0x68,
          0x6F, 0x73, 0x65, 0x6E, 0x20, 0x74, 0x6F, 0x20,
          0x62, 0x65, 0x20, 0x65, 0x78, 0x61, 0x63, 0x74,
          0x6C, 0x79, 0x20, 0x36, 0x34, 0x20, 0x62, 0x79,
          0x74, 0x65, 0x73, 0x20, 0x6C, 0x6F, 0x6E, 0x67,
          0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

      TestCase( memcmp( b->data, expected, 128 ) == 0 );
      freeBuffer( b );
    }


    {
      Buffer *b = makeBuffer();

      // Put some characters in the buffer.
      char *str = "This test is even longer.  It want to make it at least two "
        "blocks long, and, with padding, I'd like it to take three. "
        "Am I going to have to think of something else to say?  Guess not.";
      for ( int i = 0; str[ i ]; i++ )
        appendBuffer( b, str[ i ] );
      padBuffer( b );

      TestCase( b->len == 192 );

      unsigned char expected[ 192 ] =
        { 0x54, 0x68, 0x69, 0x73, 0x20, 0x74, 0x65, 0x73,
          0x74, 0x20, 0x69, 0x73, 0x20, 0x65, 0x76, 0x65,
          0x6E, 0x20, 0x6C, 0x6F, 0x6E, 0x67, 0x65, 0x72,
          0x2E, 0x20, 0x20, 0x49, 0x74, 0x20, 0x77, 0x61,
          0x6E, 0x74, 0x20, 0x74, 0x6F, 0x20, 0x6D, 0x61,
          0x6B, 0x65, 0x20, 0x69, 0x74, 0x20, 0x61, 0x74,
          0x20, 0x6C, 0x65, 0x61, 0x73, 0x74, 0x20, 0x74,
          0x77, 0x6F, 0x20, 0x62, 0x6C, 0x6F, 0x63, 0x6B,
          0x73, 0x20, 0x6C, 0x6F, 0x6E, 0x67, 0x2C, 0x20,
          0x61, 0x6E, 0x64, 0x2C, 0x20, 0x77, 0x69, 0x74,
          0x68, 0x20, 0x70, 0x61, 0x64, 0x64, 0x69, 0x6E,
          0x67, 0x2C, 0x20, 0x49, 0x27, 0x64, 0x20, 0x6C,
          0x69, 0x6B, 0x65, 0x20, 0x69, 0x74, 0x20, 0x74,
          0x6F, 0x20, 0x74, 0x61, 0x6B, 0x65, 0x20, 0x74,
          0x68, 0x72, 0x65, 0x65, 0x2E, 0x20, 0x41, 0x6D,
          0x20, 0x49, 0x20, 0x67, 0x6F, 0x69, 0x6E, 0x67,
          0x20, 0x74, 0x6F, 0x20, 0x68, 0x61, 0x76, 0x65,
          0x20, 0x74, 0x6F, 0x20, 0x74, 0x68, 0x69, 0x6E,
          0x6B, 0x20, 0x6F, 0x66, 0x20, 0x73, 0x6F, 0x6D,
          0x65, 0x74, 0x68, 0x69, 0x6E, 0x67, 0x20, 0x65,
          0x6C, 0x73, 0x65, 0x20, 0x74, 0x6F, 0x20, 0x73,
          0x61, 0x79, 0x3F, 0x20, 0x20, 0x47, 0x75, 0x65,
          0x73, 0x73, 0x20, 0x6E, 0x6F, 0x74, 0x2E, 0x80,
          0xB8, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

      TestCase( memcmp( b->data, expected, 192 ) == 0 );
      freeBuffer( b );
    }

  }

  // Test the md5Iteration() function, a test for each round.
  {
    {
      unsigned int data[] =
        { 0x3E89FFA4U, 0x56AF8963U, 0x72B214EFU, 0x6E078ACEU,
          0x539FAB27U, 0x754D1F0AU, 0xBC496D95U, 0x11695FEFU,
          0xBA9ED1AAU, 0x192B3715U, 0x88D80898U, 0xAEE9F73EU,
          0x02F429EDU, 0x7E840F0BU, 0x498B4509U, 0xFA54CF37U };
      unsigned int a = 0x8C91FCE1U;
      unsigned int b = 0xa3DFC292U;
      unsigned int c = 0x1247C589U;
      unsigned int d = 0X5403C3DCU;
      md5Iteration( data, &a, &b, &c, &d, 0 );

      TestCase( a == 0x5403C3DC );
      TestCase( b == 0x0B11278E );
      TestCase( c == 0xA3DFC292 );
      TestCase( d == 0x1247C589 );
    }

    {
      unsigned int data[] =
        { 0x7D7B9A46U, 0xB7851D52U, 0xECCDFCF2U, 0x4F0192C4U,
          0xB4587C78U, 0x149D9A7BU, 0xF24026F6U, 0x867C8D4FU,
          0x044D387CU, 0xBEEC6302U, 0x3103F912U, 0xEB240372U,
          0x688E8689U, 0x86566ADFU, 0xD4D78755U, 0x3ABE596BU };
      unsigned int a = 0x36F3E5FEU;
      unsigned int b = 0xF7CC2E7DU;
      unsigned int c = 0xC6CF8729U;
      unsigned int d = 0xF369EB5CU;
      md5Iteration( data, &a, &b, &c, &d, 21 );

      TestCase( a == 0xF369EB5C );
      TestCase( b == 0x0C0FEF41 );
      TestCase( c == 0xF7CC2E7D );
      TestCase( d == 0xC6CF8729 );
    }

    {
      unsigned int data[] =
        { 0x7998A7E9U, 0xD67277FFU, 0xB6CFE5D6U, 0x42B44CAEU,
          0xA0B8C114U, 0x606240D3U, 0xAF64D1C9U, 0x51E7D5CAU,
          0x2CD988C3U, 0x647164DAU, 0xB42C169AU, 0x27710515U,
          0xD442D6F8U, 0xC51BA1A9U, 0x54ECBB60U, 0xC34B15D8U };
      unsigned int a = 0x67F629DFU;
      unsigned int b = 0x8E302042U;
      unsigned int c = 0x8C198D10U;
      unsigned int d = 0x9ADE4754U;
      md5Iteration( data, &a, &b, &c, &d, 38 );

      TestCase( a == 0x9ADE4754 );
      TestCase( b == 0xC33F69D3 );
      TestCase( c == 0x8E302042 );
      TestCase( d == 0x8C198D10 );
    }

    {
      unsigned int data[] =
        {
         0xEA4902D6U, 0x6B7E4628U, 0xAAA617A2U, 0xE1D86531U,
         0x09107730U, 0xE98F9D4BU, 0x3ACC9173U, 0xC006C27AU,
         0xB9B3044DU, 0x7F809884U, 0x62D9856DU, 0xCEB1E48AU,
         0xC1B33EA0U, 0x45AFC5EEU, 0x5C22A633U, 0x944E0062U
        };
      unsigned int a = 0xC5F26E0AU;
      unsigned int b = 0x9BA81115U;
      unsigned int c = 0x8C0C4CD4U;
      unsigned int d = 0x665F2162U;
      md5Iteration( data, &a, &b, &c, &d, 59 );

      TestCase( a == 0x665F2162 );
      TestCase( b == 0xB7F63AF0 );
      TestCase( c == 0x9BA81115 );
      TestCase( d == 0x8C0C4CD4 );
    }
  }

  // Test the md5Block function
  {
    MD5State state;
    initState( &state );

    {
      unsigned char data[ 64 ] =
        { 0xB3, 0x7C, 0xB7, 0x92, 0x88, 0x4E, 0x2D, 0x5C,
          0xAF, 0x8D, 0x37, 0xFF, 0xEC, 0xF6, 0x7C, 0xB2,
          0xCA, 0xDC, 0x7F, 0xD4, 0x03, 0xC4, 0xF1, 0x80,
          0xB0, 0x48, 0x60, 0x75, 0x0C, 0xBE, 0xF5, 0x7B,
          0xB8, 0x50, 0xFE, 0x46, 0xE8, 0x42, 0xAC, 0xED,
          0xB5, 0xAD, 0xA0, 0x3B, 0xA9, 0xEF, 0xE9, 0x2A,
          0x57, 0xA5, 0x02, 0x8E, 0x3A, 0xEB, 0x18, 0x69,
          0x75, 0x3D, 0xC4, 0xA8, 0xD7, 0x38, 0x7A, 0x71 };

      md5Block( data, &state );
      TestCase( state.A == 0xCA906BA6 );
      TestCase( state.B == 0x00E9739A );
      TestCase( state.C == 0x56B375B9 );
      TestCase( state.D == 0x107668AC );
    }

    {
      unsigned char data[ 64 ] =
        { 0x45, 0x50, 0x33, 0x77, 0x24, 0x0E, 0x6D, 0x9E,
          0x86, 0x7F, 0x70, 0x86, 0xCB, 0x2A, 0x3C, 0x23,
          0x07, 0x58, 0x4F, 0xDB, 0x3E, 0x62, 0xFA, 0xE0,
          0xCA, 0xDA, 0xF4, 0x39, 0xB1, 0x85, 0xB1, 0x65,
          0x0D, 0x9E, 0x70, 0x7B, 0x8F, 0xBE, 0xE1, 0xE6,
          0xFF, 0xE6, 0xA4, 0xBE, 0x31, 0xC8, 0x7B, 0x53,
          0xF8, 0xB1, 0xC3, 0x47, 0x48, 0x41, 0x79, 0x12,
          0x24, 0x1F, 0xDE, 0x52, 0x04, 0x1E, 0x0E, 0xEB };

      md5Block( data, &state );
      TestCase( state.A == 0xD38C8EBD );
      TestCase( state.B == 0x27B2491B );
      TestCase( state.C == 0x335EEC96 );
      TestCase( state.D == 0xBD50FC28 );
    }
  }
#ifdef NEVER
#endif

  printf( "You passed %d / %d unit tests\n", passedTests, totalTests );

  if ( passedTests != 80 )
    return EXIT_FAILURE;
  else
    return EXIT_SUCCESS;
}
