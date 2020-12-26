/**
 * @file svg.h
 * @author Bilal Mohamad (bmohama)
 * This file acts as the interface for the svg.c file
 */

/** When whe're drawing the plot or sampling its Y range, we use
    PLOT_STEPS + 1 uniformly spaced samples. */
#define PLOT_STEPS 100

/**
 * Plots the points, gridlines, and polyline of the graph.
 * The graph is scaled based on the input.
 *
 * @param xmin	the minimum x value
 * @param xmax	the maximum x value
 * @param ymin	the minimum y value
 * @param ymax	the maximum y value
 * @param terms	the number of coefficients in the polynomial
 * @param coeff	the array containing the coefficients of the polynomial
 */
void plotGraph( double xmin, double xmax, double ymin, double ymax,
                int terms, double coeff[] );
