#include "svggraph_write.h"


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
    char s[128u] = "<line x1=\"0\" y1=\"%.0f\" x2=\"%.0f\" y2=\"%.0f\""
                  " stroke=\"rgb(100, 100, 100)\" stroke-width=\"3\"/>\n";
    fprintf(svg_file, s, fheight / 2, fwidth, fheight / 2);
}

void write_yaxis(FILE *svg_file, int width, int height) {
    double fwidth = (double)width;
    double fheight = (double)height;
    char s[128u] = "<line x1=\"%.0f\" y1=\"0\" x2=\"%.0f\" y2=\"%.0f\""
                  " stroke=\"rgb(100, 100, 100)\" stroke-width=\"3\"/>\n";
    fprintf(svg_file, s, fwidth / 2, fwidth / 2, fheight);
}

void write_xvalues(FILE *svg_file, int width, int height, int x) {
    double fwidth = (double)width;
    double fheight = (double)height;
    double dist =  fwidth / (double)(2 * x);
    char s[128u] = "<text x=\"%.0f\" y=\"%.0f\""
                 " text-anchor=\"middle\" fill=\"black\">%i</text>\n";
    for (int i = -x; i <= x; i++) {
        if (i != 0) {
            fprintf(svg_file, s, (i + x) * dist - 4, fheight / 2 - 2, i);
        }
    }
}

void write_yvalues(FILE *svg_file, int width, int height, int y) {
    double fwidth = (double)width;
    double fheight = (double)height;
    double dist =  fheight / (double)(2 * y);
    char s[64u] = "<text x=\"%.0f\" y=\"%.0f\" fill=\"black\">%i</text>\n";
    for (int i = -y; i <= y; i++) {
        if (i != 0) {
            fprintf(svg_file, s, fwidth / 2 + 2, (y - i) * dist + 5, i);
        }
    }
}

void write_zero(FILE *svg_file, int width, int height) {
    double fwidth = (double)width;
    double fheight = (double)height;
    char s[64u] = "<text x=\"%.0f\" y=\"%.0f\" fill=\"black\">0</text>\n";
    fprintf(svg_file, s, fwidth / 2 + 2, fheight / 2 - 2);
}

void write_cut(FILE *svg_file, int width, int height) {
    char s[128u] = "<defs>\n"
                  "<clipPath id=\"myClip\">\n"
                  "<rect x=\"0\" y=\"0\" width=\"%i\" height=\"%i\"/>\n"
                  "</clipPath>\n</defs>\n";
    fprintf(svg_file, s, width, height);
}

double map_to_ycoor(double y_range, double what, double height) {
    double dist =  height / (2 * y_range);
    return (y_range - what) * dist;
}

double map_to_xcoor(double x_range, double what, double width) {
    double dist =  width / (2 * x_range);
    return (x_range + what) * dist;
}

int write_linear_line(FILE *svg_file, int *parsed_args, double a, graph_type type, graph_color color) {
    double width = (double) parsed_args[0];
    double height = (double) parsed_args[1];
    double x = (double) parsed_args[2];
    double y = (double) parsed_args[3];
    double fx1;
    double fx2;
    switch (type) {
        case PROD:
            fx1 = a * (-x);
            fx2 = a * x;
            break;
        case PLUS:
            fx1 = -x + a;
            fx2 = x + a;
            break;
        case MINUS:
            fx1 = -x - a;
            fx2 = x - a;
            break;
        default:
            fprintf(stderr, "\"write_linear_line\" works only for PROD, PLUS, MINUS.\n");
            fflush(stderr);
            return EINVAL;
            break;
    }
    double fx1_map = map_to_ycoor(y, fx1, height);
    double fx2_map = map_to_ycoor(y, fx2, height);
    fprintf(svg_file, "<line clip-path=\"url(#myClip)\" "
                      "x1=\"0\" y1=\"%.0f\" "
                      "x2=\"%.0f\" y2=\"%.0f\" "
                      "fill=\"transparent\" stroke=\"rgb(%i, %i, %i)\" stroke-width=\"1\"/>\n",
            fx1_map, width, fx2_map, color.red, color.green, color.blue);
    return EXIT_SUCCESS;
}

int write_sine_line(FILE *svg_file, int *parsed_args, graph_type type, graph_color color) {
    if (type != SIN && type != COS) {
        fprintf(stderr, "\"write_sine_line\" works only for sine and cosine.\n");
        fflush(stderr);
        return EINVAL;
    }
    double width = (double) parsed_args[0];
    double height = (double) parsed_args[1];
    double x_range = (double) parsed_args[2];
    double y_range = (double) parsed_args[3];
    int count = 1;
    double offset = 0;
    int sgn = 1;
    while (count * M_PI_2 < x_range) {
        count++;
    }
    if (count % 2 == 0) {
        count++;
    }
    if ((count / 2) % 2 == 0) {
        sgn = -1;
    }
    if (type == COS) {
        offset = M_PI_2;
    }
    double lower_PI = -count * M_PI_2 - offset;
    double x[4];
    double fx[4];
    double x_map[4];
    double fx_map[4];
    if (type == COS) {
        count++;
    }
    for (int i = 0; i < count; i++) {
        x[0] = lower_PI + M_PI * i;
        x[1] =  x[0] + M_PI * BEZ_SINE_APP;
        x[2] = x[0] + M_PI * (1 - BEZ_SINE_APP);
        x[3] = x[0] + M_PI;
        fx[0] = sgn;
        fx[1] = sgn;
        fx[2] = -sgn;
        fx[3] = -sgn;
        for (int j = 0; j < 4; j++) {
            x_map[j] = map_to_xcoor(x_range, x[j], width);
            fx_map[j] = map_to_ycoor(y_range, fx[j], height);
        }
        char s[128u] = "<path clip-path=\"url(#myClip)\" "
                      "d=\"M %f %f C %f,%f %f,%f %f,%f\" "
                      "fill=\"transparent\" stroke=\"rgb(%i, %i, %i)\" stroke-width=\"1\"/>\n";
        fprintf(svg_file, s,
                x_map[0], fx_map[0], x_map[1], fx_map[1], x_map[2], fx_map[2], x_map[3], fx_map[3],
                color.red, color.green, color.blue);
        if (sgn == -1) {
            sgn = 1;
        } else {
            sgn = -1;
        }
    }
    return EXIT_SUCCESS;
}

void write_function_label(FILE *svg_file, int width, char *user_graph_in, int function_n, graph_color color) {
    char s[128u] = "<text x=\"%i\" y=\"%i\" text-anchor=\"end\" font-size=\"18\" fill=\"rgb(%i, %i, %i)\">%s</text>\n";
    fprintf(svg_file, s, width - 2, function_n * 20, color.red, color.green, color.blue, user_graph_in);
}

void close_svg(FILE *svg_file) {
    if (svg_file != NULL) {
        fprintf(svg_file, "</svg>");
    }
}

void close_translation(FILE *svg_file) {
    if (svg_file != NULL) {
        fprintf(svg_file, "</g>\n");
    }
}
