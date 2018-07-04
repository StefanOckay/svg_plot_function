#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/**
 * @brief The Parsed_double struct
 */
struct Parsed_double {
    int error;
    double real_number;
};

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