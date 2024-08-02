#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
   
    Display *display;
    Window window;
    int screen;
    GC gc;
    XEvent event;

   
    int x1, y1, x2, y2;
    float x, y, dx, dy, xinc, yinc;
    int steps;

   
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

    
    printf("Enter x1, y1: ");
    scanf("%d %d", &x1, &y1);
    printf("Enter x2, y2: ");
    scanf("%d %d", &x2, &y2);

   
    dx = x2 - x1;
    dy = y2 - y1;

    
    steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

   
    xinc = dx / (float) steps;
    yinc = dy / (float) steps;

  
    x = x1;
    y = y1;

  
    while (1) {
        XNextEvent(display, &event);

       
        if (event.type == Expose) {
         
            XDrawPoint(display, window, gc, round(x), round(y));

           
            for (int i = 0; i < steps; i++) {
                x += xinc;
                y += yinc;
                XDrawPoint(display, window, gc, round(x), round(y));
            }
        }

       
        if (event.type == KeyPress)
            break;
    }

   
    XCloseDisplay(display);

    return 0;
}
