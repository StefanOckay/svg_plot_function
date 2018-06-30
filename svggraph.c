#include "svggraph.h"


struct Parsed_double {
    int error;
    double real_number;
};

struct Table {
    unsigned char rows;
    unsigned char collumns;
    char **cells;
};

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
            if (table->cells[r * table->collumns + c] != NULL) {
                free(table->cells[r * table->collumns + c]);
            }
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

void cut_new_line(char *s) {
    int i = 0;
    while (s[i] != '\n') {
        if(s[i] == '\0') {
            return;
        }
        i++;
    }
    s[i] = '\0';
}

int is_valid_char(char c, const char *valid_chars) {
    int i = 0;
    while (valid_chars[i] != '\0') {
        if (c == valid_chars[i]) {
            return 0;
        }
        i++;
    }
    return -1;
}

struct Parsed_double parse_user_input(const char *graph_const, const char *user_graph_input) {
    struct Parsed_double result;
    int i = 0;
    int j = 0;
    while (graph_const[i] != '%') {
        if (graph_const[i] != user_graph_input[j]) {
            goto error_return;
        }
        i++;
        j++;
    }
    i += 2;
    char a_string[20];
    char *valid_chars;
    if (!strcmp(graph_const, "y = x + %f") || !strcmp(graph_const, "y = x - %f")) {
        valid_chars = ".0123456789";
    } else {
        valid_chars = "-.0123456789";
    }
    int k = 0;
    int found_dot = 0;
    while (!is_valid_char(user_graph_input[j], valid_chars)) {
        if (user_graph_input[j] == '.' && k == 0) {
            goto error_return;
        }
        if (user_graph_input[j] == '-' && k == 0 && user_graph_input[j + 1] == '.') {
            goto error_return;
        }
        if (user_graph_input[j] == '.') {
            if (found_dot) {
                goto error_return;
            }
            found_dot = 1;
        }
        if (user_graph_input[j] == '-' && k > 0) {
            goto error_return;
        }
        a_string[k] = user_graph_input[j];
        j++;
        k++;
    }
    if (user_graph_input[j - 1] == '.') {
        goto error_return;
    }
    a_string[k] = '\0';
    if (a_string[0] == '\0') {
        goto error_return;
    }
    if (!strcmp("-", a_string)) {
        goto error_return;
    }
    while (graph_const[i] != '\0') {
        if (graph_const[i] != user_graph_input[j]) {
            goto error_return;
        }
        i++;
        j++;
    }
    if (user_graph_input[j] != '\0') {
        goto error_return;
    }
    double a = atof(a_string); // only valid argument is expected at this point
    result.real_number = a;
    result.error = 0;
    return result;
    error_return:
        result.error = 1;
        return result;
}

void find_graph_type(enum graph_type *graph_type, const char *user_graph_input, double *a) {
    if (!strcmp("y = sin x", user_graph_input)) {
        *graph_type = SIN;
        *a = 0;
        return;
    }
    if (!strcmp("y = cos x", user_graph_input)) {
        *graph_type = COS;
        *a = 0;
        return;
    }
    struct Parsed_double a_struct;
    if ((a_struct = parse_user_input("y = %f x", user_graph_input)).error == 0) {
        *graph_type = PROD;
        *a = a_struct.real_number;
        return;
    }
    if ((a_struct = parse_user_input("y = x + %f", user_graph_input)).error == 0) {
        *graph_type = PLUS;
        *a = a_struct.real_number;
        return;
    }
    if ((a_struct = parse_user_input("y = x - %f", user_graph_input)).error == 0) {
        *graph_type = MINUS;
        *a = a_struct.real_number;
        return;
    }
    // input does not fit any graph type
    *graph_type = ERROR;
}

int parse_cml_input(int *graph_in_values, char *argv[]) {
    for (int i = 2; i < 6; i++) {
        if ((graph_in_values[i - 2] = atoi(argv[i])) <= 0) {
            fprintf(stderr, "Invalid input value number %i: %s\n", i - 1, argv[i]);
            return EXIT_FAILURE;
        }
    }
    return 0;
}

void write_basics(FILE *svg_file) {
    fprintf(svg_file,
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" "
            "\"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n"
            "<svg xmlns=\"http://www.w3.org/2000/svg\""
            " xmlns:xlink=\"http://www.w3.org/1999/xlink\" ");
}

void write_wh(FILE *svg_file, int width, int height) {
    double fwidth = (double)width;
    double fheight = (double)height;
#ifdef DEBUG
    fwidth = fwidth + 2 * OFFSET;
    fheight = fheight + 2 * OFFSET;
#endif
    fprintf(svg_file, "width=\"%.0f\" height=\"%.0f\">\n", fwidth, fheight);
}

void write_coordinates_translation(FILE *svg_file) {
    fprintf(svg_file, "<g transform=\"translate(%i, %i)\">\n", OFFSET, OFFSET);
}

void write_xaxis(FILE *svg_file, int width, int height) {
    double fwidth = (double)width;
    double fheight = (double)height;
    fprintf(svg_file, "<line x1=\"0\" y1=\"%.0f\" x2=\"%.0f\" y2=\"%.0f\""
                      " stroke=\"rgb(100, 100, 100)\" stroke-width=\"3\"/>\n",
            fheight / 2, fwidth, fheight / 2);
}

void write_yaxis(FILE *svg_file, int width, int height) {
    double fwidth = (double)width;
    double fheight = (double)height;
    fprintf(svg_file, "<line x1=\"%.0f\" y1=\"0\" x2=\"%.0f\" y2=\"%.0f\""
                      " stroke=\"rgb(100, 100, 100)\" stroke-width=\"3\"/>\n",
            fwidth / 2, fwidth / 2, fheight);
}

void write_xvalues(FILE *svg_file, int width, int height, int x) {
    double fwidth = (double)width;
    double fheight = (double)height;
    double dist =  fwidth / (double)(2 * x);
    for (int i = -x; i <= x; i++) {
        if (i == 0) {
            continue;
        }
        fprintf(svg_file, "<text x=\"%.0f\" y=\"%.0f\""
                          " text-anchor=\"middle\" fill=\"black\">%i</text>\n",
               (i + x) * dist - 4, fheight / 2 - 2, i);
    }

}

void write_yvalues(FILE *svg_file, int width, int height, int y) {
    double fwidth = (double)width;
    double fheight = (double)height;
    double dist =  fheight / (double)(2 * y);
    for (int i = -y; i <= y; i++) {
        if (i == 0) {
            continue;
        }
        fprintf(svg_file, "<text x=\"%.0f\" y=\"%.0f\" fill=\"black\">%i</text>\n",
               fwidth / 2 + 2, (y - i) * dist + 5, i);
    }

}

void write_zero(FILE *svg_file, int width, int height) {
    double fwidth = (double)width;
    double fheight = (double)height;
    fprintf(svg_file, "<text x=\"%.0f\" y=\"%.0f\" fill=\"black\">0</text>\n",
           fwidth / 2 + 2, fheight / 2 - 2);
}

void write_cut(FILE *svg_file, int width, int height) {
    fprintf(svg_file, "<defs>\n"
                      "<clipPath id=\"myClip\">\n"
                      "<rect x=\"0\" y=\"0\" width=\"%i\" height=\"%i\"/>\n"
                      "</clipPath>\n</defs>\n", width, height);
}

double map_to_ycoordinate(int y_range, double what, int height) {
    double dheight = (double)height;
    double dy_range = (double)y_range;
    double dist =  dheight / (double)(2 * dy_range);
    return (dy_range - what) * dist;
}

double map_to_xcoordinate(int x_range, double what, int width) {
    double dwidth = (double)width;
    double dx_range = (double)x_range;
    double dist =  dwidth / (double)(2 * dx_range);
    return (dx_range + what) * dist;
}

void write_linear_line(FILE *svg_file, int width, int height, int x, int y, double a, enum graph_type graph_type, struct graph_color color) {
    double dwidth = (double)width;
    double fx1;
    double fx2;
    double fx1_map;
    double fx2_map;
    switch (graph_type) {
        case PROD:
            fx1 = a * (double)(-x);
            fx1_map = map_to_ycoordinate(y, fx1, height);
            fx2 = a * (double)x;
            fx2_map = map_to_ycoordinate(y, fx2, height);
            break;
        case PLUS:
            fx1 = (double)(-x) + a;
            fx1_map = map_to_ycoordinate(y, fx1, height);
            fx2 = (double)x + a;
            fx2_map = map_to_ycoordinate(y, fx2, height);
            break;
        case MINUS:
            fx1 = (double)(-x) - a;
            fx1_map = map_to_ycoordinate(y, fx1, height);
            fx2 = (double)x - a;
            fx2_map = map_to_ycoordinate(y, fx2, height);
            break;
        default:
            break;
    }
    fprintf(svg_file, "<line clip-path=\"url(#myClip)\" "
                      "x1=\"0\" y1=\"%.0f\" "
                      "x2=\"%.0f\" y2=\"%.0f\" "
                      "fill=\"transparent\" stroke=\"rgb(%i, %i, %i)\" stroke-width=\"1\"/>\n",
            fx1_map, dwidth, fx2_map, color.red, color.green, color.blue);
}

void write_sine_line(FILE *svg_file, int width, int height, int x, int y, enum graph_type graph_type, struct graph_color color) {
    double dx = (double)x;
    int count = 1;
    double offset = 0;
    while (count * PI_2 < dx) {
        count++;
    }
    if (count % 2 == 0) {
        count++;
    }
    int sgn = 1;
    if ((count / 2) % 2 == 0) {
        sgn = -1;
    }
    if (graph_type == COS) {
        offset = PI_2;
    }
    double lower_PI = -count * PI_2 - offset;
    double x1;
    double x2;
    double x3;
    double x4;
    double fx1;
    double fx2;
    double fx3;
    double fx4;
    double x1_map;
    double x2_map;
    double x3_map;
    double x4_map;
    double fx1_map;
    double fx2_map;
    double fx3_map;
    double fx4_map;
    if (graph_type == COS) {
        count++;
    }
    for (int i = 0; i < count; i++) {
        x1 = lower_PI + PI * i;
        x2 =  x1 + PI * 0.364212423249;
        x3 = x1 + PI * 0.635787576751;
        x4 = x1 + PI;
        fx1 = sgn;
        fx2 = sgn;
        fx3 = -sgn;
        fx4 = -sgn;
        x1_map = map_to_xcoordinate(x, x1, width);
        x2_map = map_to_xcoordinate(x, x2, width);
        x3_map = map_to_xcoordinate(x, x3, width);
        x4_map = map_to_xcoordinate(x, x4, width);
        fx1_map = map_to_ycoordinate(y, fx1, height);
        fx2_map = map_to_ycoordinate(y, fx2, height);
        fx3_map = map_to_ycoordinate(y, fx3, height);
        fx4_map = map_to_ycoordinate(y, fx4, height);
        fprintf(svg_file, "<path clip-path=\"url(#myClip)\" "
                          "d=\"M %f %f C %f,%f %f,%f %f,%f\" "
                          "fill=\"transparent\" stroke=\"rgb(%i, %i, %i)\" stroke-width=\"1\"/>\n",
                x1_map, fx1_map, x2_map, fx2_map, x3_map, fx3_map, x4_map, fx4_map, color.red, color.green, color.blue);
        if (sgn == -1) {
            sgn = 1;
        } else {
            sgn = -1;
        }
    }
}

void write_function_label(FILE *svg_file, int width, char *user_graph_input, int function_n, struct graph_color color) {
    fprintf(svg_file, "<text x=\"%i\" y=\"%i\" text-anchor=\"end\" font-size=\"18\" fill=\"rgb(%i, %i, %i)\">%s</text>\n",
            width - 2, function_n * 20, color.red, color.green, color.blue, user_graph_input);
}

void close_svg(FILE *svg_file) {
    if (svg_file != NULL)
        fprintf(svg_file, "</svg>");
}

void close_translation(FILE *svg_file) {
    if (svg_file != NULL)
        fprintf(svg_file, "</g>\n");
}
