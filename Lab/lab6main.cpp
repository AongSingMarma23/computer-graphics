#include <GL/glut.h>
#include <stdio.h>

int xc, yc, r;

void drawPixel(int x, int y)
{
    glVertex2i(x, y);
}

void drawCirclePoints(int xc, int yc, int x, int y)
{
    drawPixel(xc + x, yc + y);
    drawPixel(xc - x, yc + y);
    drawPixel(xc + x, yc - y);
    drawPixel(xc - x, yc - y);
    drawPixel(xc + y, yc + x);
    drawPixel(xc - y, yc + x);
    drawPixel(xc + y, yc - x);
    drawPixel(xc - y, yc - x);
}

void drawMidPointCircle()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);   // Red color

    int x = 0;
    int y = r;
    int p = 1 - r;

    glBegin(GL_POINTS);

    drawCirclePoints(xc, yc, x, y);

    while (x < y)
    {
        x = x + 1;

        if (p < 0)
        {
            p = p + 2 * x + 1;
        }
        else
        {
            y = y - 1;
            p = p + 2 * x - 2 * y + 1;
        }

        drawCirclePoints(xc, yc, x, y);
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
    printf("Enter center of circle (xc yc): ");
    scanf("%d %d", &xc, &yc);

    printf("Enter radius of circle: ");
    scanf("%d", &r);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Mid-Point Circle Drawing Algorithm");

    init();
    glutDisplayFunc(drawMidPointCircle);
    glutMainLoop();

    return 0;
}
