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
int simplify_function_str(char *str);

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
int print_table_for_function(char* function_str, double (*operation)(double, double), double a);

/**
 * @brief print_to_stdout .. print the table for function values
 * @param type .. type of a graph
 * @param parameter .. coeffiecient of the function
 * @return int value indicating (non)success
 */
int print_to_stdout(enum graph_type type, double a);

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
