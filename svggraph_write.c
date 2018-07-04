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
    while (count * M_PI_2 < dx) {
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
        offset = M_PI_2;
    }
    double lower_PI = -count * M_PI_2 - offset;
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
        x1 = lower_PI + M_PI * i;
        x2 =  x1 + M_PI * BEZ_SINE_APR;
        x3 = x1 + M_PI * (1 - BEZ_SINE_APR);
        x4 = x1 + M_PI;
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
