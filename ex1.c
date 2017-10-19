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
  int    ID;
  GLfloat position;
  GLfloat velocity;
  GLfloat acceleration;  
}point;

typedef struct { 
  char*    name;
  GLfloat  position;
  int  numParticles;
  point particles[100];
}particleEmitter;

particleEmitter testEmitter;
//point testPoint;

float timeStep = 1;
float simTime;

///////////////////////////////////////////////

double myRandom()
//Return random double within range [0,1]
{
  return (rand()/(double)RAND_MAX);
}

///////////////////////////////////////////////
void initialiseEmitter()
{
  testEmitter.name = "parent";
  testEmitter.position = 0;
  testEmitter.numParticles = 0;
  printf("blah\n");
}

void initialisePoint()
{
  point testPoint;
  
  testPoint.ID = testEmitter.numParticles;
  printf("particle ID: %i\n", testPoint.ID);
  testPoint.position = testEmitter.position;
  testPoint.velocity = 0.1;

  double random = myRandom();
  printf("acceleration modifier: %f\n", random);
  testPoint.acceleration = 0.5 * random;
  
  testEmitter.particles[testEmitter.numParticles] = testPoint;
  testEmitter.numParticles += 1;
}

void tickPoint(void)
{
  int i = 0;

  for (i; i < sizeof(testEmitter.particles) / sizeof(point); i++)
  {
    simTime += timeStep;

    //float displacement = (testEmitter.particles[i].velocity * timeStep) + (0.5 * testEmitter.particles[i].acceleration * timeStep * timeStep);
    float displacement = timeStep * (testEmitter.particles[i].velocity + timeStep * testEmitter.particles[i].acceleration / 2);
    testEmitter.particles[i].velocity += timeStep * testEmitter.particles[i].acceleration ;;
    //printf("displacement: %f\n", displacement);
    testEmitter.particles[i].position += displacement;
  }

  glutPostRedisplay();
}

void drawPoint()
{
  glBegin (GL_POINTS);
      int i = 0;
      for (i; i < sizeof(testEmitter.particles) / sizeof(point); i++)
      {
        //printf("particles size at point %i is: %d\n", i, sizeof(testEmitter.particles) / sizeof(point));
        //printf("testEmitter.particle[%i] acceleration is: %f\n", i, testEmitter.particles[i].acceleration);
        glVertex3f (0.0, 0.0, testEmitter.particles[i].position);
      }
      //glVertex3f (0.0, 0.0, testPoint.position);
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
  switch (key)
  {
    case 27:  /* Escape key */
      exit(0);
      glutPostRedisplay();
      break;
    case 32:
      initialisePoint();
      glutPostRedisplay();
      break;
  }
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
    //glBegin (GL_POINTS);
      //glVertex3f (0.0, 100.0, 0.0);
      //glVertex3f (100, 0.0, 0.0);
      //glVertex3f (0.0, -100.0, 0.0);
      //glVertex3f (-100, 0.0, 0.0);
    //glEnd ();
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
  initialiseEmitter();
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
  glutIdleFunc(tickPoint);
  glutMainLoop();
}




//generate more particles that start at same position and move at different speeds
//currently on changing y position, must do for x and z too with repect to gravity

//do the particles need to live inside the emitter? initialisepoint() would then be 
//run from inside emitter and we wouldn't need the points to keep track of their parent emitter