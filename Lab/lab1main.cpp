//Home
/*#include <GL/glut.h>

void drawHome()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // House Base
    glColor3f(0.6, 0.3, 0.0);
    glBegin(GL_POLYGON);
    glVertex2f(100, 100);
    glVertex2f(300, 100);
    glVertex2f(300, 250);
    glVertex2f(100, 250);
    glEnd();

    // Roof
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2f(90, 250);
    glVertex2f(310, 250);
    glVertex2f(200, 350);
    glEnd();

    // Door
    glColor3f(0.3, 0.2, 0.1);
    glBegin(GL_POLYGON);
    glVertex2f(170, 100);
    glVertex2f(230, 100);
    glVertex2f(230, 180);
    glVertex2f(170, 180);
    glEnd();

    // Left Window
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
    glVertex2f(120, 170);
    glVertex2f(160, 170);
    glVertex2f(160, 210);
    glVertex2f(120, 210);
    glEnd();

    // Right Window
    glBegin(GL_POLYGON);
    glVertex2f(240, 170);
    glVertex2f(280, 170);
    glVertex2f(280, 210);
    glVertex2f(240, 210);
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
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Draw a Home - OpenGL");
    init();
    glutDisplayFunc(drawHome);
    glutMainLoop();
    return 0;
}*/

//Stars
#include <GL/glut.h>

void drawStar(float x, float y)
{
    glBegin(GL_POLYGON);
    glVertex2f(x,     y + 30);
    glVertex2f(x + 10, y + 10);
    glVertex2f(x + 30, y + 10);
    glVertex2f(x + 15, y - 5);
    glVertex2f(x + 20, y - 30);
    glVertex2f(x,     y - 15);
    glVertex2f(x - 20, y - 30);
    glVertex2f(x - 15, y - 5);
    glVertex2f(x - 30, y + 10);
    glVertex2f(x - 10, y + 10);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Star 1 - Red
    glColor3f(1.0, 0.0, 0.0);
    drawStar(100, 300);

    // Star 2 - Green
    glColor3f(0.0, 1.0, 0.0);
    drawStar(300, 300);

    // Star 3 - Blue
    glColor3f(0.0, 0.0, 1.0);
    drawStar(100, 100);

    // Star 4 - Yellow
    glColor3f(1.0, 1.0, 0.0);
    drawStar(300, 100);

    glFlush();
}

void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    gluOrtho2D(0, 400, 0, 400);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Draw 4 Stars - OpenGL");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}


