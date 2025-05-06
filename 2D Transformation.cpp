#include <GL/glut.h>
#include <vector>

// Original polygon points
std::vector<std::pair<float, float>> polygon = {
    {100, 100}, {150, 200}, {200, 100}
};

// Transformation parameters
float theta = 45.0f;
float scaleX = 1.5f, scaleY = 1.5f;
float refX = 150.0f, refY = 150.0f;

void drawPolygon() {
    glBegin(GL_LINE_LOOP);
    for (auto &p : polygon)
        glVertex2f(p.first, p.second);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 1. Original polygon (Yellow)
    glColor3f(1, 1, 0);
    drawPolygon();

    // 2. Scaled polygon (Green)
    glPushMatrix();
        glTranslatef(250, 0, 0); // Move to right
        glTranslatef(refX, refY, 0);
        glScalef(scaleX, scaleY, 1);
        glTranslatef(-refX, -refY, 0);
        glColor3f(0, 1, 0);
        drawPolygon();
    glPopMatrix();

    // 3. Rotated polygon (Red)
    glPushMatrix();
        glTranslatef(0, 250, 0); // Move upward
        glTranslatef(refX, refY, 0);
        glRotatef(theta, 0, 0, 1);
        glTranslatef(-refX, -refY, 0);
        glColor3f(1, 0, 0);
        drawPolygon();
    glPopMatrix();

    // 4. Reflected polygon (Cyan)
    glPushMatrix();
        glTranslatef(250, 250, 0); // Move up-right
        glScalef(-1, 1, 1);        // Reflect across Y-axis
        glTranslatef(-300, 0, 0);  // Adjust position
        glColor3f(0, 1, 1);
        drawPolygon();
    glPopMatrix();

    glFlush();
}

void init() {
    glClearColor(0, 0, 0, 1);
    gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("2D Transformations");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

