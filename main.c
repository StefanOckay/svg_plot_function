#include "svggraph.h"


int main(int argc, char *argv[])
{
    if (argc != 1 && argc != 6) {
        fprintf(stderr, "Wrong number of arguments. Exitting.\n");
        return -1;
    }
    FILE *svg_file = NULL;
    int graph_in_values[4];
    if (argc == 6) {
        svg_file = fopen(argv[1], "w");
        if (svg_file == NULL) {
            perror(argv[1]);
            return -1;
        }
        // parse command line input
        if (parse_cml_input(graph_in_values, argv) != 0) {
            fclose(svg_file);
            return -1;
        }
        // WRITING TO SVG FILE BEGIN -------------------------
        // basics
        write_basics(svg_file);
        // width and height of the svg file
        write_wh(svg_file, graph_in_values[0], graph_in_values[1]);
        // translation of the coordinates
#ifdef DEBUG
        write_coordinates_translation(svg_file);
#endif
        // axis
        write_xaxis(svg_file, graph_in_values[0], graph_in_values[1]);
        write_yaxis(svg_file, graph_in_values[0], graph_in_values[1]);
        // axis values
        write_xvalues(svg_file, graph_in_values[0], graph_in_values[1], graph_in_values[2]);
        write_yvalues(svg_file, graph_in_values[0], graph_in_values[1], graph_in_values[3]);
        write_zero(svg_file, graph_in_values[0], graph_in_values[1]);
        write_cut(svg_file, graph_in_values[0], graph_in_values[1]);
        // WRITING TO SVG FILE END -------------------------
    }
    // USER INPUT BEGIN ----------------------------------
    // declare useful variables
    enum graph_type graph_type;
    int function_counter = 0;
    struct graph_color graph_color;
    double *a = calloc(1, sizeof(double)); // graph coefficient
    char user_graph_input[22];
    char a_buffer[22];
    // initialize input
    printf("> ");
    fflush(stdout); // c crap
    fgets(user_graph_input, 22, stdin);
    cut_new_line(user_graph_input);
    // continue with cycle
    while (strcmp("exit", user_graph_input)) {
        find_graph_type(&graph_type, user_graph_input, a);
        if (graph_type == ERROR) {
            fprintf(stderr, "Bad function.\n");
            fflush(stderr);
            goto continue_print;
        }
        if (graph_type == SIN || graph_type == COS) {
            *a = 0;
        }
        sprintf(a_buffer, "%.2f", *a); // rounding
        sscanf(a_buffer, "%lf", a);    // rounding
        // print to svg file
        if (svg_file != NULL) {
            function_counter++;
            graph_color.red = rand() % 200;
            graph_color.green = rand() % 200;
            graph_color.blue = rand() % 200;
            write_function_label(svg_file, graph_in_values[0], user_graph_input, function_counter, graph_color);
            if (graph_type != SIN && graph_type != COS) {
                write_linear_line(svg_file, graph_in_values[0], graph_in_values[1], graph_in_values[2], graph_in_values[3], *a, graph_type, graph_color);
            } else {
                write_sine_line(svg_file, graph_in_values[0], graph_in_values[1], graph_in_values[2], graph_in_values[3], graph_type, graph_color);
            }
        }
        // print to stdout
        print_to_stdout(graph_type, *a);
    continue_print:
        printf("> ");
        fflush(stdout);
        fgets(user_graph_input, 22, stdin);
        cut_new_line(user_graph_input);
    }
        // USER INPUT END ------------------------------------
        // CLOSE TAGS, THE FILE AND FREE HEAP MEMORY ---------------
#ifdef DEBUG
    close_translation(svg_file);
#endif
    free(a);
    if (svg_file != NULL) {
        close_svg(svg_file);
        fclose(svg_file);
    }
    // SUCCESSFULLY FINISH THE PROGRAM -------------------------
    return 0;
}
