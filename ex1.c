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

void tickPoint(int emitterID);
void initialisePoint(int emitterID);
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

  GLfloat xGroundDirection;
  GLfloat zGroundDirection;

  GLfloat rColor;
  GLfloat gColor;
  GLfloat bColor;
}point;

typedef struct { 
  int ID;
  GLfloat  yPosition;
  GLfloat  xPosition;
  GLfloat  zPosition;

  int  numParticles;
  point particles[10000];
}particleEmitter;

particleEmitter emitters[10];
int numEmitters = 0;

int timeStep = 1;
int simTime;

int currentView;

float gravityModifier = 1;

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
void initialiseEmitter(int xPos, int yPos, int zPos)
{
  particleEmitter testEmitter;
  testEmitter.ID = numEmitters;

  testEmitter.xPosition = xPos;
  testEmitter.yPosition = yPos;
  testEmitter.zPosition = zPos;

  testEmitter.numParticles = 0;

  emitters[numEmitters] = testEmitter;
  numEmitters += 1;
}

void tickEmitter(void)
{
  simTime += timeStep;
  int i = 0;
  for (; i < numEmitters; i++)
  {
    if (simTime % 20 == 0)
    {      
      emitters[i].xPosition = 100 * cos(simTime) + emitters[i].xPosition;
      emitters[i].yPosition = 100 * sin(simTime) + emitters[i].yPosition;
    }

    initialisePoint(i);
    tickPoint(i);
  }
}

void initialisePoint(int emitterID)
{
  point testPoint;
  
  testPoint.ID = emitters[emitterID].numParticles;
  printf("particle ID: %i\n", testPoint.ID);

  testPoint.xPosition = emitters[emitterID].xPosition;
  testPoint.yPosition = emitters[emitterID].yPosition;  
  testPoint.zPosition = emitters[emitterID].zPosition;

  testPoint.velocity = 0.1;
  double random = myRandom();
  testPoint.acceleration = 0.5 * random;

  testPoint.xGroundDirection = 0;
  testPoint.zGroundDirection = 0;

  testPoint.rColor = myRandom();
  testPoint.gColor = myRandom();
  testPoint.bColor = myRandom();
  
  emitters[emitterID].particles[emitters[emitterID].numParticles] = testPoint;
  emitters[emitterID].numParticles += 1;
}

float distanceFromOrigin(float xPosition, float zPosition)
{
  return sqrt(pow(xPosition, 2) + pow(zPosition, 2));
}

void tickPoint(int emitterID)
{
  int i = 0;
  for (; i < sizeof(emitters[emitterID].particles) / sizeof(point); i++)
  {
    //gravity equation
    float yDisplacement = timeStep * (emitters[emitterID].particles[i].velocity + timeStep * emitters[emitterID].particles[i].acceleration / 2);
    yDisplacement *= gravityModifier;
    emitters[emitterID].particles[i].velocity += timeStep * emitters[emitterID].particles[i].acceleration ;;
    //printf("ID: %i, displacement: %f\n", emitters[emitterID].particles[i].ID, yDisplacement);
    
    if (emitters[emitterID].particles[i].yPosition - yDisplacement > -500)
    {
      emitters[emitterID].particles[i].yPosition -= yDisplacement;

      float xDisplacement = emitters[emitterID].particles[i].velocity * myRandomInclNeg();
      emitters[emitterID].particles[i].xPosition += xDisplacement;

      float zDisplacement = emitters[emitterID].particles[i].velocity * myRandomInclNeg();
      emitters[emitterID].particles[i].zPosition += zDisplacement;
    }
    else
    {
      emitters[emitterID].particles[i].yPosition -= log(distanceFromOrigin(emitters[emitterID].particles[i].xPosition, emitters[emitterID].particles[i].zPosition) / 1000);
      if (emitters[emitterID].particles[i].xGroundDirection == 0 && emitters[emitterID].particles[i].zGroundDirection == 0)
      {
        emitters[emitterID].particles[i].xGroundDirection = myRandom();
        emitters[emitterID].particles[i].zGroundDirection = myRandom();
      }
      else
      {
        if (emitters[emitterID].particles[i].xPosition > 0)
        {
          emitters[emitterID].particles[i].xPosition += emitters[emitterID].particles[i].xGroundDirection;
        }
        else
        {
          emitters[emitterID].particles[i].xPosition -= emitters[emitterID].particles[i].xGroundDirection;
        }
        
        if (emitters[emitterID].particles[i].zPosition > 0)
        {
          emitters[emitterID].particles[i].zPosition += emitters[emitterID].particles[i].zGroundDirection;
        }
        else
        {
          emitters[emitterID].particles[i].zPosition -= emitters[emitterID].particles[i].zGroundDirection;
        }
      }

    }


    
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
  glPointSize(3.0f);
  
  glBegin (GL_POINTS);
      int h = 0;
      for (; h < numEmitters; h++)
      {
        int i = 0;
        for (; i < sizeof(emitters[h].particles) / sizeof(point); i++)
        {
          //printf("particles size at point %i is: %d\n", i, sizeof(emitters[i].particles) / sizeof(point));
          //printf("emitters[i].particle[%i] acceleration is: %f\n", i, emitters[i].particles[i].acceleration);
          //printf("i: %i, ID: %ix: %f y: %f\n", i, emitters[i].particles[i].ID, emitters[i].particles[i].xPosition, emitters[i].particles[i].yPosition);
          glColor3f(emitters[h].particles[i].rColor, emitters[h].particles[i].gColor, emitters[h].particles[i].bColor);
          glVertex3f (emitters[h].particles[i].xPosition, emitters[h].particles[i].yPosition, emitters[h].particles[i].zPosition);
        }
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
    gluLookAt(0, 100.0, 2000.0,
            0.0, -200.0, 0.0,
            0.0, 1.0, 0.0);
    break;  
  case 50:
    gluLookAt(800, 500.0, 1500.0,
            0.0, -200.0, 0.0,
            0.0, 1.0, 0.0);
    break;
  case 51:
    gluLookAt(1, 1500.0, 0.0,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);
    break;
  default:
    gluLookAt(0, 100.0, 2000.0,
            0.0, -200, 0.0,
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
  gluLookAt(0, 100.0, 2000.0,
            0.0, -200.0, 0.0,
            0.0, 1.0, 0.0);
  // Clear the screen
  glClear(GL_COLOR_BUFFER_BIT);
  //Colour of background
  glClearColor(0.0, 0.0, 0.0, 0.0);
  // If enabled, draw coordinate axis
  if(axisEnabled) glCallList(axisList);

  rotateView();
  drawPoint();

  if (gravityModifier == 0)
  {
    glColor3f(100.0, 100.0, 100.0); 
    glRasterPos2f(0.0f, 0.0f);    
    glutBitmapString(GLUT_BITMAP_HELVETICA_18, "PARTY MODE ACTIVATED");
  }


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
      tickEmitter();
      tickEmitter();
      //initialisePoint(0);
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
  //makeAxes();
  initialiseEmitter(100, 500, 0);
  initialiseEmitter(-100, 500, 0);
  initialiseEmitter(0, 500, 100);
  initialiseEmitter(0, 500, -100);
}

///////////////////////////////////////////////

void change_gravity(int item)
{ /* Callback called when the user clicks the right mouse button */
  switch (item)
  {
    case 1:  /* Escape key */
      gravityModifier += 0.5;
      break;
    case 2:
      gravityModifier -= 0.5;
      break;
  }  
}

///////////////////////////////////////////////

void change_simulation(int item)
{ /* Callback called when the user clicks the right mouse button */
  printf ("Change simulation: you clicked item %d\n", item);
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

  int gravity_menu = glutCreateMenu(change_gravity); /* Create the first menu & add items */
  glutAddMenuEntry("Increase gravity", 1);
  glutAddMenuEntry("Decrease gravity", 2);

  int main_menu = glutCreateMenu(change_simulation);
  glutAddSubMenu("gravity shit shit", gravity_menu);

  glutAttachMenu(GLUT_RIGHT_BUTTON); /* Attach it to the right button */

  glutMainLoop();
}

//move the camera around (priority)
//get some cool shading effects on the particles