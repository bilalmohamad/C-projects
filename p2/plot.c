/**
 * @file plot.c
 * @author Bilal Mohamad (bmohama)
 * This program will create a plot based on the values of the input.
 * It will also scale the size of the plot based on the input.
 */

#include <stdlib.h>
#include <stdio.h>

#include "poly.h"
#include "svg.h"

/** Exist status if the X-value range at the start of the input is bad. */
#define INVALID_RANGE_STATUS 100

/** Exit status if the input polynomial is bad. */
#define INVALID_POLYNOMIAL_STATUS 101

/** Maximum number of coefficients a polynomial can have. */
#define MAX_POLY_COEFF 10

/** Increment of the ymin and ymax if the polynomial is constant for any x value */
#define ZERO_INCREMENT 0.5

/**
 * Starting point for the program
 * @return the exit status
 */
int main() {
	double xmin, xmax, ymin, ymax;

	if (scanf("%lf", &xmin) == 0) {
		printf("Invalid X range\n");
		return INVALID_RANGE_STATUS;
	}

	if (scanf("%lf", &xmax) == 0) {
		printf("Invalid X range\n");
		return INVALID_RANGE_STATUS;
	}

	if (xmin >= xmax) {
		printf("Invalid X range\n");
		return INVALID_RANGE_STATUS;
	}

	double coeff[MAX_POLY_COEFF];
	int read = readPoly(MAX_POLY_COEFF, coeff);
	if (read == INVALID_POLYNOMIAL_STATUS) {
		return INVALID_POLYNOMIAL_STATUS;
	}

	double xIncrement = (xmax - xmin) / PLOT_STEPS;
	double xValue = xmin;
	ymin = evaluate(xValue, read, coeff);
	ymax = ymin;

	for (int i = 0; i < PLOT_STEPS + 1; i++) {
		double y = evaluate(xValue, read, coeff);

		if (y > ymax) {
			ymax = y;
		}

		if (y < ymin) {
			ymin = y;
		}

		xValue += xIncrement;
	}

	if (ymin == ymax) {
		ymin -= ZERO_INCREMENT;
		ymax += ZERO_INCREMENT;
	}

	plotGraph(xmin, xmax, ymin, ymax, read, coeff);

	return EXIT_SUCCESS;
}
