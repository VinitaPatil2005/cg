#include <GL/glut.h>// Replace this header file with #include <GL/glut.h>
#include <iostream>
#include <cmath>

int radius = 50;            // Radius of the circle
int centerX = 0, centerY = 0; // Center of circle (logical center)

// Function to draw a pixel at (x, y) and its reflections in all 4 quadrants
void drawSymmetricPoints(int xc, int yc, int x, int y) {
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc - x, yc - y);
    glVertex2i(xc + y, yc + x);
    glVertex2i(xc - y, yc + x);
    glVertex2i(xc + y, yc - x);
    glVertex2i(xc - y, yc - x);
}

// Bresenham Circle Drawing Algorithm
void drawCircle(int xc, int yc, int r) {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    glBegin(GL_POINTS);
    while (x <= y) {
        drawSymmetricPoints(xc, yc, x, y);
        if (d < 0)
            d += 4 * x + 6;
        else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
    glEnd();
}

// Draw axes
void drawAxes() {
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINES);
    glVertex2i(-250, 0);
    glVertex2i(250, 0);
    glVertex2i(0, -250);
    glVertex2i(0, 250);
    glEnd();
}

// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();

    glColor3f(1.0, 0.0, 0.0);

    // Draw circle in all four quadrants (mirrored)
    drawCircle(centerX, centerY, radius);     // Original
    drawCircle(-centerX, centerY, radius);    // Reflect X
    drawCircle(centerX, -centerY, radius);    // Reflect Y
    drawCircle(-centerX, -centerY, radius);   // Reflect both

    glFlush();
}

// Initialize OpenGL
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Black background
    glColor3f(1.0, 0.0, 0.0);         // Drawing color
    glPointSize(2.0);                 // Point size
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-250, 250, -250, 250);
}

int main(int argc, char** argv) {
    std::cout << "Enter radius: ";
    std::cin >> radius;
    std::cout << "Enter center (x y): ";
    std::cin >> centerX >> centerY;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham Circle in All Quadrants");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
