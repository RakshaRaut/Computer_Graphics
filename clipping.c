#include <GL/glut.h>
#include <math.h>

#define xmin 50
#define ymin 50
#define xmax 400
#define ymax 400

typedef struct
{
    double x;
    double y;
} Point;

Point line[2] = {{10, 10}, {500, 500}};

void drawLine(Point p1, Point p2, int transformed)
{
    if (transformed)
        glColor3f(1, 1, 0);
    else
        glColor3f(0, 0, 1);

    glBegin(GL_LINES);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glEnd();

    glFlush();
}

int clipTest(double p, double q, double *u1, double *u2)
{
    double r;
    int returnValue = 1;

    if (p < 0.0)
    {
        r = q / p;
        if (r > *u2)
            returnValue = 0;
        else if (r > *u1)
            *u1 = r;
    }
    else if (p > 0.0)
    {
        r = q / p;
        if (r < *u1)
            returnValue = 0;
        else if (r < *u2)
            *u2 = r;
    }
    else if (q < 0.0)
        returnValue = 0;

    return returnValue;
}

void liangBarskyClip(Point p1, Point p2)
{
    drawLine(p1, p2, 0);

    double u1 = 0.0, u2 = 1.0;
    double dx = p2.x - p1.x, dy = p2.y - p1.y;

    if (clipTest(-dx, p1.x - xmin, &u1, &u2))
        if (clipTest(dx, xmax - p1.x, &u1, &u2))
            if (clipTest(-dy, p1.y - ymin, &u1, &u2))
                if (clipTest(dy, ymax - p1.y, &u1, &u2))
                {
                    if (u2 < 1.0)
                    {
                        p2.x = p1.x + u2 * dx;
                        p2.y = p1.y + u2 * dy;
                    }
                    if (u1 > 0.0)
                    {
                        p1.x += u1 * dx;
                        p1.y += u1 * dy;
                    }
                    drawLine(p1, p2, 1);
                }
}

void drawAxes()
{
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex2f(-500, 0);
    glVertex2f(500, 0);
    glVertex2f(0, -500);
    glVertex2f(0, 500);
    glEnd();
    glFlush();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();
    liangBarskyClip(line[0], line[1]);
}

void init()
{
    glClearColor(0, 0, 0, 1);
    gluOrtho2D(-500, 500, -500, 500);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("Liang-Barsky Line Clipping");
    init();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}