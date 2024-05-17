#include<stdio.h> 
#include<GL/glut.h> 

// Raed Massoud | Github.com/RaedMassoud
// 5/17/2024
// 
// 3D Car On Road Simulation | OpenGL | C++ 
// 'V' key to switch perspetive


static const int FPS = 60;
static double roadStripLocationX = 0; // Reset on -1.25 
static double carWheelRotation = 0;

static double angleOfRotationZ = 65;
static double carVibration = 0;
static bool vibrationDir = false;
static bool flag = false;
static bool direction = false;

// Road Func
void cube(double x, double y, double z) {
    glPushMatrix();
    glScalef(x, y, z);

    glColor3f(0.55f, 0.27f, 0.07f); // Brown color for the cube
    glutSolidCube(.99);
    glColor3f(1, 1, 1); // White color of the wire cube
    glutWireCube(1.0);
    glPopMatrix();
    
}

// Car Wheel (x = +-.300 | y = 0 | z = +-.400)
void wheel(double x, double y, double z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(carWheelRotation, 0, 0, 1);
    glColor3f(0, 0, 0);
    glutSolidSphere(.1, 7, 7);
    glutWireSphere(.125, 7, 7);
    glPopMatrix();
}

// Road Strip
void roadStrip(int c) {
    if(c == 1) { // white
        glColor3f(1, 1, 1);
        glBegin(GL_POLYGON); // +-.0625 centers the stips to the main road
            glVertex3f(0, 0, 0.0625);
            glVertex3f(.75, 0, 0.0625);
            glVertex3f(.75, 0, -.0625);
            glVertex3f(0, 0, -.0625);
        glEnd();
    }
    else  {
        // black
        glColor3f(0, 0, 0);
        glBegin(GL_POLYGON); // +-.0625 centers the stips to the main road
            glVertex3f(-.5, 0, 0.125);
            glVertex3f(.75, 0, 0.125);
            glVertex3f(.75, 0, -.125);
            glVertex3f(-.5, 0, -.125);
        glEnd();
    }
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    glPushMatrix();
    glScalef(1.5, 1, 1.5); // view adjustments
    glRotatef(25, 1, 0, 0); // view adjustments
    glTranslatef(-.2, 0, 0);
    glRotatef(angleOfRotationZ, 0 , 1, 0);
    glTranslatef(.2, 0, 0);
    glTranslatef(.3, -.3, 0);

    // ROAD BLOCK
    glPushMatrix();

    cube(6, .25, 2);
    glTranslatef(roadStripLocationX, 0, 0);
    // Road Strip 0
    glPushMatrix();
    glTranslatef(3.1875, .125, 0);
    roadStrip(1);
    glPopMatrix();

    // Road Strip 1
    glPushMatrix();
    glTranslatef(2, .125, 0);
    roadStrip(1);
    glPopMatrix();

    // Road Strip 2
    glPushMatrix();
    glTranslatef(0.8125, .125, 0);
    roadStrip(1);
    glPopMatrix();

    // Road Strip 3
    glPushMatrix();
    glTranslatef(-.375, .125, 0);
    roadStrip(1);
    glPopMatrix();

    // Road Strip 4
    glPushMatrix();
    glTranslatef(-1.5625, .125, 0);
    roadStrip(1);
    glPopMatrix();

    // Road Strip 5
    glPushMatrix();
    glTranslatef(-2.75, .125, 0);
    roadStrip(1);
    glPopMatrix();

    glPopMatrix();

    // Road Strip breaker 1
    glPushMatrix();
    glTranslatef(3.5, .127, 0);
    roadStrip(2);
    glPopMatrix();

    // Road Strip breaker 2
    glPushMatrix();
    glTranslatef(-3.75, .127, 0);
    roadStrip(2);
    glPopMatrix();

    // CAR BLOCK ( MAIN BODY / WHEELS )
    // Main body
    glPushMatrix();
    glTranslatef(-1.75, .04, 0); // Car location
    glTranslatef(0 ,.25, 0); // Raise car from road
    
    glPushMatrix();
    glTranslatef(0, carVibration, 0);
    cube(.75, .125, .5); // car
    glPopMatrix();

    // Wheels
    wheel(-.300, 0, .400); // Car back wheel right
    wheel(-.300, 0, -.400); // Car back wheel left
    wheel(.300, 0, .400); // Car front wheel right
    wheel(.300, 0, -.400); // Car front wheel left

    glPopMatrix();

    glPopMatrix();
    glutSwapBuffers();
}

// Timer func
void timer(int value) {

    // move road strips
    roadStripLocationX -= .05;
    // Reset road strips
    if(roadStripLocationX < -1.25)
        roadStripLocationX = 0;

    // rotate wheels
    carWheelRotation -= 5;
    // reset wheels
    if(carWheelRotation > 360)
        carWheelRotation = 0;

    // CASE FLAG ONE // 45 ANGLE
    if(flag) {
        if(direction)
            angleOfRotationZ += .5;
        else
            angleOfRotationZ -= .5;
    }
    
    // Oscillate logic
    if(angleOfRotationZ >= 160)
        direction = false;
    if(angleOfRotationZ <= 20)
        direction = true;

    // Car vibration ( Oscillates between .05 and 0 )
    if(vibrationDir)
        carVibration += 0.005;
    else
        carVibration -= .005;

    if(carVibration > .05)
        vibrationDir = false;
    if(carVibration < 0)
        vibrationDir = true;

    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer, value);
}

// Keyboard func
void normalKeyHandler(unsigned char key, int x, int y) {
    switch(key) { // start / stop oscillation
        case 'v':
            if(flag) flag = false;
            else flag = true;
            break;
    }
}

// Reshape function
void reshape(GLint w, GLint h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, GLfloat(w) / GLfloat(h), 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Init function
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, // CP
              0.0, 0.0, 0.0, // LAP
              0.0, 1.0, 0.0); // UV
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Car On Road Simulation");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(normalKeyHandler);
    glutTimerFunc(100, timer, 0);

    glutMainLoop();
    return 0;
}
