#include <GL/glut.h>
#include <math.h>
#include<stdio.h>

float xs, ys, xe, ye;

void drawLineDDA()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0);   // Red color

    float dx = xe - xs;
    float dy = ye - ys;

    float steps;

    if (fabs(dx) > fabs(dy))
        steps = fabs(dx);
    else
        steps = fabs(dy);

    float xInc = dx / steps;
    float yInc = dy / steps;

    float x = xs;
    float y = ys;

    glBegin(GL_POINTS);
    for (int i = 0; i <= (int)steps; i++)
    {
        glVertex2f(round(x), round(y));
        x = x + xInc;
        y = y + yInc;
    }
    glEnd();

    glFlush();
}

void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    gluOrtho2D(0, 400, 0, 400);
}

int main(int argc, char** argv)
{
    printf("Enter value of X1 :");
    scanf("%f",&xs);
    printf("Enter value of y1 :");
    scanf("%f",&ys);
    printf("Enter value of X2 :");
    scanf("%f",&xe);
    printf("Enter value of Y2 :");
    scanf("%f",&ye);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("DDA Line Drawing Algorithm");

    init();
    glutDisplayFunc(drawLineDDA);
    glutMainLoop();

    return 0;
}
