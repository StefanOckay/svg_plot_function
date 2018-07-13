#include "svggraph_table.h"


struct Table *new_table(unsigned char rows, unsigned char collumns) {
    struct Table *table = calloc(1, sizeof(struct Table));
    if (table == NULL) {
        return NULL;
    }
    table->rows = rows;
    table->collumns = collumns;
    table->cells = calloc(collumns * rows, sizeof(char *));
    if (table->cells == NULL) {
        free(table);
        return NULL;
    }
    return table;
}

void free_table(struct Table *table) {
    for (size_t r = 0; r < table->rows; r++) {
        for (size_t c = 0; c < table->collumns; c++) {
            free(table->cells[r * table->collumns + c]);
        }
    }
    free(table->cells);
}

void print_horizontal_table_border(struct Table *table, size_t *collumn_widths) {
    putchar('+');
    for(size_t c = 0; c < table->collumns; c++) {
        for(size_t j = 0; j < collumn_widths[c]; j++) {
            putchar('-');
        }
        putchar('+');
    }
    putchar('\n');
}

void print_table_line(struct Table *table, unsigned char row, size_t *collumn_widths) {
    putchar('|');
    for(size_t c = 0; c < table->collumns; c++){
        if(table->cells[row * table->collumns + c] == NULL) {
            printf("%-*s", (int)collumn_widths[c], "");
        } else {
            printf("%-*s", (int)collumn_widths[c], table->cells[row * table->collumns + c]);
        }
        putchar('|');
    }
    putchar('\n');
}

int print_table(struct Table *table) {
    size_t *collumn_widths = calloc(table->collumns, sizeof(size_t));
    if (collumn_widths == NULL) {
        return ENOMEM;
    }
    for (size_t r = 0; r < table->rows; r++) {
        for (size_t c = 0; c < table->collumns; c++) {
            if (table->cells[r * table->collumns + c] != NULL) {
                if (collumn_widths[c] < strlen(table->cells[r * table->collumns + c])) {
                    collumn_widths[c] = strlen(table->cells[r * table->collumns + c]);
                }
            }
        }
    }
    print_horizontal_table_border(table, collumn_widths);
    for(unsigned char r = 0; r < table->rows; r++) {
        print_table_line(table, r, collumn_widths);
        print_horizontal_table_border(table, collumn_widths);
    }
    free(collumn_widths);
    return EXIT_SUCCESS;
}

int assignCellI(struct Table *table, unsigned char row, unsigned char collumn, int value) {
    if (row >= table->rows || collumn >= table->collumns ) {
        return EINVAL;
    }
    char buffer[22];
    sprintf(buffer, "%i", value);
    size_t len = strlen(buffer);
    char *str = calloc(len + 1, sizeof(char));
    if (str == NULL) {
        return ENOMEM;
    }
    strcpy(str, buffer);
    table->cells[row * table->collumns + collumn] = str;
    return EXIT_SUCCESS;
}

int assignCellF(struct Table *table, unsigned char row, unsigned char collumn, double value) {
    if (row >= table->rows || collumn >= table->collumns ) {
        return EINVAL;
    }
    char buffer[22];
    sprintf(buffer, "%.2f", value);
    int len = strlen(buffer);
    char *str = calloc(len + 1, sizeof(char));
    if (str == NULL) {
        return ENOMEM;
    }
    if (value == 0) {
        strcpy(str, "0");
    } else {
        strcpy(str, buffer);
    }
    table->cells[row * table->collumns + collumn] = str;
    return EXIT_SUCCESS;
}

int assignCellS(struct Table *table, unsigned char row, unsigned char collumn, char* value) {
    if (row >= table->rows || collumn >= table->collumns ) {
        return EINVAL;
    }
    size_t len = strlen(value);
    char *str = calloc(len + 1, sizeof(char));
    if (str == NULL) {
        return ENOMEM;
    }
    strcpy(str, value);
    table->cells[row * table->collumns + collumn] = str;
    return EXIT_SUCCESS;
}

int assign_table_values(struct Table *table, char *function_str, double (*operation)(double, double), double a) {
    if (table == NULL) {
        return ENOMEM;
    }
    int ret_code = 0;
    if ((ret_code = assignCellS(table, 0, 0, "x"))) {
        return ret_code;
    }
    if ((ret_code = assignCellS(table, 1, 0, function_str))) {
        return ret_code;
    }
    for (int x = 0; x <= 5; x++){
        if ((ret_code = assignCellI(table, 0, x + 1, x))) {
            return ret_code;
        }
        if ((ret_code = assignCellF(table, 1, x + 1, operation(x, a)))) {
            return ret_code;
        }
    }
    return EXIT_SUCCESS;
}

int simplify_function_str(char *str) {
    int len = strlen(str);
    for(int i = len - 1; len > 0; i--) {
        if (str[i] == '0') {
            str[i] = '\0';
            len--;
        } else if (str[i] == '.') {
            str[i] = '\0';
            len--;
            break;
        } else {
            break;
        }
    }
    return len;
}

int print_to_stdout(graph_type type, double a) {
    struct Table *table = new_table(2, 7);
    char label[22];
    int ret_code;
    switch (type) {
        case SIN:
            ret_code = assign_table_values(table, "sin x", &sinus, 0);
            break;
        case COS:
            ret_code = assign_table_values(table, "cos x", &cosinus, 0);
            break;
        case PLUS:
            sprintf(label, "x + %.2f", a);
            simplify_function_str(label);
            ret_code = assign_table_values(table, label, &plus, a);
            break;
        case MINUS:
            sprintf(label, "x - %.2f", a);
            simplify_function_str(label);
            ret_code = assign_table_values(table, label, &minus, a);
            break;
        case PROD:
            sprintf(label, "%.2f", a);
            simplify_function_str(label);
            strcat(label, " x");
            ret_code = assign_table_values(table, label, &multiply, a);
            break;
        default:
            // shouldn't happen
            fprintf(stderr, "Unknown graph type.");
            return EINVAL;
    }
    if (ret_code == EXIT_SUCCESS) {
        ret_code = print_table(table);
    }
    free_table(table);
    free(table);
    return ret_code;
}

double plus(double x, double a) {
    return x + a;
}

double minus(double x, double a) {
    return x - a;
}

double multiply(double x, double a) {
    return x * a;
}

double sinus(double x, double a) {
    return sin(x + a);
}

double cosinus(double x, double a) {
    return cos(x + a);
}

