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

void destroy_table(struct Table *table) {
    for (unsigned int r = 0; r < table->rows; r++) {
        for (unsigned int c = 0; c < table->collumns; c++) {
            free(table->cells[r * table->collumns + c]);
        }
    }
    free(table->cells);
}

void print_horizontal_table_border(struct Table *table, size_t *collun_widths) {
    putchar('+');
    for(size_t c = 0; c < table->collumns; c++) {
        for(size_t j = 0; j < collun_widths[c]; j++) {
            putchar('-');
        }
        putchar('+');
    }
    putchar('\n');
}

void print_table_line(struct Table *table, unsigned char row, size_t *collun_widths) {
    putchar('|');
    for(size_t c = 0; c < table->collumns; c++){
        if(table->cells[row * table->collumns + c] == NULL) {
            printf("%-*s", (int)collun_widths[c], "");
        } else {
            printf("%-*s", (int)collun_widths[c], table->cells[row * table->collumns + c]);
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
    for (unsigned int r = 0; r < table->rows; r++) {
        for (unsigned int c = 0; c < table->collumns; c++) {
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
    return SUCCESS;
}

int assignCellI(struct Table *table, unsigned char row, unsigned char collumn, int value) {
    if (row >= table->rows || collumn >= table->collumns ) {
        return EINVAL;
    }
    char buffer[21];
    sprintf(buffer, "%i", value);
    size_t len = strlen(buffer);
    char *str = calloc((len + 1), sizeof(char));
    if (str == NULL) {
        return ENOMEM;
    }
    if (table->cells[row*table->collumns+collumn] != NULL) {
        free(table->cells[row * table->collumns + collumn]);
    }
    strcpy(str, buffer);
    table->cells[row * table->collumns + collumn] = str;
    return 0;
}

int simplify(char *str) {
    int len = strlen(str);
    for(int i = len - 1; len >0; i--) {
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

int assignCellF(struct Table *table, unsigned char row, unsigned char collumn, double value) {
    if (row >= table->rows || collumn >= table->collumns ) {
        return EINVAL;
    }
    char buffer[21];
    sprintf(buffer, "%.2f",value);
    int len = simplify(buffer);
    char *str = calloc((len + 1), sizeof(char));
    if (str == NULL) {
        return ENOMEM;
    }
    if (table->cells[row * table->collumns+collumn] != NULL) {
        free(table->cells[row * table->collumns + collumn]);
    }
    if (value == 0) {
        strcpy(str, "0");
    } else {
        strcpy(str, buffer);
    }
    table->cells[row * table->collumns + collumn] = str;
    return 0;
}

int assignCellS(struct Table *table, unsigned char row, unsigned char collumn, char* value) {
    if (row >= table->rows || collumn >= table->collumns ) {
        return EINVAL;
    }
    size_t len = strlen(value);
    char *str = calloc((len + 1), sizeof(char));
    if (str == NULL){
        return ENOMEM;
    }
    if (table->cells[row*table->collumns+collumn] != NULL) {
        free(table->cells[row * table->collumns + collumn]);
    }
    strcpy(str, value);
    table->cells[row * table->collumns + collumn] = str;
    return 0;
}

int print_table_for_function(char *function_str, double (*function) (double, double), double parameter) {
    struct Table *table = new_table(2, 7);
    if (table == NULL){
        return(ENOMEM);
    }
    int ret_code = 0;
    if ((ret_code = assignCellS(table, 0, 0, "x"))) {
        goto cleanup;
    }
    if ((ret_code = assignCellS(table, 1, 0, function_str))) {
        goto cleanup;
    }
    for (int x = 0; x <= 5; x++){
        if ((ret_code = assignCellI(table, 0, x + 1, x))) {
            goto cleanup;
        }
        if ((ret_code = assignCellF(table, 1, x + 1, function(x,parameter)))) {
            goto cleanup;
        }
    }
    if ((ret_code = print_table(table))) {
        goto cleanup;
    }
    ret_code = SUCCESS;
    cleanup:
        destroy_table(table);
        free(table);
        return ret_code;
}


int print_to_stdout(enum graph_type type, double parameter) {
    char label[21];
    int ret_code;
    switch (type) {
        case SIN:
            ret_code = print_table_for_function("sin x", &sinus, 0);
            break;
        case COS:
            ret_code = print_table_for_function("cos x", &cosinus, 0);
            break;
        case PLUS:
            sprintf(label,"x + %.2f", parameter);
            simplify(label);
            ret_code = print_table_for_function(label, &plus, parameter);
            break;
        case MINUS:
            sprintf(label,"x - %.2f", parameter);
            simplify(label);
            ret_code = print_table_for_function(label, &minus, parameter);
            break;
        case PROD:
            sprintf(label,"%.2f", parameter);
            simplify(label);
            strcat(label," x");
            ret_code = print_table_for_function(label, &multiply, parameter);
            break;
        default:
            //should never hapen
            fprintf(stderr,"Switch control unknown value");
            return EINVAL;
    }
    return ret_code;
}

double plus(double x, double parameter) {
    return x + parameter;
}

double minus(double x, double parameter) {
    return x - parameter;
}

double multiply(double x, double parameter) {
    return x * parameter;
}

double sinus(double x, double parameter) {
    return sin(x + parameter);
}

double cosinus(double x, double parameter) {
    return cos(x + parameter);
}

