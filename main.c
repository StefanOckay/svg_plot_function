#include "svggraph_input.h"
#include "svggraph_write.h"
#include "svggraph_table.h"


int main(int argc, char *argv[])
{
    if (argc != 1 && argc != 6) {
        fprintf(stderr, "Wrong number of arguments. Exitting.\n");
        return EINVAL;
    }
    FILE *svg_file = NULL;
    int parsed_args[4];
    if (argc == 6) {
        svg_file = fopen(argv[1], "w");
        if (svg_file == NULL) {
            perror(argv[1]);
            return 1;
        }
        if (parse_cml_input(parsed_args, argv) != EXIT_SUCCESS) {
            fclose(svg_file);
            return EINVAL;
        }
        /* WRITING TO SVG FILE BEGIN ------------------------------------ */
        write_basics(svg_file);
        write_wh(svg_file, parsed_args[0], parsed_args[1]);
#ifdef DEBUG
        write_coordinates_translation(svg_file);
#endif
        write_xaxis(svg_file, parsed_args[0], parsed_args[1]);
        write_yaxis(svg_file, parsed_args[0], parsed_args[1]);
        write_xvalues(svg_file, parsed_args[0], parsed_args[1], parsed_args[2]);
        write_yvalues(svg_file, parsed_args[0], parsed_args[1], parsed_args[3]);
        write_zero(svg_file, parsed_args[0], parsed_args[1]);
        write_cut(svg_file, parsed_args[0], parsed_args[1]);
        /* WRITING TO SVG FILE END -------------------------------------- */
    }
    /* USER INPUT BEGIN --------------------------------------------- */
    double *a = calloc(1, sizeof(double)); // graph coefficient
    if (a == NULL) {
        fprintf(stderr, "Couldn't allocate memory. Exitting.\n");
        return ENOMEM;
    }
    graph_type type;
    graph_color rgb_color;
    char user_graph_in[22];
    char fnc_label[22];
    size_t function_counter = 0;
    int ret_code = EXIT_SUCCESS;
    do {
        printf("> ");
        fflush(stdout);
        fgets(user_graph_in, 22, stdin);
        cut_new_line(user_graph_in);
        if (!strcmp("exit", user_graph_in)) {
            break;
        }
        find_graph_type_and_values(&type, user_graph_in, a);
        if (type == ERROR) {
            fprintf(stderr, "Bad function.\n");
            fflush(stderr);
            continue;
        }
        switch (type) {
        case SIN:
        case COS:
            *a = 0;
            break;
        default:
            prov_round(a);
            break;
        }
        if ((ret_code = get_function_label(type, *a, fnc_label))) {
            goto error_return;
        }
        if ((ret_code = print_to_stdout(type, *a, fnc_label))) {
            goto error_return;
        }
        // write to svg file
        if (svg_file != NULL) {
            function_counter++;
            set_color(&rgb_color);
            write_function_label(svg_file, parsed_args[0], fnc_label, function_counter, rgb_color);
            switch (type) {
            case PROD:
            case MINUS:
            case PLUS:
                if ((ret_code = write_linear_line(svg_file, parsed_args, *a, type, rgb_color))) {
                    goto error_return;
                }
                break;
            case SIN:
            case COS:
                if ((ret_code = write_sine_line(svg_file, parsed_args, type, rgb_color))) {
                    goto error_return;
                }
                break;
            default:
                ret_code = EINVAL;
                goto error_return;
                break;
            }
        }
    } while (1);
    /* USER INPUT END -------------------------------------------------- */
    error_return:
    /* CLOSE TAGS, THE FILE AND FREE THE HEAP MEMORY ------------------- */
#ifdef DEBUG
    close_translation(svg_file);
#endif
    free(a);
    if (svg_file != NULL) {
        close_svg(svg_file);
        fclose(svg_file);
    }
    return ret_code;
}
