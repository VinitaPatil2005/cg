#include <GL/glut.h>
#include <cmath>
#include <unistd.h>

float ballY = 300;     // Y-position
float velocity = 0;    // Velocity
float gravity = -0.5;  // Gravity
float bounceFactor = 0.7;
bool goingDown = true;
float stretch = 1.0;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();

    // Apply squash and stretch
    float scaleY = goingDown ? 1.0 + fabs(velocity) * 0.01 : 1.0 - fabs(velocity) * 0.01;
    float scaleX = 1.0 / scaleY;
    glTranslatef(400, ballY, 0);     // Move to current position
    glScalef(scaleX, scaleY, 1);     // Stretch/squash
    glTranslatef(-400, -ballY, 0);   // Undo translate for scaling center

    // Draw the ball (circle)
    glColor3f(1.0, 0.3, 0.2);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.14159f / 180;
        glVertex2f(400 + 30 * cos(theta), ballY + 30 * sin(theta));
    }
    glEnd();

    glPopMatrix();

    glFlush();
}

void timer(int value) {
    // Physics and squash/stretch logic
    velocity += gravity;
    ballY += velocity;

    // Hit the ground
    if (ballY <= 30) {
        ballY = 30;
        velocity = -velocity * bounceFactor;

        // Anticipation - small delay before bounce
        usleep(50000); // 50 ms pause
    }

    // Slow in and out effect
    if (velocity < 0)
        goingDown = true;
    else
        goingDown = false;

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

void init() {
    glClearColor(0, 0, 0, 1); // Black background
    gluOrtho2D(0, 800, 0, 600); // Set 2D viewing area
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("Bouncing Ball Animation - Principles Demo");

    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}
