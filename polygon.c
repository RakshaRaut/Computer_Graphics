#include <GL/glut.h>

void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 800.0, 0.0, 600.0); // Set the coordinate system
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 0.0, 0.0); // Red color
    glBegin(GL_QUADS);
        glVertex2i(100, 250); // Bottom-left corner
        glVertex2i(300, 250); // Bottom-right corner
        glVertex2i(300, 450); // Top-right corner
        glVertex2i(100, 450); // Top-left corner
    glEnd();

    glColor3f(0.0, 1.0, 0.0); // Green color
    glBegin(GL_POLYGON);
        glVertex2i(500, 300); // First vertex
        glVertex2i(550, 400); // Second vertex
        glVertex2i(600, 350); // Third vertex
        glVertex2i(650, 400); // Fourth vertex
        glVertex2i(700, 300); // Fifth vertex
        glVertex2i(600, 250); // Sixth vertex
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600); 
    glutCreateWindow("Separate Rectangle and Polygon");
    init(); 
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
