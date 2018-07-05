#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <math.h>
#include <string.h>


#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2           3.14159265358979323846 / (double)2
#endif

#define BEZ_SINE_APR 0.364212423249

#define SUCCESS 0

//#define DEBUG
#ifdef DEBUG
#define OFFSET 100
#else
#define OFFSET 0
#endif

/**
 * @brief The graph_color struct .. RGB values of the graph plot
 */
struct graph_color
{
    int red;
    int green;
    int blue;
};

/**
 * @brief The graph_type enum .. "list" of the available functions
 */
enum graph_type
{
    SIN,
    COS,
    PROD,
    PLUS,
    MINUS,
    ERROR
};


