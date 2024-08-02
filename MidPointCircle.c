#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function to draw a pixel with symmetry in all eight octants
void drawCirclePoints(Display *display, Window window, GC gc, int xc, int yc, int x, int y) {
    XDrawPoint(display, window, gc, xc + x, yc + y);
    XDrawPoint(display, window, gc, xc - x, yc + y);
    XDrawPoint(display, window, gc, xc + x, yc - y);
    XDrawPoint(display, window, gc, xc - x, yc - y);
    XDrawPoint(display, window, gc, xc + y, yc + x);
    XDrawPoint(display, window, gc, xc - y, yc + x);
    XDrawPoint(display, window, gc, xc + y, yc - x);
    XDrawPoint(display, window, gc, xc - y, yc - x);
}

void drawCircle(Display *display, Window window, GC gc, int xc, int yc, int r) {
    int x = 0, y = r;
    int p = 1 - r; // Initial decision parameter P0 = (5/4) - r

    drawCirclePoints(display, window, gc, xc, yc, x, y);

    // Iterate until x >= y
    while (x < y) {
        x++;
        if (p < 0) {
            p += 2 * x + 1; // Pk + 1 = Pk + 2xk + 1
        } else {
            y--;
            p += 2 * (x - y) + 1; // Pk + 1 = Pk + 2xk + 1 - 2yk + 1
        }
        // Plot the points for this step
        drawCirclePoints(display, window, gc, xc, yc, x, y);
    }
}

int main() {
    Display *display;
    Window window;
    int screen;
    GC gc;
    XEvent event;

    int xc, yc, r;

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

    // Read the values of circle center and radius
    printf("Enter xc, yc: ");
    scanf("%d %d", &xc, &yc);
    printf("Enter radius r: ");
    scanf("%d", &r);

    // Main event loop
    while (1) {
        XNextEvent(display, &event);

        if (event.type == Expose) {
            drawCircle(display, window, gc, xc, yc, r);
        }

        if (event.type == KeyPress)
            break;
    }

    // Close the X11 display
    XCloseDisplay(display);

    return 0;
}
