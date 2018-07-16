#include "svggraph.h"


/**
 * @brief prov_round .. provisore as rounds via sprintf, sscanf
 * @param x .. double to be rounded to 2 decimal places
 */
void prov_round(double *x);

/**
 * @brief set_color .. sets readable(= not too bright) color for graph
 * @param color .. rgb color struct
 */
void set_color(graph_color *rgb_color);

/**
 * @brief cut_new_line .. transform the s string from "X\n" to "X\0", doesn't modify "X" without "\n"
 * @param s
 */
void cut_new_line(char *s);

/**
 * @brief is_valid_char .. checks the validity of chars passed to float conversion from stdin
 * @param c .. the char to be checked
 * @param valid_chars .. array of valid chars
 * @return 0 if the char c is contained in the valid_chars, non 0 otherwise
 */
int is_valid_char(char c, const char *valid_chars);

/**
 * @brief find_graph_type .. stores graph type from the user runtime input to graph_type
 * @param graph_type .. enum type listing graph types available
 * @param user_graph_input
 * @param a .. graph coefficient
 */
void find_graph_type_and_values(graph_type *graph_type, const char *user_graph_in, double *a);

/**
 * @brief parse_user_input .. stores graph coefficient into a
 * @param graph_const .. model of the graph string
 * @param user_graph_input
 * @param a .. graph coefficient
 * @return 0 value if success, non-zero otherwise
 */
int parse_user_input(const char *graph_const, const char *user_graph_input, double *a);

/**
 * @brief parse_cml_input .. parses command line user input
 * @param graph_in_values .. storage for parsed cml values
 * @param argv .. command line string values
 * @return 0 if success, error otherwise
 */
int parse_cml_input(int *parsed_args, char *argv[]);
