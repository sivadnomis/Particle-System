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
  point particles[20000];
}particleEmitter;

enum colour 
{
  RAINBOW = 0, 
  RED = 1, 
  BLUE = 2,
  GREEN = 3
};

enum render_types 
{
  POINTS = 0, 
  QUADS = 1, 
  LINES = 2,
};

particleEmitter emitters[10];
int numEmitters = 0;

int timeStep = 1;
int simTime;

int currentView;

float gravityModifier = 1;
float velocityModifier = 1;
int particles_colour = 0;
int render_mode = 0;

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

  switch (particles_colour)
  {
    case RAINBOW:  /* Escape key */
      testPoint.rColor = myRandom();
      testPoint.gColor = myRandom();
      testPoint.bColor = myRandom();
      break;
    case RED:
      testPoint.rColor = 1;
      testPoint.gColor = 0;
      testPoint.bColor = 0;
      break;
    case BLUE:
      testPoint.rColor = 0;
      testPoint.gColor = 0;
      testPoint.bColor = 1;
      break;
    case GREEN:
      testPoint.rColor = 0;
      testPoint.gColor = 1;
      testPoint.bColor = 0;
      break;
  }
  
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
    emitters[emitterID].particles[i].velocity *= velocityModifier;
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

  switch (render_mode)
  {
    case POINTS:  /* Escape key */
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
      break;

    case QUADS:
      glBegin(GL_QUADS);
      int i = 0;
      for (; i < numEmitters; i++)
      {
        int k = 0;
        for (; k < sizeof(emitters[0].particles) / sizeof(point); k+=5)
        {
          glColor4f(emitters[i].particles[k].rColor, emitters[i].particles[k].gColor, emitters[i].particles[k].bColor, 0.5);

          glVertex3f (emitters[i].particles[k].xPosition-10, emitters[i].particles[k].yPosition+10, emitters[i].particles[k].zPosition);
          glVertex3f (emitters[i].particles[k].xPosition+10, emitters[i].particles[k].yPosition+10, emitters[i].particles[k].zPosition);
          glVertex3f (emitters[i].particles[k].xPosition+10, emitters[i].particles[k].yPosition-10, emitters[i].particles[k].zPosition);
          glVertex3f (emitters[i].particles[k].xPosition-10, emitters[i].particles[k].yPosition-10, emitters[i].particles[k].zPosition);
        }
      }
      glEnd();
      break;

    case LINES:
      glLineWidth(1); 

      glBegin(GL_LINES);
      int j = 0;
      for (; j < sizeof(emitters[0].particles) / sizeof(point); j+=10)
      {
        glColor3f (emitters[0].particles[j].rColor, emitters[0].particles[j].gColor, emitters[0].particles[j].bColor);

        glVertex3f (0, 800, 0);
        glVertex3f (emitters[0].particles[j].xPosition, emitters[0].particles[j].yPosition, emitters[0].particles[j].zPosition);
      }
      glEnd();
      break;
  } 
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

  if (gravityModifier == 0 && velocityModifier == 1)
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

void change_physics(int item)
{ /* Callback called when the user clicks the right mouse button */
  switch (item)
  {
    case 1:
      gravityModifier += 0.5;
      break;
    case 2:
      gravityModifier -= 0.5;
      break;
    case 3:
      velocityModifier += 0.2;
      break;
    case 4:
      velocityModifier -= 0.2;
      break;
    case 5:
      gravityModifier = 1;
      velocityModifier = 1;
      break;
  }  
}

///////////////////////////////////////////////

void change_colour(int item)
{ /* Callback called when the user clicks the right mouse button */
  switch (item)
  {
    case 1:  /* Escape key */
      particles_colour = RAINBOW;
      break;
    case 2:
      particles_colour = RED;
      break;
    case 3:
      particles_colour = BLUE;
      break;
    case 4:
      particles_colour = GREEN;
      break;
  }  
}

///////////////////////////////////////////////

void change_render_type(int item)
{ /* Callback called when the user clicks the right mouse button */
  switch (item)
  {
    case 1:  /* Escape key */
      render_mode = POINTS;
      break;
    case 2:
      render_mode = QUADS;
      break;
    case 3:
      render_mode = LINES;
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

  int physics_menu = glutCreateMenu(change_physics);
  glutAddMenuEntry("Increase gravity", 1);
  glutAddMenuEntry("Decrease gravity", 2);
  glutAddMenuEntry("Increase particle velocity", 3);
  glutAddMenuEntry("Decrease particle velocity", 4);
  glutAddMenuEntry("Reset", 5);

  int colour_menu = glutCreateMenu(change_colour);
  glutAddMenuEntry("Rainbow", 1);
  glutAddMenuEntry("Red", 2);
  glutAddMenuEntry("Blue", 3);
  glutAddMenuEntry("Green", 4);

  int funky_menu = glutCreateMenu(change_render_type);
  glutAddMenuEntry("Points", 1);
  glutAddMenuEntry("Quads", 2);
  glutAddMenuEntry("Lines", 3);

  int main_menu = glutCreateMenu(change_simulation);
  glutAddSubMenu("Physics", physics_menu);
  glutAddSubMenu("Particle Colour", colour_menu);
  glutAddSubMenu("Funky render options", funky_menu);


  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutMainLoop();
}

//colour and size change depending on position

//move the camera around (priority)
//get some cool shading effects on the particles