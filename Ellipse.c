#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function to draw a pixel with symmetry in all four quadrants
void drawEllipsePoints(Display *display, Window window, GC gc, int xc, int yc, int x, int y) {
    XDrawPoint(display, window, gc, xc + x, yc + y);
    XDrawPoint(display, window, gc, xc - x, yc + y);
    XDrawPoint(display, window, gc, xc + x, yc - y);
    XDrawPoint(display, window, gc, xc - x, yc - y);
}

void drawEllipse(Display *display, Window window, GC gc, int xc, int yc, int rx, int ry) {
    int x = 0, y = ry;
    int rx2 = rx * rx;
    int ry2 = ry * ry;
    int tworx2 = 2 * rx2;
    int twory2 = 2 * ry2;

    // Initial decision parameter in region 1
    int p1 = ry2 - (rx2 * ry) + (0.25 * rx2);

    // Region 1
    int px = 0;
    int py = tworx2 * y;
    while (px < py) {
        drawEllipsePoints(display, window, gc, xc, yc, x, y);
        x++;
        px += twory2;
        if (p1 < 0) {
            p1 += ry2 + px;
        } else {
            y--;
            py -= tworx2;
            p1 += ry2 + px - py;
        }
    }

    // Initial decision parameter in region 2
    int p2 = (ry2 * (x + 0.5) * (x + 0.5)) + (rx2 * (y - 1) * (y - 1)) - (rx2 * ry2);

    // Region 2
    while (y > 0) {
        drawEllipsePoints(display, window, gc, xc, yc, x, y);
        y--;
        py -= tworx2;
        if (p2 > 0) {
            p2 += rx2 - py;
        } else {
            x++;
            px += twory2;
            p2 += rx2 - py + px;
        }
    }
}

int main() {
    Display *display;
    Window window;
    int screen;
    GC gc;
    XEvent event;

    int xc, yc, rx, ry;

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

    // Read the values of ellipse center and radii
    printf("Enter xc, yc: ");
    scanf("%d %d", &xc, &yc);
    printf("Enter rx, ry: ");
    scanf("%d %d", &rx, &ry);

    // Main event loop
    while (1) {
        XNextEvent(display, &event);

        if (event.type == Expose) {
            drawEllipse(display, window, gc, xc, yc, rx, ry);
        }

        if (event.type == KeyPress)
            break;
    }

    // Close the X11 display
    XCloseDisplay(display);

    return 0;
}
