#include <GL/glut.h>
#include <stdio.h>
int xc = 0, yc = 0, r;
void plotPoints(int x, int y)
{
    glBegin(GL_POINTS);
        glVertex2i(xc + x, yc + y);
        glVertex2i(xc - x, yc + y);
        glVertex2i(xc + x, yc - y);
        glVertex2i(xc - x, yc - y);
        glVertex2i(xc + y, yc + x);
        glVertex2i(xc - y, yc + x);
        glVertex2i(xc + y, yc - x);
        glVertex2i(xc - y, yc - x);
    glEnd();
}
void drawCircle()
{
    int x = 0;
    int y = r;
    int p = 1 - r;
    glClear(GL_COLOR_BUFFER_BIT);
    while (x <= y)
    {
        plotPoints(x, y);
        x++;
        if (p < 0)
            p = p + 2 * x + 1;
        else
        {
            y--;
            p = p + 2 * x + 1 - 2 * y;
        }
    }
    glFlush();
}
void init()
{
    glClearColor(0, 0, 0, 1);
    glColor3f(1, 1, 1);
    glPointSize(2.0);
    gluOrtho2D(-200, 200, -200, 200);
}
int main(int argc, char** argv)
{
    printf("Enter radius of the circle (1 to 100): ");
    scanf("%d", &r);
    if (r < 1 || r > 100)
    {
        printf("Invalid radius! Please enter a value between 1 and 100.\n");
        return 0;
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Mid Point Circle Algorithm");
    init();
    glutDisplayFunc(drawCircle);
    glutMainLoop();
    return 0;
}

