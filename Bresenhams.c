#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>

void drawLine(Display *display, Window window, GC gc, int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int x = x1;
    int y = y1;
    
    int sx = (dx > 0) ? 1 : -1; 
    int sy = (dy > 0) ? 1 : -1; 
    
    dx = abs(dx);
    dy = abs(dy);
    
    int p;

    if (dx > dy) {
        p = 2 * dy - dx;
        while (x != x2) {
            XDrawPoint(display, window, gc, x, y);
            x += sx;
            if (p < 0) {
                p += 2 * dy;
            } else {
                y += sy;
                p += 2 * (dy - dx);
            }
        }
    } else { 
        p = 2 * dx - dy;
        while (y != y2) {
            XDrawPoint(display, window, gc, x, y);
            y += sy;
            if (p < 0) {
                p += 2 * dx;
            } else {
                x += sx;
                p += 2 * (dx - dy);
            }
        }
    }

    XDrawPoint(display, window, gc, x, y);
}

int main() {
    Display *display;
    Window window;
    int screen;
    GC gc;
    XEvent event;
    
    int x1, y1, x2, y2;

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

    printf("Enter starting points x1, y1: ");
    scanf("%d %d", &x1, &y1);
    printf("Enter ending points x2, y2: ");
    scanf("%d %d", &x2, &y2);

    while (1) {
        XNextEvent(display, &event);

        if (event.type == Expose) {
            drawLine(display, window, gc, x1, y1, x2, y2);
        }

        if (event.type == KeyPress)
            break;
    }

    XCloseDisplay(display);

    return 0;
}
