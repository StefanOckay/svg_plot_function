#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PI (3.14159265358979323846)
#define PI_2 (3.14159265358979323846 / (double)2)
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

/**
 * @brief new_table .. initializes a table struct for stdout
 * @param rows
 * @param collumns
 * @return the new table
 */
struct Table *new_table(unsigned char rows, unsigned char collumns);

/**
 * @brief destroy_table .. frees memory of the table
 * @param table
 */
void destroy_table(struct Table *table);

/**
 * @brief print_horizontal_table_border
 * @param table
 * @param collun_widths
 */
void print_horizontal_table_border(struct Table *table, size_t *collun_widths);

/**
 * @brief print_table_line
 * @param table
 * @param row
 * @param collun_widths
 */
void print_table_line(struct Table *table, unsigned char row, size_t *collun_widths);

/**
 * @brief print_table
 * @param table
 * @return
 */
int print_table(struct Table *table);

/**
 * @brief assignCellI
 * @param table
 * @param row
 * @param collumn
 * @param value
 * @return
 */
int assignCellI(struct Table *table, unsigned char row, unsigned char collumn, int value);

/**
 * @brief simplify
 * @param str
 * @return
 */
int simplify(char *str);

/**
 * @brief assignCellF
 * @param table
 * @param row
 * @param collumn
 * @param value
 * @return
 */
int assignCellF(struct Table *table, unsigned char row, unsigned char collumn, double value);

/**
 * @brief assignCellS
 * @param table
 * @param row
 * @param collumn
 * @param value
 * @return
 */
int assignCellS(struct Table *table, unsigned char row, unsigned char collumn, char *value);

/**
 * @brief print_table_for_function
 * @param function_str
 * @param parameter
 * @return
 */
int print_table_for_function(char* function_str, double (*function)(double, double), double parameter);

/**
 * @brief print_to_stdout .. print the table for function values
 * @param type .. type of a graph
 * @param parameter .. coeffiecient of the function
 * @return int value indicating (non)success
 */
int print_to_stdout(enum graph_type type, double parameter);

/**
 * @brief plus
 * @param x
 * @param parameter
 * @return
 */
double plus(double x, double parameter);

/**
 * @brief minus
 * @param x
 * @param parameter
 * @return
 */
double minus(double x, double parameter);

/**
 * @brief multiply
 * @param x
 * @param parameter
 * @return
 */
double multiply(double x, double parameter);

/**
 * @brief sinus
 * @param x
 * @param parameter
 * @return
 */
double sinus(double x, double parameter);

/**
 * @brief cosinus
 * @param x
 * @param parameter
 * @return
 */
double cosinus(double x, double parameter);

/**
 * @brief cut_new_line .. transform the s string from "X\n" to "X\0", doesn't modify "X" without "\n"
 * @param s
 */
void cut_new_line(char *s);

/**
 * @brief is_valid_char .. checks the validity of chars passed to float conversion from stdin
 * @param c .. the char to be checked
 * @param valid_chars .. array of valid chars
 * @return 0 if the char c is contained in the valid_chars
 */
int is_valid_char(char c, const char *valid_chars);

/**
 * @brief find_graph_type .. determines graph type from the user runtime input
 * @param graph_type .. enum type listing graph types available
 * @param user_graph_input
 * @param a .. outer variable expecting value from Parsed_double struct
 */
void find_graph_type(enum graph_type *graph_type, const char *user_graph_input, double *a);

/**
 * @brief parse_user_input .. parses runtime user function input
 * @param graph_const .. type of a function stored in a model string to be compared with UI
 * @param user_graph_input
 * @return structure containing the parsed input and error value if invalid input was obtained
 */
struct Parsed_double parse_user_input(const char *graph_const, const char *user_graph_input);

/**
 * @brief parse_cml_input .. parses command line user input
 * @param graph_in_values .. storage for parsed cml values
 * @param argv .. command line string values
 * @return 0 if success, non 0 otherwise
 */
int parse_cml_input(int graph_in_values[4], char *argv[]);

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
void write_wh(FILE *svg_file, int width, int height);

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
void write_xaxis(FILE *svg_file, int width, int height);

/**
 * @brief write_yaxis ..  appends y axis line to a svg file
 * @param svg_file
 * @param width
 * @param height
 */
void write_yaxis(FILE *svg_file, int width, int height);

/**
 * @brief write_xvalues .. y text values of the axes
 * @param svg_file
 * @param width
 * @param height
 * @param x
 */
void write_xvalues(FILE *svg_file, int width, int height, int x);

/**
 * @brief write_yvalues .. y text values of the axes
 * @param svg_file
 * @param width
 * @param height
 * @param y
 */
void write_yvalues(FILE *svg_file, int width, int height, int y);

/**
 * @brief write_zero .. zero text value at the crosssection of the axes, it needs to be treated as a special case
 * @param svg_file
 * @param width
 * @param height
 */
void write_zero(FILE *svg_file, int width, int height);

/**
 * @brief write_cut appends code for clippath
 * @param svg_file
 * @param width
 * @param height
 */
void write_cut(FILE *svg_file, int width, int height);

/**
 * @brief map_to_xcoordinate
 * @param x_range
 * @param what
 * @param width
 * @return
 */
double map_to_xcoordinate(int x_range, double what, int width);

/**
 * @brief map_to_ycoordinate
 * @param y_range
 * @param what
 * @param height
 * @return
 */
double map_to_ycoordinate(int y_range, double what, int height);

/**
 * @brief write_linear_line .. appends a code for a linear line of a linear function
 * @param svg_file
 * @param width
 * @param height
 * @param x .. x axis range
 * @param y .. y axis range
 * @param a .. the function coefficient
 * @param graph_type
 * @param color
 */
void write_linear_line(FILE *svg_file, int width, int height, int x, int y, double a, enum graph_type graph_type, struct graph_color color);

// https://stackoverflow.com/questions/29022438/how-to-approximate-a-half-cosine-curve-with-bezier-paths-in-svg
/**
 * @brief write_sine_line .. appends a code for a sine approximation of the Bezier Curve to a svg file
 * @param svg_file
 * @param width
 * @param height
 * @param x .. x axis range
 * @param y .. y axis range
 * @param graph_type .. the enum type
 * @param color
 */
void write_sine_line(FILE *svg_file, int width, int height, int x, int y, enum graph_type graph_type, struct graph_color color);

/**
 * @brief write_function_label .. appends the function label to a svg file
 * @param svg_file
 * @param width
 * @param user_graph_input
 * @param function_n .. number of functions to be plotted
 * @param color
 */
void write_function_label(FILE *svg_file, int width, char *user_graph_input, int function_n, struct graph_color color);

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



