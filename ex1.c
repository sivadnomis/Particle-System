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

////////////////////////////////////////////////////////////////

// Display list for coordinate axis 
GLuint axisList;

int AXIS_SIZE= 200;
int axisEnabled= 1;

typedef struct { 
  int    ID;
  GLfloat yPosition;
  GLfloat xPosition;
  GLfloat zPosition;
  GLfloat velocity;
  GLfloat acceleration;  
}point;

typedef struct { 
  char*    name;
  GLfloat  yPosition;
  GLfloat  xPosition;
  int  numParticles;
  point particles[10000];
}particleEmitter;

particleEmitter testEmitter;
//point testPoint;

int timeStep = 1;
int simTime;
int currentView;

///////////////////////////////////////////////

double myRandom()
//Return random double within range [0,1]
{
  return (rand()/(double)RAND_MAX);
}

///////////////////////////////////////////////

double myRandomInclNeg()
//Return random double within range [0,1]
{
  return (rand()/(double)RAND_MAX) * 2 - 1;
}

///////////////////////////////////////////////
void initialiseEmitter(int yPos)
{
  testEmitter.name = "parent";
  testEmitter.yPosition = yPos;
  testEmitter.numParticles = 0;
  //printf("blah\n");
}

void tickEmitter(void)
{
  simTime += timeStep;
  // if (simTime % 20 == 0)
  {
    //testEmitter.xPosition = 100 * cos(simTime) + testEmitter.xPosition;
    //estEmitter.yPosition = 100 * sin(simTime) + testEmitter.yPosition;
  }

  //printf("emitter yPosition: %f\n", testEmitter.yPosition);
  initialisePoint();
  tickPoint();
}

void initialisePoint()
{
  point testPoint;
  
  testPoint.ID = testEmitter.numParticles;
  printf("particle ID: %i\n", testPoint.ID);
  testPoint.yPosition = testEmitter.yPosition;
  testPoint.xPosition = testEmitter.xPosition;
  testPoint.velocity = 0.1;

  double random = myRandom();
  //printf("acceleration modifier: %f\n", random);
  testPoint.acceleration = 0.5 * random;
  
  testEmitter.particles[testEmitter.numParticles] = testPoint;
  testEmitter.numParticles += 1;
}

void tickPoint(void)
{
  int i = 0;
  for (; i < sizeof(testEmitter.particles) / sizeof(point); i++)
  {
    //gravity equation
    float yDisplacement = timeStep * (testEmitter.particles[i].velocity + timeStep * testEmitter.particles[i].acceleration / 2);
    testEmitter.particles[i].velocity += timeStep * testEmitter.particles[i].acceleration ;;
    //printf("ID: %i, displacement: %f\n", testEmitter.particles[i].ID, yDisplacement);
    testEmitter.particles[i].yPosition -= yDisplacement;

    float xDisplacement = testEmitter.particles[i].velocity * myRandomInclNeg();
    testEmitter.particles[i].xPosition += xDisplacement;

    float zDisplacement = testEmitter.particles[i].velocity * myRandomInclNeg();
    testEmitter.particles[i].zPosition += zDisplacement;
  }

  glutPostRedisplay();
}

void drawPoint()
{
  // enable blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   
  // enable point smoothing
  glEnable(GL_POINT_SMOOTH);
  glPointSize(5.0f);

  glBegin (GL_POINTS);
      int i = 0;
      for (; i < sizeof(testEmitter.particles) / sizeof(point); i++)
      {
        //printf("particles size at point %i is: %d\n", i, sizeof(testEmitter.particles) / sizeof(point));
        //printf("testEmitter.particle[%i] acceleration is: %f\n", i, testEmitter.particles[i].acceleration);
        //printf("i: %i, ID: %ix: %f y: %f\n", i, testEmitter.particles[i].ID, testEmitter.particles[i].xPosition, testEmitter.particles[i].yPosition);
        glVertex3f (testEmitter.particles[i].xPosition, testEmitter.particles[i].yPosition, testEmitter.particles[i].zPosition);
      }
      //glVertex3f (0.0, 0.0, testPoint.yPosition);
  glEnd ();  
}

///////////////////////////////////////////////

void rotateView(void)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  switch (currentView) {
  case 49:
    gluLookAt(0, 100.0, 1000.0,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);
    break;  
  case 50:
    gluLookAt(500, 500.0, 1000.0,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);
    break;
  case 51:
    gluLookAt(100, 500.0, 0.0,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);
    break;
  default:
    gluLookAt(0, 100.0, 1000.0,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);
    break;
  }

  glClear(GL_COLOR_BUFFER_BIT);
  if(axisEnabled) glCallList(axisList);

  glutPostRedisplay();
}

///////////////////////////////////////////////

void display()
{
  glLoadIdentity();
  gluLookAt(0, 100.0, 1000.0,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT);
  // If enabled, draw coordinate axis
  if(axisEnabled) glCallList(axisList);

  rotateView();
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
    case 49:
      currentView = 49;
      break;
    case 50:
      currentView = 50;
      break;
    case 51:
      currentView = 51;
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
  initialiseEmitter(500);
}


/////////////////////////////////////////////////

int main(int argc, char *argv[])
{
  //double f;
  srand(time(NULL));
  initGraphics(argc, argv);
  glutDisplayFunc(display);
  glEnable(GL_POINT_SMOOTH);
  glutIdleFunc(tickEmitter); 
  //glutIdleFunc(tickPoint);
  glutMainLoop();
}




//multiple emmiters to create rain#
//a floor for rain to bounce off/slide