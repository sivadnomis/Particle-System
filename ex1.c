////////////////////////////////////////////////////////////////
// School of Computer Science
// The University of Manchester
//
// This code is licensed under the terms of the Creative Commons 
// Attribution 2.0 Generic (CC BY 3.0) License.
//
// Skeleton code for COMP37111 coursework, 2013-14
//
// Authors: Arturs Bekasovs and Toby Howard
//
/////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#ifdef MACOSX
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

// Display list for coordinate axis 
GLuint axisList;

int AXIS_SIZE= 200;
int axisEnabled= 1;

typedef struct { 
  char*    name;       /* name */
  GLfloat position        /* colour */
 }point;

point testPoint;

///////////////////////////////////////////////

double myRandom()
//Return random double within range [0,1]
{
  return (rand()/(double)RAND_MAX);
}

///////////////////////////////////////////////

void tickPoint(void)
{
  if (testPoint.position < 500)
  {
    testPoint.position += 20;
    printf("added 20\n");
  }
  else
  {
    testPoint.position = 0;
    printf("zeroed\n");
  }

  glutPostRedisplay();
}

void initialisePoint()
{
  testPoint.name = "bob";
  testPoint.position = 20;
}

void drawPoint()
{
  //printf("lol\n");
  //tickPoint();
  printf("%f\n", testPoint.position);

  glBegin (GL_POINTS);
      glVertex3f (testPoint.position, 0.0, 0.0);
  glEnd ();  
}

///////////////////////////////////////////////

void display()
{
  glLoadIdentity();
  gluLookAt(0.0, 100.0, 1000.0,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT);
  // If enabled, draw coordinate axis
  if(axisEnabled) glCallList(axisList);

  
  drawPoint();

  glutSwapBuffers();
}

///////////////////////////////////////////////

void keyboard(unsigned char key, int x, int y)
{
  if(key == 27) exit(0);
  glutPostRedisplay();
}

///////////////////////////////////////////////

void reshape(int width, int height)
{
  glClearColor(0.9, 0.9, 0.9, 1.0);
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 10000.0);
  glMatrixMode(GL_MODELVIEW);
}

///////////////////////////////////////////////

void makeAxes() {
// Create a display list for drawing coord axis
  axisList = glGenLists(1);
  glNewList(axisList, GL_COMPILE);
      glLineWidth(2.0);
      glBegin(GL_LINES);
      glColor3f(1.0, 0.0, 0.0);       // X axis - red
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(AXIS_SIZE, 0.0, 0.0);
      glColor3f(0.0, 1.0, 0.0);       // Y axis - green
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(0.0, AXIS_SIZE, 0.0);
      glColor3f(0.0, 0.0, 1.0);       // Z axis - blue
      glVertex3f(0.0, 0.0, 0.0);
      glVertex3f(0.0, 0.0, AXIS_SIZE);
    glEnd();
    glPointSize(5);
    glBegin (GL_POINTS);
      glVertex3f (0.0, 100.0, 0.0);
      glVertex3f (100, 0.0, 0.0);
      glVertex3f (0.0, -100.0, 0.0);
      glVertex3f (-100, 0.0, 0.0);
    glEnd ();
  glEndList();
}

///////////////////////////////////////////////
void initGraphics(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(100, 100);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutCreateWindow("COMP37111 Particles");
  
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  makeAxes();
  initialisePoint();
}


/////////////////////////////////////////////////

int main(int argc, char *argv[])
{
  //double f;
  srand(time(NULL));
  initGraphics(argc, argv);
  glutDisplayFunc(display);
  glEnable(GL_POINT_SMOOTH);
  //glutIdleFunc (drawPoint);
  glutIdleFunc (tickPoint);
  glutMainLoop();
}
