/**
 * @file poly.c
 * @author Bilal Mohamad (bmohama)
 * This component will read in the polynomial and evaluate an entered polynomial.
 */

// Include our own header first
#include "poly.h"

// Then, anything else we need in the implementation file.
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/** Exit status if the input polynomial is bad. */
#define INVALID_POLYNOMIAL_STATUS 101

/**
 * This function is part of the poly component. It reads coefficients for the polynomial from standard input and stores them in the given array.
 * The capacity parameter tells the function how much room the coeff[] array has for coefficients.
 * The function tries to read all the coefficients it can until it reaches the end-of-file and returns the number of coefficients it actually reads.
 * If the input polynomial is bad (e.g., with too many coefficients or with input that doesn't parse as a floating point number),
 * this function will print an appropriate error message and exit the program with a status of 101.
 *
 * @param	capacity	the number of coefficients allowed for the polynomial
 * @param	coeff		the array containing the coefficients for the polynomial
 *
 * @return	the number of values read in through scanf
 */
int readPoly(int capacity, double coeff[])
{
	// ...
	int loopStatus = 1;
	int index = 0;
	int read = 0;

	while (loopStatus){
		loopStatus = scanf("%lf", &coeff[index]);
		read++;

		if (loopStatus == 0){
			printf("Invalid polynomial\n");
			return INVALID_POLYNOMIAL_STATUS;
		}

		else if (read - 1 > capacity){
			printf("Invalid polynomial\n");
			return INVALID_POLYNOMIAL_STATUS;
		}

		else if (loopStatus == EOF){
			loopStatus = 0;
		}

		index++;
	}

	return read;
}


/**
 * This function is part of the poly component. It evaluates a polynomial at the given x value and returns the result.
 * The coeff[] array contains the coefficients for the polynomial it should evaluate,
 * and the terms parameter tells it how many terms the polynomial has (how many elements in coeff[] to use).
 *
 * @param	x		the value to evaluate the polynomial at
 * @param	terms	the number of coefficients in the polynomial
 * @param	coeff	the array containing the coefficients for the polynomial
 *
 * @return the polynomial evaluated at the entered x value
 */
double evaluate(double x, int terms, double coeff[])
{
	// ...
	double total = 0;

	for (int i = 0; i < terms; i++){
		total += coeff[i] * pow(x, i);
	}

	return total;
}

