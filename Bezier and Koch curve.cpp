// fractal_curves.cpp
#include <GL/glut.h> // Replace this header file with #include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>

int mode = 1; // 1 = Bezier, 2 = Koch

// -------------------- Bezier Curve --------------------
std::vector<std::pair<float, float>> controlPoints = {
    {100, 100}, {150, 300}, {300, 300}, {350, 100}
};

std::pair<float, float> interpolate(std::pair<float, float> a, std::pair<float, float> b, float t) {
    return {
        (1 - t) * a.first + t * b.first,
        (1 - t) * a.second + t * b.second
    };
}

std::pair<float, float> deCasteljau(std::vector<std::pair<float, float>> points, float t) {
    while (points.size() > 1) {
        std::vector<std::pair<float, float>> next;
        for (size_t i = 0; i < points.size() - 1; ++i)
            next.push_back(interpolate(points[i], points[i + 1], t));
        points = next;
    }
    return points[0];
}

void drawBezierCurve() {
    glColor3f(1, 0, 0);
    glBegin(GL_POINTS);
    for (float t = 0; t <= 1.0; t += 0.001) {
        auto p = deCasteljau(controlPoints, t);
        glVertex2f(p.first, p.second);
    }
    glEnd();
}

// -------------------- Koch Curve --------------------
void drawKoch(float x1, float y1, float x2, float y2, int depth) {
    if (depth == 0) {
        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
        return;
    }

    float dx = (x2 - x1) / 3.0;
    float dy = (y2 - y1) / 3.0;

    float x3 = x1 + dx;
    float y3 = y1 + dy;

    float x5 = x2 - dx;
    float y5 = y2 - dy;

    float x4 = x3 + (cos(M_PI / 3) * dx - sin(M_PI / 3) * dy);
    float y4 = y3 + (sin(M_PI / 3) * dx + cos(M_PI / 3) * dy);

    drawKoch(x1, y1, x3, y3, depth - 1);
    drawKoch(x3, y3, x4, y4, depth - 1);
    drawKoch(x4, y4, x5, y5, depth - 1);
    drawKoch(x5, y5, x2, y2, depth - 1);
}

// -------------------- OpenGL Display --------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (mode == 1) {
        drawBezierCurve();
    } else if (mode == 2) {
        glColor3f(0, 1, 1);
        drawKoch(100, 250, 400, 250, 4);
    }
    glFlush();
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
        case '1':
            mode = 1;
            break;
        case '2':
            mode = 2;
            break;
        case 27: // ESC
            exit(0);
    }
    glutPostRedisplay();
}

void init() {
    glClearColor(0, 0, 0, 1);
    gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char** argv) {
    std::cout << "=== Fractal Viewer ===\n"
          << "Press '1' : Show Bezier Curve\n"
          << "Press '2' : Show Koch Snowflake Curve\n"
          << "Press 'Esc' : Exit the program\n";
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Bezier and Koch Fractals");
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
