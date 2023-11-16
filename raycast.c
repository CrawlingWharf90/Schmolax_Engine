#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define WIDTH 1024
#define HEIGHT 512

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSwapBuffers(); 
}

void init()
{
    glClearColor(0.3, 0.3, 0.3, 0.3);
    gluOrtho2D(0, 1024, 512, 0);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Schmolax Raycast");
    init();
    glutDisplayFunc(display);
    glutMainLoop(); 
}