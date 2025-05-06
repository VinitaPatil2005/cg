#include <GL/glut.h> // Replace this header file with #include <GL/glut.h>
#include <cmath>
#include <vector>
#include <iostream>
enum LineAlgorithm { DDA, BRESENHAM };
enum LineStyle { SIMPLE, DOTTED, DASHED, SOLID };

LineAlgorithm currentAlgorithm = DDA;
LineStyle currentStyle = SIMPLE;

struct Point {
    int x, y;
};

std::vector<Point> points;

Point screenToGL(int x, int y) {
    return {x - 250, 250 - y};  
}

void setPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void drawLineDDA(Point p1, Point p2) {
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;

    int steps = std::max(abs(dx), abs(dy));
    float xInc = dx / (float)steps;
    float yInc = dy / (float)steps;

    float x = p1.x;
    float y = p1.y;

    for (int i = 0; i <= steps; ++i) {
        if (currentStyle == DOTTED && i % 4 != 0) continue;
        if (currentStyle == DASHED && (i / 5) % 2 == 1) continue;
        setPixel(round(x), round(y));
        x += xInc;
        y += yInc;
    }
}

void drawLineBresenham(Point p1, Point p2) {
    int x = p1.x, y = p1.y;
    int dx = abs(p2.x - p1.x);
    int dy = abs(p2.y - p1.y);
    int sx = p1.x < p2.x ? 1 : -1;
    int sy = p1.y < p2.y ? 1 : -1;
    bool isSteep = dy > dx;

    int err = (isSteep ? dy : dx) / 2;

    for (int i = 0; ; ++i) {
        if (currentStyle == DOTTED && i % 4 != 0) {
            // skip
        } else if (currentStyle == DASHED && (i / 5) % 2 == 1) {
            // skip
        } else {
            setPixel(x, y);
        }

        if (x == p2.x && y == p2.y) break;

        if (isSteep) {
            y += sy;
            err -= dx;
        } else {
            x += sx;
            err -= dy;
        }

        if (err < 0) {
            if (isSteep)
                x += sx;
            else
                y += sy;
            err += isSteep ? dy : dx;
        }
    }
}

void drawAxes() {
    glColor3f(0.7, 0.7, 0.7);
    for (int i = -250; i <= 250; ++i) {
        setPixel(i, 0); 
        setPixel(0, i); 
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();
    glColor3f(1.0, 1.0, 1.0);

    if (points.size() == 2) {
        if (currentAlgorithm == DDA)
            drawLineDDA(points[0], points[1]);
        else
            drawLineBresenham(points[0], points[1]);
    }

    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        Point p = screenToGL(x, y);
        if (points.size() == 2) points.clear();
        points.push_back(p);
    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        Point p = screenToGL(x, y);
        if (points.size() == 1) points.push_back(p);
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
        case '1': currentAlgorithm = DDA; break;
        case '2': currentAlgorithm = BRESENHAM; break;
        case 'a': currentStyle = SIMPLE; break;
        case 's': currentStyle = DOTTED; break;
        case 'd': currentStyle = DASHED; break;
        case 'f': currentStyle = SOLID; break;
    }
    glutPostRedisplay();
}

void init() {
    glClearColor(0, 0, 0, 1);
    gluOrtho2D(-250, 250, -250, 250);  
    glPointSize(2.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Line Drawing with DDA and Bresenham");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
