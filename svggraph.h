#ifndef SVGGRAPH
#define SVGGRAPH

#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2 3.14159265358979323846 / (double)2
#endif

#define BEZ_SINE_APP 0.364212423249

//#define DEBUG
#ifdef DEBUG
#define OFFSET 100
#else
#define OFFSET 0
#endif

/**
 * @brief The graph_color struct .. RGB values of the graph plot
 */
typedef struct _graph_color
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} graph_color;

/**
 * @brief The graph_type enum .. "list" of the available functions
 */
typedef enum _graph_type
{
    SIN,
    COS,
    PROD,
    PLUS,
    MINUS,
    ERROR
} graph_type;


#endif /* SVGGRAPH */
