#include <GL/glut.h> // Replace this header file with #include <GL/glut.h>
#include <vector>
#include <iostream>

#define INSIDE 0 // 0000
#define LEFT 1   // 0001
#define RIGHT 2  // 0010
#define BOTTOM 4 // 0100
#define TOP 8    // 1000

// Window bounds
int winXmin = 100, winYmin = 100, winXmax = 300, winYmax = 300;
// Viewport bounds
int vpXmin = 350, vpYmin = 100, vpXmax = 500, vpYmax = 300;

std::vector<std::pair<int, int>> polygon;
bool isClipped = false;

// Compute region code for a point (x, y)
int computeCode(int x, int y) {
    int code = INSIDE;

    if (x < winXmin) code |= LEFT;
    else if (x > winXmax) code |= RIGHT;

    if (y < winYmin) code |= BOTTOM;
    else if (y > winYmax) code |= TOP;

    return code;
}

// Cohen-Sutherland Line Clipping Algorithm
bool cohenSutherlandClip(int &x1, int &y1, int &x2, int &y2) {
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);
    bool accept = false;

    while (true) {
        if ((code1 | code2) == 0) {
            accept = true;
            break;
        }
        else if (code1 & code2) {
            break;
        }
        else {
            int codeOut = code1 ? code1 : code2;
            int x, y;

            if (codeOut & TOP) {
                x = x1 + (x2 - x1) * (winYmax - y1) / (y2 - y1);
                y = winYmax;
            }
            else if (codeOut & BOTTOM) {
                x = x1 + (x2 - x1) * (winYmin - y1) / (y2 - y1);
                y = winYmin;
            }
            else if (codeOut & RIGHT) {
                y = y1 + (y2 - y1) * (winXmax - x1) / (x2 - x1);
                x = winXmax;
            }
            else {
                y = y1 + (y2 - y1) * (winXmin - x1) / (x2 - x1);
                x = winXmin;
            }

            if (codeOut == code1) {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            }
            else {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }
    return accept;
}

// Map to viewport
void mapToViewport(int x, int y, int &vx, int &vy) {
    float sx = (float)(vpXmax - vpXmin) / (winXmax - winXmin);
    float sy = (float)(vpYmax - vpYmin) / (winYmax - winYmin);

    vx = vpXmin + (x - winXmin) * sx;
    vy = vpYmin + (y - winYmin) * sy;
}

// Draw window rectangle
void drawWindow() {
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(winXmin, winYmin);
    glVertex2i(winXmax, winYmin);
    glVertex2i(winXmax, winYmax);
    glVertex2i(winXmin, winYmax);
    glEnd();
}

// Draw viewport rectangle
void drawViewport() {
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(vpXmin, vpYmin);
    glVertex2i(vpXmax, vpYmin);
    glVertex2i(vpXmax, vpYmax);
    glVertex2i(vpXmin, vpYmax);
    glEnd();
}

// Draw original polygon
void drawOriginalPolygon() {
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_LOOP);
    for (auto &p : polygon)
        glVertex2i(p.first, p.second);
    glEnd();
}

// Draw clipped polygon
void drawClippedPolygon() {
    glColor3f(0.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
    for (size_t i = 0; i < polygon.size(); ++i) {
        int x1 = polygon[i].first;
        int y1 = polygon[i].second;
        int x2 = polygon[(i + 1) % polygon.size()].first;
        int y2 = polygon[(i + 1) % polygon.size()].second;

        int cx1 = x1, cy1 = y1, cx2 = x2, cy2 = y2;
        if (cohenSutherlandClip(cx1, cy1, cx2, cy2)) {
            int vx1, vy1, vx2, vy2;
            mapToViewport(cx1, cy1, vx1, vy1);
            mapToViewport(cx2, cy2, vx2, vy2);
            glVertex2i(vx1, vy1);
            glVertex2i(vx2, vy2);
        }
    }
    glEnd();
}

// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawWindow();
    drawViewport();
    if (!polygon.empty())
        drawOriginalPolygon();
    if (isClipped)
        drawClippedPolygon();
    glFlush();
}

// Mouse click to add polygon points
void mouse(int btn, int state, int x, int y) {
    y = 500 - y;
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN && !isClipped) {
        polygon.emplace_back(x, y);
        glutPostRedisplay();
    }
}

// Menu options
void menu(int option) {
    switch (option) {
        case 1:
            isClipped = true;
            glutPostRedisplay();
            break;
        case 2:
            polygon.clear();
            isClipped = false;
            glutPostRedisplay();
            break;
        case 3:
            exit(0);
    }
}

// OpenGL init
void init() {
    glClearColor(0, 0, 0, 1);
    gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Cohen-Sutherland Polygon Clipping");
    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    glutCreateMenu(menu);
    glutAddMenuEntry("Clip Polygon", 1);
    glutAddMenuEntry("Clear", 2);
    glutAddMenuEntry("Exit", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
