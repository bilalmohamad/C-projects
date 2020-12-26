/**
 * @file poly.c
 * @author Bilal Mohamad (bmohama)
 * This component will the graph to the SVG canvas with gridlines, plotted points, and a line
 * that connects all the points.
 * The graph is scaled based on the input to best fit the results.
 */

// Include our own header first
#include "svg.h"

// Then, anything else we need in the implementation file.
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "poly.h"

// If you're doing the extra credit, you can use this flag to
// determine whether you should print numbers on the gridlines.  We
// can use a compile-time option to set this value to true, if we want
// to check the extra credit.  Otherwise, if this flag is false, your
// output should look just like the regular output.
#ifndef PLOT_NUMBERS
#define PLOT_NUMBERS false
#endif

/** Maximum number of gridlines we'd like to see on the plot, either
 horizontally or vertically. */
#define MAX_GRID 20.0

/** Constant used for the width of the graph */
#define WIDTH 600

/** Constant used for the height of the graph */
#define HEIGHT 400.0

/** Constant used for the adjusted 90% of the graph */
#define ADJ_HEIGHT 360

/** Constant used for the bottom 5% margin of the graph */
#define MARGIN 20

/** Constant used to prevent error with the double value */
#define OFFSET 0.2

/** Constant used for incrementing the x value until it reaches the width */
#define INCREMENT 6

/** Constant used for logarithmic calculations */
#define LOG_CALC 10.0

/**
 * The plot uses a range of X coordinates given by the user by sampling values of the polynomial.
 * These coordinates need to be converted to 0..600 range expected by the SVG canvas.
 *
 * @param xmin	the minimum x value
 * @param xmax	the maximum x value
 * @param x		the current x value
 *
 * @return the converted x value into the SVG coordinate system
 */
static double convertX(double xmin, double xmax, double x) {
	// ...
	double range = xmax - xmin;
	double b = WIDTH / range;
	double a = -1 * b * xmin;
	return a + b * x;
}

/**
 * The plot uses a range of Y coordinates given by the user by sampling values of the polynomial.
 * These coordinates need to be converted to 0..400 range expected by the SVG canvas.
 *
 * @param ymin	the minimum y value
 * @param ymax	the maximum y value
 * @param y		the current y value
 *
 * @return the converted y value into the SVG coordinate system
 */
static double convertY(double ymin, double ymax, double y) {
	double range = ymax - ymin;

	if (range == 1) {
		double b = HEIGHT / range;
		double a = 0 - ymin * b;
		double answer = HEIGHT - (a + b * y);
		return answer;
	} else {
		double b = ADJ_HEIGHT / range;
		double a = MARGIN - ymin * b;
		double answer = HEIGHT - (a + b * y);
		return answer;
	}
}

/**
 * This function is responsible for writing out the vertical gridlines in the plot.
 * It will have to compute how many gridlines there should be, then write out an SVG element for each gridline.
 *
 * @param xmin	the minimum x value
 * @param xmax	the maximum x value
 */
static void plotVerticalGrid(double xmin, double xmax) {
	// Range of X coordinates.
	double delta = xmax - xmin;

	// Round the base-10 log down, up to this threshold.
	double threshold = log10( MAX_GRID) - 1.0;

	// Spacing for the gridlines based on this power of 10.
	double gap = pow(LOG_CALC, floor(log10(delta) - threshold));

	// From the left edge of the plot, round down to a multiple of gap.
	double start = floor(xmin / gap) * gap;

	// Draw vertical gridlines as long as they fit inside the plot.
	double pos = start;
	while (pos < xmax) {
		if (pos > xmin) {
			double convertValue = convertX(xmin, xmax, pos);
			printf("  <line x1=\"%.1lf\" y1=\"0.0\" x2=\"%.1lf\" y2=\"400.0\" stroke=\"gray\"/>\n", convertValue, convertValue);
		}
		pos += gap;
	}
}

/**
 * This function is responsible for writing out the horizontal gridlines in the plot.
 * It will have to compute how many gridlines there should be, then write out an SVG element for each gridline.
 *
 * @param ymin	the minimum y value
 * @param ymax	the maximum y value
 */
static void plotHorizontalGrid(double ymin, double ymax) {
	// Range of X coordinates.
	double delta = ymax - ymin;

	// Round the base-10 log down, up to this threshold.
	double threshold = log10( MAX_GRID) - 1.0;

	// Spacing for the gridlines based on this power of 10.
	double gap = pow(LOG_CALC, floor(log10(delta) - threshold));

	// From the left edge of the plot, round down to a multiple of gap.
	double start = floor(ymin / gap) * gap;

	// Draw vertical gridlines as long as they fit inside the plot.
	double pos = start;
	while (pos < ymax + gap * OFFSET) {
		if (pos > ymin - gap * OFFSET) {
			// ...
			double convertValue = convertY(ymin, ymax, pos);
			printf("  <line x1=\"0.0\" y1=\"%.1lf\" x2=\"600.0\" y2=\"%.1lf\" stroke=\"gray\"/>\n", convertValue, convertValue);
		}
		pos += gap;
	}
}

/**
 * This function is part of the svg component. Its job is to write out all the HTML and SVG to draw the plot.
 * It can use other functions, like plotVerticalGrid() to help generate the output.
 *
 * @param xmin	the minimum x value
 * @param xmax	the maximum x value
 * @param ymin	the minimum y value
 * @param ymax	the maximum y value
 * @param terms	the number of coefficients in the polynomial
 * @param coeff	the array containing the coefficients of the polynomial
 */
void plotGraph(double xmin, double xmax, double ymin, double ymax, int terms, double coeff[]) {

	printf("<!DOCTYPE html>\n");
	printf("<html>\n");
	printf("<body>\n");
	printf("<svg width=\"600\" height=\"400\">\n");

	plotVerticalGrid(xmin, xmax);
	plotHorizontalGrid(ymin, ymax);

	printf("  <polyline points=\"");

	double xIncrement = (xmax - xmin) / PLOT_STEPS;
	double xValue = xmin;

	for (int x = 0; x <= WIDTH; x += INCREMENT) {
		double y = evaluate(xValue, terms, coeff);
		double convertedValue = convertY(ymin, ymax, y);
		printf("%.1lf,%.1lf", (double) x, convertedValue);

		xValue += xIncrement;

		if (x < WIDTH) {
			printf(" ");
		}
	}

	printf("\" fill=\"none\" stroke=\"red\" stroke-width=\"3\"/>\n");
	printf("</svg>\n");
	printf("</body>\n");
	printf("</html>\n");
}
