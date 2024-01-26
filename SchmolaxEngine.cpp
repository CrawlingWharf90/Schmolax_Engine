#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>

/* ========== ========== ========== ========== ========== ========== */

/*

    A demonstration painting program for IM211 and MA8
    Based on Interactive Computer Graphics
    E. Angel
    Chapter 3 and Appendix A.3
        How to set up menus and submenus.
        How to set up colours.
        How to determine and use the position of the mouse.
*/

// Function prototypes
void ColourMenu(int id);
void Display(void);
void DrawSquare(int x, int y);
void FillMenu(int id);
void Mouse(int btn, int state, int x, int y);
void MyInit(void);
int Pick(int x, int y);
void PixelMenu(int id);
void Reshape(int w, int h);
void RightMenu(int id);
void ScreenBox(int x, int y, int size);

// Global variables
int fill = 0;
GLfloat r = 1.0, g = 1.0, b = 1.0;
GLfloat size = 3.0; /* half side of square */
GLsizei wh = 250;   /* initial height of window */
GLsizei ww = 250;

void ColourMenu(int id) {
    /*
    * sets the red, green, and blue components
    * for the appropriate menu selection.
    */
    if (1 == id)
        { r = 1.0; g = 0.0; b = 0.0; }
    else if (2 == id)
        { r = 0.0; g = 1.0; b = 0.0; }
    else if (3 == id)
        { r = 0.0; g = 0.0; b = 1.0; }
    else if (4 == id)
        { r = 0.0; g = 1.0; b = 1.0; }
    else if (5 == id)
        { r = 1.0; g = 0.0; b = 1.0; }
    else if (6 == id)
        { r = 1.0; g = 1.0; b = 0.0; }
    else if (7 == id)
        { r = 1.0; g = 1.0; b = 1.0; }
    else if (8 == id)
        { r = 0.0; g = 0.0; b = 0.0; }
} /* end of function ColourMenu */

void Display(void) {
    /*
    * sets up a cleared display window with
    * the tool bar containing icons.
    */
    glPushAttrib(GL_ALL_ATTRIB_BITS); /* save attributes */

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    /* place tool bar */
    glColor3f(1.0, 1.0, 1.0);
    ScreenBox(0, wh - ww / 10, ww / 10);

    glColor3f(1.0, 0.0, 0.0);
    ScreenBox(ww / 10, wh - ww / 10, ww / 10);

    glColor3f(0.0, 1.0, 0.0);
    ScreenBox(ww / 5, wh - ww / 10, ww / 10);

    glColor3f(0.0, 0.0, 1.0);
    ScreenBox(3 * ww / 10, wh - ww / 10, ww / 10);

    /* place icons */
    glColor3f(0.0, 0.0, 0.0);
    ScreenBox(ww / 10 + ww / 40, wh - ww / 10 + ww / 40, ww / 20);

    glBegin(GL_LINES);
    glVertex2i(wh / 40, wh - ww / 20);
    glVertex2i(wh / 40 + ww / 20, wh - ww / 20);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2i(ww / 5 + ww / 40, wh - ww / 10 + ww / 40);
    glVertex2i(ww / 5 + ww / 20, wh - ww / 40);
    glVertex2i(ww / 5 + 3 * ww / 40, wh - ww / 10 + ww / 40);
    glEnd();

    glPointSize(5.0);
    glBegin(GL_POINTS);
    glVertex2i(3 * ww / 10 + ww / 20, wh - ww / 20);
    glEnd();

    glFlush();

    glPopAttrib(); /* restore attributes */
} /* end of function Display */

void DrawSquare(int x, int y) {
    /*
    * draws a square using a randomly chosen colour.
    */
    y = wh - y;
    glColor3ub((GLubyte)rand() % 256,
        (GLubyte)rand() % 256,
        (GLubyte)rand() % 256);

    glBegin(GL_POLYGON);
    glVertex2f(x + size, y + size);
    glVertex2f(x - size, y + size);
    glVertex2f(x - size, y - size);
    glVertex2f(x + size, y - size);
    glEnd();
    glFlush();
} /* end of function DrawSquare */

void FillMenu(int id) {
    /*
    * sets the global fill flag.
    */
    if (1 == id)
        fill = 1;
    else if (2 == id)
        fill = 0;
} /* end of function FillMenu */

void Mouse(int btn, int state, int x, int y) {
    /*
    * determines the action to be taken based on the
    * position of the mouse.
    *
    * drawMode identifies the tool chosen
    * 1 = line, 2 = square, 3 = triangle, 4 = pixel
    *
    * count marks the number of points already chosen
    * in specifying a square, triangle, etc.
    */
    static int count;
    static int drawMode = 0;
    static int xp[2], yp[2];

    int where;

    if (GLUT_LEFT_BUTTON == btn && GLUT_DOWN == state) {
        glPushAttrib(GL_ALL_ATTRIB_BITS);

        where = Pick(x, y);
        glColor3f(r, g, b);

        if (where != 0) {
            count = 0;
            drawMode = where;
        }
        else if (1 == drawMode && 0 == count) {
            count = 1;
            xp[0] = x;
            yp[0] = y;
        }
        else if (1 == drawMode && 0 != count) {
            glBegin(GL_LINES);
            glVertex2i(x, wh - y);
            glVertex2i(xp[0], wh - yp[0]);
            glEnd();
            count = 0;
            drawMode = 0;
        }
        else if (2 == drawMode && 0 == count) {
            count = 1;
            xp[0] = x;
            yp[0] = y;
        }
        else if (2 == drawMode && 0 != count) {
            if (fill)
                glBegin(GL_POLYGON);
            else
                glBegin(GL_LINE_LOOP);

            glVertex2i(x, wh - y);
            glVertex2i(x, wh - yp[0]);
            glVertex2i(xp[0], wh - yp[0]);
            glVertex2i(xp[0], wh - y);
            glEnd();
            count = 0;
            drawMode = 0;
        }
        else if (3 == drawMode && 0 == count) {
            count = 1;
            xp[0] = x;
            yp[0] = y;
        }
        else if (3 == drawMode && 1 == count) {
            count = 2;
            xp[1] = x;
            yp[1] = y;
        }
        else if (3 == drawMode && 2 == count) {
            if (fill)
                glBegin(GL_POLYGON);
            else
                glBegin(GL_LINE_LOOP);

            glVertex2i(xp[0], wh - yp[0]);
            glVertex2i(xp[1], wh - yp[1]);
            glVertex2i(x, wh - y);
            glEnd();
            count = 0;
            drawMode = 0;
        }
        else if (4 == drawMode) {
            DrawSquare(x, y);
        }

        glFlush();
        glPopAttrib();
    }
} /* end of function Mouse */

void MyInit(void) {
    glViewport(0, 0, ww, wh);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)ww, 0.0, (GLdouble)wh);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glFlush();
} /* end of function MyInit */

int Pick(int x, int y) {
    /*
    * returns token picked by mouse.
    *
    * 0 => no selection
    * 1 - 4 identify the tools from left to right.
    */
    y = wh - y;
    if (y < wh - ww / 10)
        return 0;
    else if (x < ww / 10)
        return 1;
    else if (x < ww / 5)
        return 2;
    else if (x < 3 * ww / 10)
        return 3;
    else if (x < 2 * ww / 5)
        return 4;
    else
        return 0;
} /* end of function Pick */

void PixelMenu(int id) {
    if (1 == id)
        size = size * 2.0;
    else if (2 == id)
        size = size / 2.0;
} /* end of function PixelMenu */

void Reshape(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glViewport(0, 0, w, h);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glFlush();

    wh = h;
    ww = w;
} /* end of function Reshape */

void RightMenu(int id) {
    if (1 == id)
        exit(0);
    else if (2 == id)
        glutPostRedisplay();
} /* end of function RightMenu */

void ScreenBox(int x, int y, int size) {
    glBegin(GL_POLYGON);
    glVertex2i(x, y);
    glVertex2i(x + size, y);
    glVertex2i(x + size, y + size);
    glVertex2i(x, y + size);
    glEnd();
} /* end of function ScreenBox */

int main(int argc, char** argv) {
    int cMenu; // handle for colour menu
    int pMenu;
    int fMenu;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(ww, wh);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Graphic Paint");

    glutDisplayFunc(Display);

    cMenu = glutCreateMenu(ColourMenu);
    glutAddMenuEntry("Red", 1);
    glutAddMenuEntry("Green", 2);
    glutAddMenuEntry("Blue", 3);
    glutAddMenuEntry("Cyan", 4);
    glutAddMenuEntry("Magenta", 5);
    glutAddMenuEntry("Yellow", 6);
    glutAddMenuEntry("White", 7);
    glutAddMenuEntry("Black", 8);

    pMenu = glutCreateMenu(PixelMenu);
    glutAddMenuEntry("Increase size", 1);
    glutAddMenuEntry("Decrease size", 2);

    fMenu = glutCreateMenu(FillMenu);
    glutAddMenuEntry("Fill on", 1);
    glutAddMenuEntry("Fill off", 2);

    glutCreateMenu(RightMenu);
    glutAddMenuEntry("Quit", 1);
    glutAddMenuEntry("Clear", 2);
    glutAddSubMenu("Colours", cMenu);
    glutAddSubMenu("Pixel size", pMenu);
    glutAddSubMenu("Fill", fMenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    MyInit();

    glutReshapeFunc(Reshape);
    glutMouseFunc(Mouse);

    glutMainLoop();

    return 0;
}
