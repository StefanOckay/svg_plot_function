#include "svggraph.h"


/**
 * @brief The Table struct
 */
struct Table {
    unsigned char rows;
    unsigned char collumns;
    char **cells;
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
void print_horizontal_table_border(struct Table *table, size_t *collumn_widths);

/**
 * @brief print_table_line
 * @param table
 * @param row
 * @param collun_widths
 */
void print_table_line(struct Table *table, unsigned char row, size_t *collumn_widths);

/**
 * @brief print_table
 * @param table
 * @return int value indicating (non)success
 */
int print_table(struct Table *table);

/**
 * @brief assignCellI .. integer on pos [row, column]
 * @param table
 * @param row
 * @param collumn
 * @param value .. to be stored to the table
 * @return int value indicating (non)success
 */
int assignCellI(struct Table *table, unsigned char row, unsigned char collumn, int value);

/**
 * @brief simplify
 * @param str
 * @return int value indicating (non)success
 */
int simplify_function_str(char *str);

/**
 * @brief assignCellF .. float on pos [row, column]
 * @param table
 * @param row
 * @param collumn
 * @param value .. to be stored to the table
 * @return int value indicating (non)success
 */
int assignCellF(struct Table *table, unsigned char row, unsigned char collumn, double value);

/**
 * @brief assignCellS .. string on pos [row, column]
 * @param table
 * @param row
 * @param collumn
 * @param value .. to be stored to the table
 * @return int value indicating (non)success
 */
int assignCellS(struct Table *table, unsigned char row, unsigned char collumn, char *value);

/**
 * @brief get_function_label
 * @param type
 * @param a .. coeffiecient of the function
 * @param fnc_label
 * @return 0 if success, non-zero otherwise
 */
int get_function_label(graph_type type, double a, char *fnc_label);

/**
 * @brief assign_table_values
 * @param fnc_label
 * @param operation
 * @param a .. coeffiecient of the function
 * @return 0 if success, non-zero otherwise
 */
int assign_table_values(struct Table *table, char *fnc_label, double (*operation)(double, double), double a);

/**
 * @brief print_to_stdout .. print a table for function values
 * @param type .. type of a graph
 * @param a .. coeffiecient of the function
 * @param fnc_label
 * @return int value indicating (non)success
 */
int print_to_stdout(graph_type type, double a, char *fnc_label);

/**
 * @brief plus
 * @param x
 * @param parameter
 * @return
 */
double plus(double x, double a);

/**
 * @brief minus
 * @param x
 * @param parameter
 * @return
 */
double minus(double x, double a);

/**
 * @brief multiply
 * @param x
 * @param parameter
 * @return
 */
double multiply(double x, double a);

/**
 * @brief sinus
 * @param x
 * @param parameter
 * @return
 */
double sinus(double x, double a);

/**
 * @brief cosinus
 * @param x
 * @param parameter
 * @return
 */
double cosinus(double x, double a);
