#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function to draw a line using Bresenham's line drawing algorithm
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

// Function to draw the coordinate axes
void draw_axes(Display *display, Window window, GC gc, int width, int height) {
    bresenham_line(display, window, gc, width / 2, 0, width / 2, height);
    bresenham_line(display, window, gc, 0, height / 2, width, height / 2);
}

// Function to draw a polygon
void draw_polygon(Display *display, Window window, GC gc, int vertices[][2], int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        int next = (i + 1) % num_vertices;
        bresenham_line(display, window, gc, vertices[i][0], vertices[i][1], vertices[next][0], vertices[next][1]);
    }
}

int main() {
    Display *display;
    Window window;
    int screen;
    GC gc;
    XEvent event;

    // Initialize the X11 display and window
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

    // Original polygon vertices (centered around the origin)
    int original_vertices[4][2] = {{-25, -25}, {25, -25}, {25, 25}, {-25, 25}};
    int num_vertices = 4;

    // Center of the screen
    int cx = 400, cy = 300;

    // Adjust vertices to center on screen
    for (int i = 0; i < num_vertices; i++) {
        original_vertices[i][0] += cx;
        original_vertices[i][1] += cy;
    }

    // Translation amounts
    int translate_x = 100;
    int translate_y = 50;

    // Translated polygon vertices
    int translated_vertices[4][2];

    // Apply translation to each vertex
    for (int i = 0; i < num_vertices; i++) {
        translated_vertices[i][0] = original_vertices[i][0] + translate_x;
        translated_vertices[i][1] = original_vertices[i][1] + translate_y;
    }

    // Main event loop
    while (1) {
        XNextEvent(display, &event);

        if (event.type == Expose) {
            // Clear the window
            XClearWindow(display, window);

            // Draw the coordinate axes
            draw_axes(display, window, gc, 800, 600);

            // Draw the original polygon
            XSetForeground(display, gc, 0xFF0000); // Red color
            draw_polygon(display, window, gc, original_vertices, num_vertices);

            // Draw the translated polygon
            XSetForeground(display, gc, 0x0000FF); // Blue color
            draw_polygon(display, window, gc, translated_vertices, num_vertices);
        }

        if (event.type == KeyPress)
            break;
    }

    // Close the X11 display
    XCloseDisplay(display);

    return 0;
}
