#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void bresenham_line(Display *display, Window window, GC gc, int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        XDrawPoint(display, window, gc, x1, y1);
        if (x1 == x2 && y1 == y2)
            break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void draw_axes(Display *display, Window window, GC gc, int width, int height) {
    bresenham_line(display, window, gc, width / 2, 0, width / 2, height);
    bresenham_line(display, window, gc, 0, height / 2, width, height / 2);
}

void draw_polygon(Display *display, Window window, GC gc, int vertices[][2], int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        int next = (i + 1) % num_vertices;
        bresenham_line(display, window, gc, vertices[i][0], vertices[i][1], vertices[next][0], vertices[next][1]);
    }
}

void rotate(int vertices[][2], int num_vertices, float angle, int result[][2], int cx, int cy) {
    float rad = angle * M_PI / 180.0;
    for (int i = 0; i < num_vertices; i++) {
        int x = vertices[i][0] - cx;
        int y = vertices[i][1] - cy;
        result[i][0] = cx + (x * cos(rad) - y * sin(rad));
        result[i][1] = cy + (x * sin(rad) + y * cos(rad));
    }
}

int main() {
    Display *display;
    Window window;
    int screen;
    GC gc;
    XEvent event;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Unable to open X display\n");
        exit(1);
    }

    screen = DefaultScreen(display);
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, 800, 600, 1,
                                 BlackPixel(display, screen), WhitePixel(display, screen));

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    gc = XCreateGC(display, window, 0, NULL);

    int vertices[4][2] = {{-50, -50}, {50, -50}, {50, 50}, {-50, 50}};
    int num_vertices = 4;

    int cx = 400, cy = 300;

    for (int i = 0; i < num_vertices; i++) {
        vertices[i][0] += cx;
        vertices[i][1] += cy;
    }

    float angle = 45;

    int rotated_vertices[4][2];

    while (1) {
        XNextEvent(display, &event);

        if (event.type == Expose) {
            XClearWindow(display, window);

            draw_axes(display, window, gc, 800, 600);

            XSetForeground(display, gc, 0xFF0000);
            draw_polygon(display, window, gc, vertices, num_vertices);

            rotate(vertices, num_vertices, angle, rotated_vertices, cx, cy);

            XSetForeground(display, gc, 0x0000FF);
            draw_polygon(display, window, gc, rotated_vertices, num_vertices);
        }

        if (event.type == KeyPress)
            break;
    }

    XCloseDisplay(display);

    return 0;
}
