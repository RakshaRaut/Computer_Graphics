#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>

#define WIDTH 800
#define HEIGHT 600

void drawLaptop(Display *display, Window window, GC gc) {
    // Draw laptop body (rectangular shape)
    XSetForeground(display, gc, 0x888888); // Gray color
    XFillRectangle(display, window, gc, 100, 100, 600, 400);

    // Draw screen (part of the laptop body)
    XSetForeground(display, gc, 0xCCCCCC); // Light gray color
    XFillRectangle(display, window, gc, 120, 120, 560, 320);

    // Draw keyboard (bottom part of the laptop body)
    XSetForeground(display, gc, 0x333333); // Dark gray color
    XFillRectangle(display, window, gc, 100, 500, 600, 50);
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
    window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, WIDTH, HEIGHT, 1,
                                 BlackPixel(display, screen), WhitePixel(display, screen));

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    gc = XCreateGC(display, window, 0, NULL);

    // Main event loop
    while (1) {
        XNextEvent(display, &event);

        if (event.type == Expose) {
            drawLaptop(display, window, gc);
        }

        if (event.type == KeyPress)
            break;
    }

    // Close the X11 display
    XCloseDisplay(display);

    return 0;
}
