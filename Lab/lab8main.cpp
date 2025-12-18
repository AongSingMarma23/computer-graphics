#include <GL/glut.h>
#include <math.h>

// ------------------------------------
// Function to draw a PERFECT SOLID circle
// ------------------------------------
void drawSolidCircle(float xc, float yc, float r)
{
    glBegin(GL_TRIANGLE_FAN);

    // Center of the circle
    glVertex2f(xc, yc);

    // Draw circle using smooth triangles
    for (int i = 0; i <= 360; i++)
    {
        float angle = i * 3.1415926f / 180.0f;
        float x = xc + r * cos(angle);
        float y = yc + r * sin(angle);
        glVertex2f(x, y);
    }

    glEnd();
}

// ------------------------------------
// Display Function
// ------------------------------------
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // ----------------------------
    // Draw GREEN RECTANGLE (Flag)
    // ----------------------------
    glColor3f(0.0f, 0.5f, 0.0f);   // Green
    glBegin(GL_POLYGON);
        glVertex2f(50.0f, 150.0f);
        glVertex2f(450.0f, 150.0f);
        glVertex2f(450.0f, 350.0f);
        glVertex2f(50.0f, 350.0f);
    glEnd();

    // ----------------------------
    // Draw PERFECT SOLID RED CIRCLE
    // ----------------------------
    glColor3f(1.0f, 0.0f, 0.0f);   // Red
    drawSolidCircle(230.0f, 250.0f, 60.0f);

    glFlush();
}

// ------------------------------------
// Initialization
// ------------------------------------
void init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   // White background
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
}

// ------------------------------------
// Main Function
// ------------------------------------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Flag of Bangladesh - Solid Red Circle (Final)");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
