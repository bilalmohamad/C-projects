/**
 * @file poly.h
 * @author Bilal Mohamad (bmohama)
 * This file acts as the interface for the poly.c file
 */

/** Exit status if the input polynomial is bad. */
#define INVALID_POLYNOMAIL_STATUS 101

/**
 * This function will read in an entered polynomial based on the input.
 *
 * @param	capacity	the number of coefficients allowed for the polynomial
 * @param	coeff		the array containing the coefficients for the polynomial
 *
 * @return	the number of values read in through scanf
 */
int readPoly(int capacity, double coeff[]);

/**
 * This function evaluates the polynomial at the entered x value
 *
 * @param	x		the value to evaluate the polynomial at
 * @param	terms	the number of coefficients in the polynomial
 * @param	coeff	the array containing the coefficients for the polynomial
 *
 * @return the polynomial evaluated at the entered x value
 */
double evaluate(double x, int terms, double coeff[]);
