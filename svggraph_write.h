#include "svggraph.h"


/**
 * @brief write_basics .. head of the svg file: doctype, xml, ..
 * @param svg_file
 */
void write_basics(FILE *svg_file);

/**
 * @brief write_wh .. appends width and height specification for the svg picture
 * @param svg_file
 * @param width
 * @param height
 */
void write_wh(FILE *svg_file, size_t width, size_t height);

/**
 * @brief write_coordinates_translation .. appends translation to a svg file for debugging
 * @param svg_file
 */
void write_coordinates_translation(FILE *svg_file);

/**
 * @brief write_xaxis ..  appends x axis line to a svg file
 * @param svg_file
 * @param width
 * @param height
 */
void write_xaxis(FILE *svg_file, size_t width, size_t height);

/**
 * @brief write_yaxis ..  appends y axis line to a svg file
 * @param svg_file
 * @param width
 * @param height
 */
void write_yaxis(FILE *svg_file, size_t width, size_t height);

/**
 * @brief write_xvalues .. y text values of the axes
 * @param svg_file
 * @param width
 * @param height
 * @param x
 */
void write_xvalues(FILE *svg_file, size_t width, size_t height, int x);

/**
 * @brief write_yvalues .. y text values of the axes
 * @param svg_file
 * @param width
 * @param height
 * @param y
 */
void write_yvalues(FILE *svg_file, size_t width, size_t height, int y);

/**
 * @brief write_zero .. zero text value at the crosssection of the axes, it needs to be treated as a special case
 * @param svg_file
 * @param width
 * @param height
 */
void write_zero(FILE *svg_file, size_t width, size_t height);

/**
 * @brief write_cut appends code for clippath
 * @param svg_file
 * @param width
 * @param height
 */
void write_cut(FILE *svg_file, size_t width, size_t height);

/**
 * @brief map_to_xcoordinate
 * @param x_range
 * @param what
 * @param width
 * @return
 */
double map_to_xcoor(double x_range, double what, double width);

/**
 * @brief map_to_ycoordinate
 * @param y_range
 * @param what
 * @param height
 * @return
 */
double map_to_ycoor(double y_range, double what, double height);

/**
 * @brief write_linear_line .. appends a code for a linear line of a linear function
 * @param svg_file
 * @param width
 * @param height
 * @param x .. x axis range
 * @param y .. y axis range
 * @param a .. the function coefficient
 * @param graph_type .. available functions enum type
 * @param color .. RGB struct type
 */
int write_linear_line(FILE *svg_file, int *parsed_args, double a, graph_type graph_type, graph_color color);

/**
 * @brief write_sine_line .. appends a code for a sine approximation of the Bezier Curve to a svg file
 * @param svg_file
 * @param width
 * @param height
 * @param x .. x axis range
 * @param y .. y axis range
 * @param graph_type .. available functions enum type
 * @param color .. RGB struct type
 */
int write_sine_line(FILE *svg_file, int *parsed_args, graph_type graph_type, graph_color color);

/**
 * @brief write_function_label .. appends the function label to a svg file
 * @param svg_file
 * @param width
 * @param user_graph_input
 * @param function_n .. number of functions to be plotted
 * @param color .. RGB struct type
 */
void write_function_label(FILE *svg_file, size_t width, char *fnc_label, size_t function_n, graph_color color);

/**
 * @brief close_svg
 * @param svg_file
 */
void close_svg(FILE *svg_file);

/**
 * @brief close_translation
 * @param svg_file
 */
void close_translation(FILE *svg_file);

