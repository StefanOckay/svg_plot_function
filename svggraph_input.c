#include "svggraph_input.h"


void prov_round(double *x) {
    char x_buffer[22];
    sprintf(x_buffer, "%.2f", *x);
    sscanf(x_buffer, "%lf", x);
}

void set_color(graph_color *rgb_color) {
    rgb_color->red = rand() % 200;
    rgb_color->green = rand() % 200;
    rgb_color->blue = rand() % 200;
}

void cut_new_line(char *s) {
    unsigned char i = 0;
    while (s[i] != '\n') {
        if(s[i] == '\0') {
            return;
        }
        i++;
    }
    s[i] = '\0';
}

int is_valid_char(char c, const char *valid_chars) {
    unsigned char i = 0;
    while (valid_chars[i] != '\0') {
        if (c == valid_chars[i]) {
            return 0;
        }
        i++;
    }
    return 1;
}

int parse_user_input(const char *graph_const, const char *user_graph_in, double *a) {
    unsigned char i = 0;
    unsigned char j = 0;
    while (graph_const[i] != '%') {
        if (graph_const[i] != user_graph_in[j]) {
            return EINVAL;
        }
        i++;
        j++;
    }
    i += 2;
    char a_string[22];
    char *valid_chars;
    unsigned char k = 0;
    unsigned char found_dot = 0;
    if (!strcmp(graph_const, "y = x + %f") || !strcmp(graph_const, "y = x - %f")) {
        valid_chars = ".0123456789";
    } else {
        valid_chars = "-.0123456789";
    }
    while (!is_valid_char(user_graph_in[j], valid_chars)) {
        //valid char is not enough, positioning is also important
        if (user_graph_in[j] == '.' && k == 0) {
            return EINVAL;
        }
        if (user_graph_in[j] == '-' && k == 0 && user_graph_in[j + 1] == '.') {
            return EINVAL;
        }
        if (user_graph_in[j] == '.') {
            if (found_dot) {
                return EINVAL;
            }
            found_dot = 1;
        }
        if (user_graph_in[j] == '-' && k > 0) {
            return EINVAL;
        }
        a_string[k] = user_graph_in[j];
        j++;
        k++;
    }
    if (user_graph_in[j - 1] == '.') {
        return EINVAL;
    }
    a_string[k] = '\0';
    if (a_string[0] == '\0') {
        return EINVAL;
    }
    if (!strcmp("-", a_string)) {
        return EINVAL;
    }
    while (graph_const[i] != '\0') {
        if (graph_const[i] != user_graph_in[j]) {
            return EINVAL;
        }
        i++;
        j++;
    }
    if (user_graph_in[j] != '\0') {
        return EINVAL;
    }
    *a = atof(a_string); // only valid argument is expected at this point
    return EXIT_SUCCESS;
}

void find_graph_type_and_values(graph_type *type, const char *user_graph_input, double *a) {
    if (!strcmp("y = sin x", user_graph_input)) {
        *type = SIN;
        *a = 0;
        return;
    }
    if (!strcmp("y = cos x", user_graph_input)) {
        *type = COS;
        *a = 0;
        return;
    }
    if (!parse_user_input("y = %f x", user_graph_input, a)) {
        *type = PROD;
        return;
    }
    if (!parse_user_input("y = x + %f", user_graph_input, a)) {
        *type = PLUS;
        return;
    }
    if (!parse_user_input("y = x - %f", user_graph_input, a)) {
        *type = MINUS;
        return;
    }
    *type = ERROR;
}

int parse_cml_input(int *parsed_args, char *argv[]) {
    for (unsigned char i = 2; i < 6; i++) {
        if ((parsed_args[i - 2] = atoi(argv[i])) <= 0) {
            // prints only the first wrong
            fprintf(stderr, "Invalid input value number %i: %s\n", i - 1, argv[i]);
            return EINVAL;
        }
    }
    return EXIT_SUCCESS;
}
