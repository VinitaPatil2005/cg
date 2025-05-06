#include <GL/glut.h>
#include <vector>
#include <stack>
#include <iostream>

using namespace std;

vector<pair<int, int>> polygonPoints;
int fillMethod = 0; // 1: Flood Fill, 2: Boundary Fill
bool polygonFinalized = false;
pair<int, int> seedPoint = {-1, -1};

void setPixel(int x, int y, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
    glFlush();
}

void getPixelColor(int x, int y, float* color) {
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, color);
}

void floodFill(int x, int y, float* targetColor, float* fillColor) {
    float current[3];
    getPixelColor(x, y, current);
    if (abs(current[0] - targetColor[0]) > 0.01 ||
        abs(current[1] - targetColor[1]) > 0.01 ||
        abs(current[2] - targetColor[2]) > 0.01) {
        return;
    }

    setPixel(x, y, fillColor[0], fillColor[1], fillColor[2]);

    floodFill(x + 1, y, targetColor, fillColor);
    floodFill(x - 1, y, targetColor, fillColor);
    floodFill(x, y + 1, targetColor, fillColor);
    floodFill(x, y - 1, targetColor, fillColor);
}

void boundaryFill(int x, int y, float* boundaryColor, float* fillColor) {
    float current[3];
    getPixelColor(x, y, current);

    if ((abs(current[0] - boundaryColor[0]) > 0.01 ||
         abs(current[1] - boundaryColor[1]) > 0.01 ||
         abs(current[2] - boundaryColor[2]) > 0.01) &&
        (abs(current[0] - fillColor[0]) > 0.01 ||
         abs(current[1] - fillColor[1]) > 0.01 ||
         abs(current[2] - fillColor[2]) > 0.01)) {

        setPixel(x, y, fillColor[0], fillColor[1], fillColor[2]);

        boundaryFill(x + 1, y, boundaryColor, fillColor);
        boundaryFill(x - 1, y, boundaryColor, fillColor);
        boundaryFill(x, y + 1, boundaryColor, fillColor);
        boundaryFill(x, y - 1, boundaryColor, fillColor);
    }
}

void drawPolygon() {
    glColor3f(1, 1, 1); // white polygon
    glBegin(GL_LINE_LOOP);
    for (auto& pt : polygonPoints)
        glVertex2i(pt.first, pt.second);
    glEnd();
    glFlush();
}

void mouse(int button, int state, int x, int y) {
    y = 500 - y;

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (!polygonFinalized) {
            polygonPoints.push_back({x, y});
            setPixel(x, y, 1, 1, 1);
        } else {
            seedPoint = {x, y};
            cout << "Seed point set at: (" << x << ", " << y << ")\n";
        }
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 13) { // ENTER
        if (polygonPoints.size() >= 3) {
            polygonFinalized = true;
            drawPolygon();
            cout << "Polygon finalized.\n";
        }
    } else if (key == 'f') {
        if (seedPoint.first != -1 && polygonFinalized) {
            float targetColor[3];
            float fillColor[] = {1, 0, 0}; // Red fill

            getPixelColor(seedPoint.first, seedPoint.second, targetColor);

            if (fillMethod == 1) {
                floodFill(seedPoint.first, seedPoint.second, targetColor, fillColor);
                cout << "Flood Fill applied.\n";
            } else if (fillMethod == 2) {
                float boundaryColor[] = {1, 1, 1}; // White boundary
                boundaryFill(seedPoint.first, seedPoint.second, boundaryColor, fillColor);
                cout << "Boundary Fill applied.\n";
            }
        }
    }
}

void menu(int option) {
    fillMethod = option;
    if (fillMethod == 1)
        cout << "Flood Fill selected.\n";
    else if (fillMethod == 2)
        cout << "Boundary Fill selected.\n";
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

void init() {
    glClearColor(0, 0, 0, 0); // Black background
    gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Flood Fill and Boundary Fill");

    init();

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glutCreateMenu(menu);
    glutAddMenuEntry("Flood Fill", 1);
    glutAddMenuEntry("Boundary Fill", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}

