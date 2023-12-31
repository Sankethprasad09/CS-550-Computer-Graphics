//  CS 450 - Intro to Computer Graphics Final Project
//
//  Created by Sanketh Karuturi on 12/5/23.

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MS_IN_THE_ANIMATION_CYCLE   300000
#define NUM_ORBITS 9 // Assuming we have 9 orbits including Pluto
#define ASTEROID_MIN_DIST_FROM_ORBIT 0.1f  // This is an arbitrary value, adjust as needed


// Orbital radii of planets
const float orbit_radii[] = {0.7, 1.2, 2.0, 3.0, 4.3, 5.5, 6.5, 7.5, 8.5}; // Example values

// Define the number of asteroid belts
const int NUM_BELTS = sizeof(orbit_radii) / sizeof(orbit_radii[0]) - 1;

// Define the number of asteroids per belt
const int ASTEROIDS_PER_BELT = 10;

// Total number of asteroids
const int TOTAL_ASTEROIDS = NUM_BELTS * ASTEROIDS_PER_BELT;

struct Asteroid {
    float size;
    float angle;
    float distance;
};

char displayText[50] = {0};  // Global variable to hold the display text

Asteroid asteroids[TOTAL_ASTEROIDS];
  // Array to hold asteroid data

#define ASTEROID_MIN_SIZE 0.01
#define ASTEROID_MAX_SIZE 0.05
#define ASTEROID_ORBIT_RADIUS 1.5  // Assuming this is between Mars and Jupiter
#define DEG2RAD 0.0174532925f

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#include "glew.h"
#endif

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>


//	This is a sample OpenGL / GLUT program
//
//	The objective is to draw a 3d object and change the color of the axes
//		with a glut menu
//
//	The left mouse button does rotation
//	The middle mouse button does scaling
//	The user interface allows:
//		1. The axes to be turned on and off
//		2. The color of the axes to be changed
//		3. Debugging to be turned on and off
//		4. Depth cueing to be turned on and off
//		5. The projection to be changed
//		6. The transformations to be reset
//		7. The program to quit
//
//	Author:			Sanketh Karuturi


// title of these windows:

const char *WINDOWTITLE = { "CS 450 Final Project - Sanketh Karuturi" };
const char *GLUITITLE   = { "User Interface Window" };


// what the glui package defines as true and false:

const int GLUITRUE  = { true  };
const int GLUIFALSE = { false };


// the escape key:

#define ESCAPE		0x1b


// initial window size:

const int INIT_WINDOW_SIZE = { 600 };


// size of the box:

const float BOXSIZE = { 2.f };



// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };


// minimum allowable scale factor:

const float MINSCALE = { 0.05f };


// active mouse buttons (or them together):

const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };


// which projection:

enum Projections
{
    ORTHO,
    PERSP
};


// which button:

enum ButtonVals
{
    RESET,
    QUIT
};


// window background color (rgba):

const GLfloat BACKCOLOR[ ] = { 0., 0., 0., 1. };


// line width for the axes:

const GLfloat AXES_WIDTH   = { 3. };


// the color numbers:
// this order must match the radio button order

enum Colors
{
    RED,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    MAGENTA,
    WHITE,
    BLACK
};

char * ColorNames[ ] =
{
    "Red",
    "Yellow",
    "Green",
    "Cyan",
    "Blue",
    "Magenta",
    "White",
    "Black"
};


// the color definitions:
// this order must match the menu order

const GLfloat Colors[ ][3] =
{
    { 1., 0., 0. },		// red
    { 1., 1., 0. },		// yellow
    { 0., 1., 0. },		// green
    { 0., 1., 1. },		// cyan
    { 0., 0., 1. },		// blue
    { 1., 0., 1. },		// magenta
    { 1., 1., 1. },		// white
    { 0., 0., 0. },		// black
};


// fog parameters:

const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE     = { GL_LINEAR };
const GLfloat FOGDENSITY  = { 0.30f };
const GLfloat FOGSTART    = { 1.5 };
const GLfloat FOGEND      = { 4. };

const float VELOCITY_MERCURY = 47.9f;
const float VELOCITY_VENUS = 35.0f;
const float VELOCITY_EARTH = 29.8f;
const float VELOCITY_MARS = 24.1f;
const float VELOCITY_JUPITER = 13.1f;
const float VELOCITY_SATURN = 9.7f;
const float VELOCITY_URANUS = 6.8f;
const float VELOCITY_NEPTUNE = 5.4f;



// non-constant global variables:

int		ActiveButton;			// current button that is down
GLuint	AxesList;				// list to hold the axes
int		AxesOn;					// != 0 means to draw the axes
int		DebugOn;				// != 0 means to print debugging info
int		DepthCueOn;				// != 0 means to use intensity depth cueing
GLuint	BoxList;				// object display list
int		MainWindow;				// window id for main graphics window
float	Scale;					// scaling factor
int		WhichColor;				// index into Colors[ ]
int		WhichProjection;		// ORTHO or PERSP
int		Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;				// rotation angles in degrees
float   Time;                   // Project 4
bool    Frozen;                 // Project 4
bool    viewToggleState;        // Final Project

float   White[] = {1.,1.,1.,1.};    // Project 4

// Final Project Planet Textures

int     galaxyView;
unsigned char *galaxyTexture;
int galaxyWidth, galaxyHeight;

int     sunView;
unsigned char *sunTexture;
int sunWidth, sunHeight;

int     mercuryView;
unsigned char *mercuryTexture;
int mercuryWidth, mercuryHeight;

int     venusView;
unsigned char *venusTexture;
int venusWidth, venusHeight;

int     earthView;
unsigned char *earthTexture;
int earthWidth, earthHeight;

int     marsView;
unsigned char *marsTexture;
int marsWidth, marsHeight;

int     jupiterView;
unsigned char *jupiterTexture;
int jupiterWidth, jupiterHeight;

int     saturnView;
unsigned char *saturnTexture;
int saturnWidth, saturnHeight;

int     uranusView;
unsigned char *uranusTexture;
int uranusWidth, uranusHeight;

int     neptuneView;
unsigned char *neptuneTexture;
int neptuneWidth, neptuneHeight;

int     plutoView;
unsigned char *plutoTexture;
int plutoWidth, plutoHeight;

#include "sphere.hpp"
#include "bmptotexture.cpp"
#include "/Users/sankethkaruturi/Desktop/SampleMac/OsuSphere.cpp"
#include "loadobjfile.cpp"

// function prototypes:

void	Animate( );
void	Display( );
void	DoAxesMenu( int );
void	DoColorMenu( int );
void	DoDepthMenu( int );
void	DoDebugMenu( int );
void	DoMainMenu( int );
void	DoProjectMenu( int );
void	DoRasterString( float, float, float, char * );
void	DoStrokeString( float, float, float, float, char * );
float	ElapsedSeconds( );
void	InitGraphics( );
void	InitLists( );
void	InitMenus( );
void	Keyboard( unsigned char, int, int );
void	MouseButton( int, int, int, int );
void	MouseMotion( int, int );
void	Reset( );
void	Resize( int, int );
void	Visibility( int );

void	Axes( float );
void	HsvRgb( float[3], float [3] );

// Project 4
float *MulArray3(float, float, float);
float *MulArray3(float, float[3]);
void setConfigurations(float, float, float, float);
void setPointLight(int, float, float, float, float, float, float);
void setSpotLight(int, float, float, float, float, float, float, float, float, float);

// Project 4
// Create array from three floats
float *MulArray3( float a, float b, float c ) {
    static float array[4];
    array[0] = a;
    array[1] = b;
    array[2] = c;
    array[3] = 1.;
    return array;
}

// Create array from multiplier and array
float *MulArray3( float factor, float array0[3] ) {
    static float array[4];
    array[0] = factor * array0[0];
    array[1] = factor * array0[1];
    array[2] = factor * array0[2];
    array[3] = 1.;
    return array;
}

void DrawPlanetName(const char *name, float x, float y, float z)
{
    glPushMatrix();
    glDisable(GL_LIGHTING);  // Disable lighting for text rendering
    glColor3f(1.0, 1.0, 1.0);  // Set text color to white
    glRasterPos3f(x, y, z);  // Set position for the text
    while (*name)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *name);
        name++;
    }
    glEnable(GL_LIGHTING);  // Re-enable lighting
    glPopMatrix();
}

void InitializeAsteroids() {
    for (int i = 0; i < TOTAL_ASTEROIDS; i++) {
        // Randomize the size between ASTEROID_MIN_SIZE and ASTEROID_MAX_SIZE
        asteroids[i].size = ASTEROID_MIN_SIZE + ((ASTEROID_MAX_SIZE - ASTEROID_MIN_SIZE) * rand() / (float)RAND_MAX);

        // Randomize the angle between 0 and 360 degrees
        asteroids[i].angle = (rand() % 360) * DEG2RAD;

        // Assign to a random position between the orbits, not on them
        int orbit_index = rand() % (NUM_ORBITS - 1); // Exclude the last orbit (Pluto)
        float inner_radius = orbit_radii[orbit_index];
        float outer_radius = orbit_radii[orbit_index + 1];

        // Ensure asteroids are between orbits
        float min_distance = inner_radius + ASTEROID_MIN_DIST_FROM_ORBIT;
        float max_distance = outer_radius - ASTEROID_MIN_DIST_FROM_ORBIT;

        // Check for overlap and adjust if necessary
        if (min_distance > max_distance) {
            float temp = min_distance;
            min_distance = max_distance;
            max_distance = temp;
        }

        // Assign a random distance between the min and max distance
        asteroids[i].distance = min_distance + ((max_distance - min_distance) * rand() / (float)RAND_MAX);
    }
}

void RenderText(const char* text, float x, float y, void* font)
{
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void DrawAsteroids() {
    for (int i = 0; i < TOTAL_ASTEROIDS; i++) {
        glPushMatrix();
        // Convert polar to Cartesian coordinates
        float x = asteroids[i].distance * cos(asteroids[i].angle);
        float z = asteroids[i].distance * sin(asteroids[i].angle);

        // Translate to the asteroid's position around the orbit
        glTranslatef(x, 0, z);
        glutSolidSphere(asteroids[i].size, 20, 20);  // Draw asteroid
        glPopMatrix();
    }
}

// Set material characteristics
void setConfigurations(float r, float g, float b, float shininess) {
    glMaterialfv( GL_BACK, GL_EMISSION, MulArray3( 0., 0., 0. ) );
    glMaterialfv( GL_BACK, GL_AMBIENT, MulArray3( .4f, White ) );
    glMaterialfv( GL_BACK, GL_DIFFUSE, MulArray3( 1., White ) );
    glMaterialfv( GL_BACK, GL_SPECULAR, MulArray3( 0., 0., 0. ) );
    glMaterialf ( GL_BACK, GL_SHININESS, 2.f );
    glMaterialfv( GL_FRONT, GL_EMISSION, MulArray3( 0., 0., 0. ) );
    glMaterialfv( GL_FRONT, GL_AMBIENT, MulArray3( r, g, b ) );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, MulArray3( r, g, b ) );
    glMaterialfv( GL_FRONT, GL_SPECULAR, MulArray3( .8f, White ) );
    glMaterialf ( GL_FRONT, GL_SHININESS, shininess );
}

// Set point light location and rgb
void setPointLight( int ilight, float x, float y, float z, float r, float g, float b ) {
    glLightfv( ilight, GL_POSITION, MulArray3( x, y, z ) );
    glLightfv( ilight, GL_AMBIENT, MulArray3( 0., 0., 0. ) );
    glLightfv( ilight, GL_DIFFUSE, MulArray3( r, g, b ) );
    glLightfv( ilight, GL_SPECULAR, MulArray3( r, g, b ) );
    glLightf ( ilight, GL_CONSTANT_ATTENUATION, 1. );
    glLightf ( ilight, GL_LINEAR_ATTENUATION, 0. );
    glLightf ( ilight, GL_QUADRATIC_ATTENUATION, 0. );
    glEnable( ilight );
}

// Set spot light location, direction, and rgb
void setSpotLight( int ilight, float x, float y, float z, float xdir, float ydir, float zdir, float r, float g, float b ) {
    glLightfv( ilight, GL_POSITION, MulArray3( x, y, z ) );
    glLightfv( ilight, GL_SPOT_DIRECTION, MulArray3(xdir,ydir,zdir) );
    glLightf( ilight, GL_SPOT_EXPONENT, 1. );
    glLightf( ilight, GL_SPOT_CUTOFF, 45. );
    glLightfv( ilight, GL_AMBIENT, MulArray3( 0., 0., 0. ) );
    glLightfv( ilight, GL_DIFFUSE, MulArray3( r, g, b ) );
    glLightfv( ilight, GL_SPECULAR, MulArray3( r, g, b ) );
    glLightf ( ilight, GL_CONSTANT_ATTENUATION, 1. );
    glLightf ( ilight, GL_LINEAR_ATTENUATION, 0. );
    glLightf ( ilight, GL_QUADRATIC_ATTENUATION, 0. );
    glEnable( ilight );
}

// main program:

int
main( int argc, char *argv[ ] )
{
    // turn on the glut package:
    // (do this before checking argc and argv since it might
    // pull some command line arguments out)
    
    glutInit( &argc, argv );
    
    
    // setup all the graphics stuff:
    
    InitGraphics( );
    InitializeAsteroids();

    // create the display structures that will not change:
    
    InitLists( );
    
    
    // init all the global variables used by Display( ):
    // this will also post a redisplay
    
    Reset( );
    
    
    // setup all the user interface stuff:
    
    InitMenus( );
    
    
    // draw the scene once and wait for some interaction:
    // (this will never return)
    
    glutSetWindow( MainWindow );
    glutMainLoop( );
    
    
    // this is here to make the compiler happy:
    
    return 0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutMainLoop( ) do it

void
Animate( )
{
    // put animation stuff in here -- change some global variables
    // for Display( ) to find:
    
    // force a call to Display( ) next time it is convenient:
    
    glutSetWindow( MainWindow );
    
    // Project 4
    int ms = glutGet( GLUT_ELAPSED_TIME );	// milliseconds
    ms  %=  MS_IN_THE_ANIMATION_CYCLE;
    Time = (float)ms  /  (float)MS_IN_THE_ANIMATION_CYCLE;        // [ 0., 1. )

    glutPostRedisplay( );
}

#define DEG2RAD 0.0174532925f

// Function to draw an orbit with a given radius
void DrawOrbit(float radius) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; i++) {
        float degInRad = i * DEG2RAD;
        glVertex3f(cos(degInRad) * radius, 0, sin(degInRad) * radius);
    }
    glEnd();
}

// draw the complete scene:

void
Display( )
{
    if( DebugOn != 0 )
    {
        fprintf( stderr, "Display\n" );
    }
    
    
    // set which window we want to do the graphics into:
    
    glutSetWindow( MainWindow );
    
    
    // erase the background:
    
    glDrawBuffer( GL_BACK );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable( GL_DEPTH_TEST );
    
    
    // specify shading to be flat:
    
    glShadeModel( GL_SMOOTH );
    
    
    // set the viewport to a square centered in the window:
    
    GLsizei vx = glutGet( GLUT_WINDOW_WIDTH );
    GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT );
    GLsizei v = vx < vy ? vx : vy;			// minimum dimension
    GLint xl = ( vx - v ) / 2;
    GLint yb = ( vy - v ) / 2;
    glViewport( xl, yb,  v, v );
    
    
    // set the viewing volume:
    // remember that the Z clipping  values are actually
    // given as DISTANCES IN FRONT OF THE EYE
    // USE gluOrtho2D( ) IF YOU ARE DOING 2D !
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    if( WhichProjection == ORTHO )
        glOrtho( -3., 3.,     -3., 3.,     0.1, 1000. );
    else
        gluPerspective( 90., 1.,	0.1, 1000. );
    
    
    // place the objects into the scene:
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    // Render the text
    RenderText(displayText, 10, 10, GLUT_BITMAP_HELVETICA_18);
    
    // set the eye position, look-at position, and up-vector:
    
    // Final Project - Toggle views
    if (viewToggleState) {
        // Zoomed out view
        gluLookAt( 5., 5., 5.,     0., 0., 0.,     0., 1., 0. );
    } else {
        // Zoomed in view
        gluLookAt( 2., 2., 2.,     0., 0., 0.,     0., 1., 0. );
    }
    
    
    // rotate the scene:
    
    glRotatef( (GLfloat)Yrot, 0., 1., 0. );
    glRotatef( (GLfloat)Xrot, 1., 0., 0. );
    
    
    // uniformly scale the scene:
    
    if( Scale < MINSCALE )
        Scale = MINSCALE;
    glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );

    // Draw Asteroids
    glColor3f(0.5, 0.5, 0.5);  // Grey color for asteroids
    DrawAsteroids();

    // Draw orbits
    glColor3f(1.0, 1.0, 1.0); // Set orbit color to white
    DrawOrbit(0.7); // Orbit for Mercury
    DrawOrbit(1.2); // Orbit for Venus
    DrawOrbit(2.0); // Orbit for Earth
    DrawOrbit(3.0); // Orbit for Mars
    DrawOrbit(4.3); // Orbit for Jupiter
    DrawOrbit(5.5); // Orbit for Saturn
    DrawOrbit(6.5); // Orbit for Uranus
    DrawOrbit(7.5); // Orbit for Neptune
    DrawOrbit(8.5); // Orbit for Pluto

    // Earth
    glPushMatrix();
    glRotatef((GLfloat)360. * Time * 100, 0., 1., 0.);
    glTranslatef(-2, 0, 0);
    glRotatef((GLfloat)360. * Time * 100, 0., 1., 0.);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, earthWidth, earthHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, earthTexture);
    OsuSphere(0.25, 50, 50, earthView, 0);
    DrawPlanetName("Earth", 0, 0.3, 0);  // Display the name above the planet
    glPopMatrix();

    // Mercury
    glPushMatrix();
    glRotatef((GLfloat)360. * Time * 160, 0., 1., 0.);
    glTranslatef(-0.7, 0, 0);
    glRotatef((GLfloat)360. * Time * 1.69, 0., 1., 0.);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, mercuryWidth, mercuryHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mercuryTexture);
    OsuSphere(0.1, 50, 50, mercuryView, 0);
    DrawPlanetName("Mercury", 0, 0.15, 0);  // Adjust position as needed
    glPopMatrix();

    // Venus
    glPushMatrix();
    glRotatef((GLfloat)360. * Time * 117, 0., 1., 0.);
    glTranslatef(-1.2, 0, 0);
    glRotatef((GLfloat)360. * Time * 0.41, 0., 1., 0.);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, venusWidth, venusHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, venusTexture);
    OsuSphere(0.2, 50, 50, venusView, 0);
    DrawPlanetName("Venus", 0, 0.25, 0);  // Adjust position as needed
    glPopMatrix();

    // Mars
    glPushMatrix();
    glRotatef((GLfloat)360. * Time * 80, 0., 1., 0.);
    glTranslatef(-3, 0, 0);
    glRotatef((GLfloat)360. * Time * 100, 0., 1., 0.);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, marsWidth, marsHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, marsTexture);
    OsuSphere(0.2, 50, 50, marsView, 0);
    DrawPlanetName("Mars", 0, 0.25, 0);  // Adjust position as needed
    glPopMatrix();

    // Jupiter
    glPushMatrix();
    glRotatef((GLfloat)360. * Time * 43, 0., 1., 0.);
    glTranslatef(-4.3, 0, 0);
    glRotatef((GLfloat)360. * Time * 395, 0., 1., 0.);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, jupiterWidth, jupiterHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, jupiterTexture);
    OsuSphere(0.5, 50, 50, jupiterView, 0);
    DrawPlanetName("Jupiter", 0, 0.6, 0);  // Adjust position as needed
    glPopMatrix();

    // Saturn
    glPushMatrix();
    glRotatef((GLfloat)360. * Time * 32, 0., 1., 0.);
    glTranslatef(-5.5, 0, 0);
    glRotatef((GLfloat)360. * Time * 240, 0., 1., 0.);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, saturnWidth, saturnHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, saturnTexture);
    OsuSphere(0.5, 50, 50, saturnView, 0);
    DrawPlanetName("Saturn", 0, 0.6, 0);  // Adjust position as needed
    glPopMatrix();

    // Uranus
    glPushMatrix();
    glRotatef((GLfloat)360. * Time * 22, 0., 1., 0.);
    glTranslatef(-6.5, 0, 0);
    glRotatef((GLfloat)360. * Time * 70, 0., 1., 0.);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, uranusWidth, uranusHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, uranusTexture);
    OsuSphere(0.3, 50, 50, uranusView, 0);
    DrawPlanetName("Uranus", 0, 0.35, 0);  // Adjust position as needed
    glPopMatrix();

    // Neptune
    glPushMatrix();
    glRotatef((GLfloat)360. * Time * 18, 0., 1., 0.);
    glTranslatef(-7.5, 0, 0);
    glRotatef((GLfloat)360. * Time * 150, 0., 1., 0.);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, neptuneWidth, neptuneHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, neptuneTexture);
    OsuSphere(0.23, 50, 50, neptuneView, 0);
    DrawPlanetName("Neptune", 0, 0.3, 0);  // Adjust position as needed
    glPopMatrix();

    // Pluto
    glPushMatrix();
    glRotatef((GLfloat)360. * Time * 15, 0., 1., 0.);
    glTranslatef(-8.5, 0, 0);
    glRotatef((GLfloat)360. * Time * 0.06, 0., 1., 0.);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, plutoWidth, plutoHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, plutoTexture);
    OsuSphere(0.09, 50, 50, plutoView, 0);
    DrawPlanetName("Pluto", 0, 0.15, 0);  // Adjust position as needed
    glPopMatrix();

    // set the fog parameters:
    
    if( DepthCueOn != 0 )
    {
        glFogi( GL_FOG_MODE, FOGMODE );
        glFogfv( GL_FOG_COLOR, FOGCOLOR );
        glFogf( GL_FOG_DENSITY, FOGDENSITY );
        glFogf( GL_FOG_START, FOGSTART );
        glFogf( GL_FOG_END, FOGEND );
        glEnable( GL_FOG );
    }
    else
    {
        glDisable( GL_FOG );
    }
    
    
    // possibly draw the axes:
    
    if( AxesOn != 1 )
    {
        glColor3fv( &Colors[WhichColor][0] );
        glCallList( AxesList );
    }
    
    
    // since we are using glScalef( ), be sure normals get unitized:
    
    glEnable( GL_NORMALIZE );
    
    // Final Project - Enable textures
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glDisable(GL_LIGHTING);
    
    // Sun (Draw here to not include in lighting effects)
    glPushMatrix();
    glRotatef((GLfloat)360. * Time * 100, 0., 1., 0.);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, sunWidth, sunHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, sunTexture);
    OsuSphere( 0.5, 50, 50, sunView, 0);
    glPopMatrix();

    // Galaxy (Draw here to not include in lighting effects)
    glPushMatrix();
    //glRotatef((GLfloat)360. * Time * 100, 0., 1., 0.);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, galaxyWidth, galaxyHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, galaxyTexture);
    OsuSphere( 10, 500, 500, galaxyView, 0);
    glPopMatrix();
    
    // Final Project
    glEnable(GL_LIGHTING);
    
    glPushMatrix();
    setPointLight(GL_LIGHT0, 0.6, 0, 0, 1, 1, 1);
    setPointLight(GL_LIGHT1, -0.6, 0, 0, 1, 1, 1);
    setPointLight(GL_LIGHT2, 0, 0.6, 0, 1, 1, 1);
    setPointLight(GL_LIGHT3, 0, -0.6, 0, 1, 1, 1);
    setPointLight(GL_LIGHT4, 0, 0, 0.6, 1, 1, 1);
    setPointLight(GL_LIGHT5, 0, 0, -0.6, 1, 1, 1);
    glPopMatrix();

    // draw the current object:
    
    // Final Project - Display planets
    
    // Mercury
    // Orbit
    glPushMatrix();
    glRotatef((GLfloat)360. * Time * 160, 0., 1., 0.);
    glTranslatef(-0.7, 0, 0);
    
    // Rotate
    glRotatef((GLfloat)360. * Time * 1.69, 0., 1., 0.);
    // Render texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, mercuryWidth, mercuryHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mercuryTexture);
    OsuSphere( 0.1, 50, 50, mercuryView, 0);
    glPopMatrix();
    
    // Venus
    glPushMatrix();
    glRotatef((GLfloat)360. * Time * 117, 0., 1., 0.);
    glTranslatef(-1.2, 0, 0);
    glRotatef((GLfloat)360. * Time * 0.41, 0., 1., 0.);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, venusWidth, venusHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, venusTexture);
    OsuSphere( 0.2, 50, 50, venusView, 0);
    glPopMatrix();
    
    // Earth
    glPushMatrix();
    glRotatef((GLfloat)360. * Time * 100, 0., 1., 0.);
    glTranslatef(-2, 0, 0);
    glRotatef((GLfloat)360. * Time * 100, 0., 1., 0.);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, earthWidth, earthHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, earthTexture);
    OsuSphere( 0.25, 50, 50, earthView, 0);
    glPopMatrix();
    
    // Mars
    glPushMatrix();
    glRotatef((GLfloat)360. * Time * 80, 0., 1., 0.);
    glTranslatef(-3, 0, 0);
    glRotatef((GLfloat)360. * Time * 100, 0., 1., 0.);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, marsWidth, marsHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, marsTexture);
    OsuSphere( 0.2, 50, 50, marsView, 0);
    glPopMatrix();
    
    // Jupiter
    glPushMatrix();
    glRotatef((GLfloat)360. * Time * 43, 0., 1., 0.);
    glTranslatef(-4.3, 0, 0);
    glRotatef((GLfloat)360. * Time * 395, 0., 1., 0.);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, jupiterWidth, jupiterHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, jupiterTexture);
    OsuSphere( 0.5, 50, 50, jupiterView, 0);
    glPopMatrix();
    
    // Saturn
    glPushMatrix();
    glRotatef((GLfloat)360. * Time * 32, 0., 1., 0.);
    glTranslatef(-5.5, 0, 0);
    glRotatef((GLfloat)360. * Time * 240, 0., 1., 0.);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, saturnWidth, saturnHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, saturnTexture);
    OsuSphere( 0.5, 50, 50, saturnView, 0);
    glPopMatrix();
    
    // Uranus
    glPushMatrix();
    glRotatef((GLfloat)360. * Time * 22, 0., 1., 0.);
    glTranslatef(-6.5, 0, 0);
    glRotatef((GLfloat)360. * Time * 70, 0., 1., 0.);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, uranusWidth, uranusHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, uranusTexture);
    OsuSphere( 0.3, 50, 50, uranusView, 0);
    glPopMatrix();
    
    // Neptune
    glPushMatrix();
    glRotatef((GLfloat)360. * Time * 18, 0., 1., 0.);
    glTranslatef(-7.5, 0, 0);
    glRotatef((GLfloat)360. * Time * 150, 0., 1., 0.);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, neptuneWidth, neptuneHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, neptuneTexture);
    OsuSphere( 0.23, 50, 50, neptuneView, 0);
    glPopMatrix();
    
    // Pluto
    glPushMatrix();
    glRotatef((GLfloat)360. * Time * 15, 0., 1., 0.);
    glTranslatef(-8.5, 0, 0);
    glRotatef((GLfloat)360. * Time * 0.06, 0., 1., 0.);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, plutoWidth, plutoHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, plutoTexture);
    OsuSphere( 0.09, 50, 50, plutoView, 0);
    glPopMatrix();
    
    // draw some gratuitous text that just rotates on top of the scene:
    
    glDisable( GL_DEPTH_TEST );
    glColor3f( 0., 1., 1. );
    //    DoRasterString( 0., 1., 0., "Text That Moves" );
    
    
    // draw some gratuitous text that is fixed on the screen:
    //
    // the projection matrix is reset to define a scene whose
    // world coordinate system goes from 0-100 in each axis
    //
    // this is called "percent units", and is just a convenience
    //
    // the modelview matrix is reset to identity as we don't
    // want to transform these coordinates
    
    glDisable( GL_DEPTH_TEST );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluOrtho2D( 0., 100.,     0., 100. );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    glColor3f( 1., 1., 1. );
    DoRasterString( 5., 5., 0., "3D Solar System" );
    
    
    // swap the double-buffered framebuffers:
    
    glutSwapBuffers( );
    
    
    // be sure the graphics buffer has been sent:
    // note: be sure to use glFlush( ) here, not glFinish( ) !
    
    glFlush( );
}


void
DoAxesMenu( int id )
{
    AxesOn = id;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


void
DoColorMenu( int id )
{
    WhichColor = id - RED;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


void
DoDebugMenu( int id )
{
    DebugOn = id;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


void
DoDepthMenu( int id )
{
    DepthCueOn = id;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


// main menu callback:

void
DoMainMenu( int id )
{
    switch( id )
    {
        case RESET:
            Reset( );
            break;
            
        case QUIT:
            // gracefully close out the graphics:
            // gracefully close the graphics window:
            // gracefully exit the program:
            glutSetWindow( MainWindow );
            glFinish( );
            glutDestroyWindow( MainWindow );
            exit( 0 );
            break;
            
        default:
            fprintf( stderr, "Don't know what to do with Main Menu ID %d\n", id );
    }
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


void
DoProjectMenu( int id )
{
    WhichProjection = id;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


// use glut to display a string of characters using a raster font:

void
DoRasterString( float x, float y, float z, char *s )
{
    glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
    
    char c;			// one character to print
    for( ; ( c = *s ) != '\0'; s++ )
    {
        glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
    }
}


// use glut to display a string of characters using a stroke font:

void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
    glPushMatrix( );
    glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
    float sf = ht / ( 119.05f + 33.33f );
    glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
    char c;			// one character to print
    for( ; ( c = *s ) != '\0'; s++ )
    {
        glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
    }
    glPopMatrix( );
}


// return the number of seconds since the start of the program:

float
ElapsedSeconds( )
{
    // get # of milliseconds since the start of the program:
    
    int ms = glutGet( GLUT_ELAPSED_TIME );
    
    // convert it to seconds:
    
    return (float)ms / 1000.f;
}


// initialize the glui window:

void
InitMenus( )
{
    glutSetWindow( MainWindow );
    
    int numColors = sizeof( Colors ) / ( 3*sizeof(int) );
    int colormenu = glutCreateMenu( DoColorMenu );
    for( int i = 0; i < numColors; i++ )
    {
        glutAddMenuEntry( ColorNames[i], i );
    }
    
    int axesmenu = glutCreateMenu( DoAxesMenu );
    glutAddMenuEntry( "Off",  0 );
    glutAddMenuEntry( "On",   1 );
    
    int depthcuemenu = glutCreateMenu( DoDepthMenu );
    glutAddMenuEntry( "Off",  0 );
    glutAddMenuEntry( "On",   1 );
    
    int debugmenu = glutCreateMenu( DoDebugMenu );
    glutAddMenuEntry( "Off",  0 );
    glutAddMenuEntry( "On",   1 );
    
    int projmenu = glutCreateMenu( DoProjectMenu );
    glutAddMenuEntry( "Orthographic",  ORTHO );
    glutAddMenuEntry( "Perspective",   PERSP );
    
    int mainmenu = glutCreateMenu( DoMainMenu );
    glutAddSubMenu(   "Axes",          axesmenu);
    glutAddSubMenu(   "Colors",        colormenu);
    glutAddSubMenu(   "Depth Cue",     depthcuemenu);
    glutAddSubMenu(   "Projection",    projmenu );
    glutAddMenuEntry( "Reset",         RESET );
    glutAddSubMenu(   "Debug",         debugmenu);
    glutAddMenuEntry( "Quit",          QUIT );
    
    // attach the pop-up menu to the right mouse button:
    
    glutAttachMenu( GLUT_RIGHT_BUTTON );
}



// initialize the glut and OpenGL libraries:
//	also setup display lists and callback functions

void
InitGraphics( )
{
    // request the display modes:
    // ask for red-green-blue-alpha color, double-buffering, and z-buffering:
    
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    
    // set the initial window configuration:
    
    glutInitWindowPosition( 0, 0 );
    glutInitWindowSize( INIT_WINDOW_SIZE, INIT_WINDOW_SIZE );
    
    // open the window and set its title:
    
    MainWindow = glutCreateWindow( WINDOWTITLE );
    glutSetWindowTitle( WINDOWTITLE );
    
    // set the framebuffer clear values:
    
    glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );

    // setup the callback functions:
    // DisplayFunc -- redraw the window
    // ReshapeFunc -- handle the user resizing the window
    // KeyboardFunc -- handle a keyboard input
    // MouseFunc -- handle the mouse button going down or up
    // MotionFunc -- handle the mouse moving with a button down
    // PassiveMotionFunc -- handle the mouse moving with a button up
    // VisibilityFunc -- handle a change in window visibility
    // EntryFunc	-- handle the cursor entering or leaving the window
    // SpecialFunc -- handle special keys on the keyboard
    // SpaceballMotionFunc -- handle spaceball translation
    // SpaceballRotateFunc -- handle spaceball rotation
    // SpaceballButtonFunc -- handle spaceball button hits
    // ButtonBoxFunc -- handle button box hits
    // DialsFunc -- handle dial rotations
    // TabletMotionFunc -- handle digitizing tablet motion
    // TabletButtonFunc -- handle digitizing tablet button hits
    // MenuStateFunc -- declare when a pop-up menu is in use
    // TimerFunc -- trigger something to happen a certain time from now
    // IdleFunc -- what to do when nothing else is going on
    
    glutSetWindow( MainWindow );
    glutDisplayFunc( Display );
    glutReshapeFunc( Resize );
    glutKeyboardFunc( Keyboard );
    glutMouseFunc( MouseButton );
    glutMotionFunc( MouseMotion );
    glutPassiveMotionFunc( NULL );
    glutVisibilityFunc( Visibility );
    glutEntryFunc( NULL );
    glutSpecialFunc( NULL );
    glutSpaceballMotionFunc( NULL );
    glutSpaceballRotateFunc( NULL );
    glutSpaceballButtonFunc( NULL );
    glutButtonBoxFunc( NULL );
    glutDialsFunc( NULL );
    glutTabletMotionFunc( NULL );
    glutTabletButtonFunc( NULL );
    glutMenuStateFunc( NULL );
    glutTimerFunc( -1, NULL, 0 );
    
    // Project 4
    glutIdleFunc( Animate );
    
    // init glew (a window must be open to do this):
    
#ifdef WIN32
    GLenum err = glewInit( );
    if( err != GLEW_OK )
    {
        fprintf( stderr, "glewInit Error\n" );
    }
    else
        fprintf( stderr, "GLEW initialized OK\n" );
    fprintf( stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
    
    // Final Project - Binding Textures
    sunTexture = BmpToTexture("/Users/sankethkaruturi/Desktop/SampleMac/sunmap.bmp", &sunWidth, &sunHeight);
    
    mercuryTexture = BmpToTexture("/Users/sankethkaruturi/Desktop/SampleMac/mercurymap.bmp", &mercuryWidth, &mercuryHeight);
    
    venusTexture = BmpToTexture("/Users/sankethkaruturi/Desktop/SampleMac/venusmap.bmp", &venusWidth, &venusHeight);
    
    earthTexture = BmpToTexture("/Users/sankethkaruturi/Desktop/SampleMac/earthmap1k.bmp", &earthWidth, &earthHeight);
    
    marsTexture = BmpToTexture("/Users/sankethkaruturi/Desktop/SampleMac/mars.bmp", &marsWidth, &marsHeight);
    
    jupiterTexture = BmpToTexture("/Users/sankethkaruturi/Desktop/SampleMac/jupitermap.bmp", &jupiterWidth, &jupiterHeight);
    
    saturnTexture = BmpToTexture("/Users/sankethkaruturi/Desktop/SampleMac/saturnmap.bmp", &saturnWidth, &saturnHeight);
    
    uranusTexture = BmpToTexture("/Users/sankethkaruturi/Desktop/SampleMac/uranusmap.bmp", &uranusWidth, &uranusHeight);
    
    neptuneTexture = BmpToTexture("/Users/sankethkaruturi/Desktop/SampleMac/neptunemap.bmp", &neptuneWidth, &neptuneHeight);
    
    plutoTexture = BmpToTexture("/Users/sankethkaruturi/Desktop/SampleMac/plutomap1k.bmp", &plutoWidth, &plutoHeight);

    galaxyTexture = BmpToTexture("/Users/sankethkaruturi/Desktop/SampleMac/galaxy.bmp", &galaxyWidth, &galaxyHeight);
}


// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )

void
InitLists( )
{
    float dx = BOXSIZE / 2.f;
    float dy = BOXSIZE / 2.f;
    float dz = BOXSIZE / 2.f;
    glutSetWindow( MainWindow );
    
    // create the object:
    
    BoxList = glGenLists( 1 );
    glNewList( BoxList, GL_COMPILE );
    
    glBegin( GL_QUADS );
    
    glColor3f( 0., 0., 1. );
    glNormal3f( 0., 0.,  1. );
				glVertex3f( -dx, -dy,  dz );
				glVertex3f(  dx, -dy,  dz );
				glVertex3f(  dx,  dy,  dz );
				glVertex3f( -dx,  dy,  dz );
    
    glNormal3f( 0., 0., -1. );
				glTexCoord2f( 0., 0. );
				glVertex3f( -dx, -dy, -dz );
				glTexCoord2f( 0., 1. );
				glVertex3f( -dx,  dy, -dz );
				glTexCoord2f( 1., 1. );
				glVertex3f(  dx,  dy, -dz );
				glTexCoord2f( 1., 0. );
				glVertex3f(  dx, -dy, -dz );
    
    glColor3f( 1., 0., 0. );
    glNormal3f(  1., 0., 0. );
				glVertex3f(  dx, -dy,  dz );
				glVertex3f(  dx, -dy, -dz );
				glVertex3f(  dx,  dy, -dz );
				glVertex3f(  dx,  dy,  dz );
    
    glNormal3f( -1., 0., 0. );
				glVertex3f( -dx, -dy,  dz );
				glVertex3f( -dx,  dy,  dz );
				glVertex3f( -dx,  dy, -dz );
				glVertex3f( -dx, -dy, -dz );
    
    glColor3f( 0., 1., 0. );
    glNormal3f( 0.,  1., 0. );
				glVertex3f( -dx,  dy,  dz );
				glVertex3f(  dx,  dy,  dz );
				glVertex3f(  dx,  dy, -dz );
				glVertex3f( -dx,  dy, -dz );
    
    glNormal3f( 0., -1., 0. );
				glVertex3f( -dx, -dy,  dz );
				glVertex3f( -dx, -dy, -dz );
				glVertex3f(  dx, -dy, -dz );
				glVertex3f(  dx, -dy,  dz );
    
    glEnd( );
    
    glEndList( );
    
    // create the axes:
    
    AxesList = glGenLists( 1 );
    glNewList( AxesList, GL_COMPILE );
    glLineWidth( AXES_WIDTH );
    Axes( 1.5 );
    glLineWidth( 1. );
    glEndList( );
}


// the keyboard callback:

void
Keyboard( unsigned char c, int x, int y )
{
    if( DebugOn != 0 )
        fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );
    
    switch( c )
    {
            
        // Project 4
        case 'M':
        case 'm':
            sprintf(displayText, "Orbital Velocity of Mercury: %.1f km/s", VELOCITY_MERCURY);
            break;

        case 'V':
        case 'v':
            sprintf(displayText, "Orbital Velocity of Venus: %.1f km/s", VELOCITY_VENUS);
            break;

        case 'E':
        case 'e':
            sprintf(displayText, "Orbital Velocity of Earth: %.1f km/s", VELOCITY_EARTH);
            break;

        case 'R':
        case 'r':
            sprintf(displayText, "Orbital Velocity of Mars: %.1f km/s", VELOCITY_MARS);
            break;

        case 'J':
        case 'j':
            sprintf(displayText, "Orbital Velocity of Jupiter: %.1f km/s", VELOCITY_JUPITER);
            break;

        case 'S':
        case 's':
            sprintf(displayText, "Orbital Velocity of Saturn: %.1f km/s", VELOCITY_SATURN);
            break;

        case 'U':
        case 'u':
            sprintf(displayText, "Orbital Velocity of Uranus: %.1f km/s", VELOCITY_URANUS);
            break;

        case 'N':
        case 'n':
            sprintf(displayText, "Orbital Velocity of Neptune: %.1f km/s", VELOCITY_NEPTUNE);
            break;

        case 'f':
        case 'F':
            Frozen = !Frozen;
            if (Frozen) {
                glutIdleFunc(NULL);
            } else {
                glutIdleFunc(Animate);
            }
            break;
            
        case 't':
            viewToggleState = !viewToggleState;
            break;
        case 'o':
        case 'O':
            WhichProjection = ORTHO;
            break;
            
        case 'p':
        case 'P':
            WhichProjection = PERSP;
            break;
            
        case 'q':
        case 'Q':
        case ESCAPE:
            DoMainMenu( QUIT );	// will not return here
            break;				// happy compiler
            
        default:
            fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
    }
    
    // force a call to Display( ):
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


// called when the mouse button transitions down or up:

void
MouseButton( int button, int state, int x, int y )
{
    int b = 0;			// LEFT, MIDDLE, or RIGHT
    
    if( DebugOn != 0 )
        fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );
    
    
    // get the proper button bit mask:
    
    switch( button )
    {
        case GLUT_LEFT_BUTTON:
            b = LEFT;		break;
            
        case GLUT_MIDDLE_BUTTON:
            b = MIDDLE;		break;
            
        case GLUT_RIGHT_BUTTON:
            b = RIGHT;		break;
            
        default:
            b = 0;
            fprintf( stderr, "Unknown mouse button: %d\n", button );
    }
    
    
    // button down sets the bit, up clears the bit:
    
    if( state == GLUT_DOWN )
    {
        Xmouse = x;
        Ymouse = y;
        ActiveButton |= b;		// set the proper bit
    }
    else
    {
        ActiveButton &= ~b;		// clear the proper bit
    }
}


// called when the mouse moves while a button is down:

void
MouseMotion( int x, int y )
{
    if( DebugOn != 0 )
        fprintf( stderr, "MouseMotion: %d, %d\n", x, y );
    
    
    int dx = x - Xmouse;		// change in mouse coords
    int dy = y - Ymouse;
    
    if( ( ActiveButton & LEFT ) != 0 )
    {
        Xrot += ( ANGFACT*dy );
        Yrot += ( ANGFACT*dx );
    }
    
    
    if( ( ActiveButton & MIDDLE ) != 0 )
    {
        Scale += SCLFACT * (float) ( dx - dy );
        
        // keep object from turning inside-out or disappearing:
        
        if( Scale < MINSCALE )
            Scale = MINSCALE;
    }
    
    Xmouse = x;			// new current position
    Ymouse = y;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset( )
{
    ActiveButton = 0;
    AxesOn = 1;
    DebugOn = 0;
    DepthCueOn = 0;
    Scale  = 1.0;
    WhichColor = WHITE;
    WhichProjection = PERSP;
    Xrot = Yrot = 0.;
    
    // Project 4
    Frozen = false;
}


// called when user resizes the window:

void
Resize( int width, int height )
{
    if( DebugOn != 0 )
        fprintf( stderr, "ReSize: %d, %d\n", width, height );
    
    // don't really need to do anything since window size is
    // checked each time in Display( ):
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


// handle a change to the window's visibility:

void
Visibility ( int state )
{
    if( DebugOn != 0 )
        fprintf( stderr, "Visibility: %d\n", state );
    
    if( state == GLUT_VISIBLE )
    {
        glutSetWindow( MainWindow );
        glutPostRedisplay( );
    }
    else
    {
        // could optimize by keeping track of the fact
        // that the window is not visible and avoid
        // animating or redrawing it ...
    }
}



///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////


// the stroke characters 'X' 'Y' 'Z' :

static float xx[ ] = {
    0.f, 1.f, 0.f, 1.f
};

static float xy[ ] = {
    -.5f, .5f, .5f, -.5f
};

static int xorder[ ] = {
    1, 2, -3, 4
};

static float yx[ ] = {
    0.f, 0.f, -.5f, .5f
};

static float yy[ ] = {
    0.f, .6f, 1.f, 1.f
};

static int yorder[ ] = {
    1, 2, 3, -2, 4
};

static float zx[ ] = {
    1.f, 0.f, 1.f, 0.f, .25f, .75f
};

static float zy[ ] = {
    .5f, .5f, -.5f, -.5f, 0.f, 0.f
};

static int zorder[ ] = {
    1, 2, 3, 4, -5, 6
};

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)

void
Axes( float length )
{
    glBegin( GL_LINE_STRIP );
    glVertex3f( length, 0., 0. );
    glVertex3f( 0., 0., 0. );
    glVertex3f( 0., length, 0. );
    glEnd( );
    glBegin( GL_LINE_STRIP );
    glVertex3f( 0., 0., 0. );
    glVertex3f( 0., 0., length );
    glEnd( );
    
    float fact = LENFRAC * length;
    float base = BASEFRAC * length;
    
    glBegin( GL_LINE_STRIP );
    for( int i = 0; i < 4; i++ )
    {
        int j = xorder[i];
        if( j < 0 )
        {
            
            glEnd( );
            glBegin( GL_LINE_STRIP );
            j = -j;
        }
        j--;
        glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
    }
    glEnd( );
    
    glBegin( GL_LINE_STRIP );
    for( int i = 0; i < 5; i++ )
    {
        int j = yorder[i];
        if( j < 0 )
        {
            
            glEnd( );
            glBegin( GL_LINE_STRIP );
            j = -j;
        }
        j--;
        glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
    }
    glEnd( );
    
    glBegin( GL_LINE_STRIP );
    for( int i = 0; i < 6; i++ )
    {
        int j = zorder[i];
        if( j < 0 )
        {
            
            glEnd( );
            glBegin( GL_LINE_STRIP );
            j = -j;
        }
        j--;
        glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
    }
    glEnd( );
    
}


// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//		glColor3fv( rgb );

void
HsvRgb( float hsv[3], float rgb[3] )
{
    // guarantee valid input:
    
    float h = hsv[0] / 60.f;
    while( h >= 6. )	h -= 6.;
    while( h <  0. ) 	h += 6.;
    
    float s = hsv[1];
    if( s < 0. )
        s = 0.;
    if( s > 1. )
        s = 1.;
    
    float v = hsv[2];
    if( v < 0. )
        v = 0.;
    if( v > 1. )
        v = 1.;
    
    // if sat==0, then is a gray:
    
    if( s == 0.0 )
    {
        rgb[0] = rgb[1] = rgb[2] = v;
        return;
    }
    
    // get an rgb from the hue itself:
    
    float i = floor( h );
    float f = h - i;
    float p = v * ( 1.f - s );
    float q = v * ( 1.f - s*f );
    float t = v * ( 1.f - ( s * (1.f-f) ) );
    
    float r, g, b;			// red, green, blue
    switch( (int) i )
    {
        case 0:
            r = v;	g = t;	b = p;
            break;
            
        case 1:
            r = q;	g = v;	b = p;
            break;
            
        case 2:
            r = p;	g = v;	b = t;
            break;
            
        case 3:
            r = p;	g = q;	b = v;
            break;
            
        case 4:
            r = t;	g = p;	b = v;
            break;
            
        case 5:
            r = v;	g = p;	b = q;
            break;
    }
    
    
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
}
