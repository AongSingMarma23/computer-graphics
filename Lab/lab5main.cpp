#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

int xs, ys, xe, ye;

void drawLineBresenham()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 1.0);   // Blue color

    int dx = abs(xe - xs);
    int dy = abs(ye - ys);

    int p = 2 * dy - dx;
    int x = xs;
    int y = ys;

    glBegin(GL_POINTS);
    while (x <= xe)
    {
        glVertex2i(x, y);

        if (p < 0)
        {
            p = p + 2 * dy;
        }
        else
        {
            p = p + 2 * (dy - dx);
            y = y + 1;
        }
        x = x + 1;
    }
    glEnd();

    glFlush();
}

void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);  // White background
    gluOrtho2D(0, 400, 0, 400);
}

int main(int argc, char** argv)
{
    // Take input from user BEFORE creating window
    printf("Enter starting point (xs ys): ");
    scanf("%d %d", &xs, &ys);

    printf("Enter ending point (xe ye): ");
    scanf("%d %d", &xe, &ye);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Bresenham Line Drawing Algorithm with scanf");

    init();
    glutDisplayFunc(drawLineBresenham);
    glutMainLoop();

    return 0;
}
