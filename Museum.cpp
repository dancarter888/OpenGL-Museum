//  ========================================================================
//  Assignment 1
//
//  FILE NAME: Museum.cpp
//
//  ========================================================================

#include <iostream>
#include <cmath>
#include <GL/freeglut.h>
#include "loadBMP.h"
using namespace std;

GLuint txId[8];
float angle=0, eye_x=150, eye_y=15, eye_z=200,  look_x=150, look_z=100;  //Camera parameters
float heliHgt = 0;
int heliAngle = 0;
int propellerAngle = 0;
int ballVal = 0;
GLUquadric *q;

const int N = 22;  // Total number of vertices on the base curve

float vx_init[N] = { 0, 5, 8,  12, 15, 10, 5, 5.5, 6, 5.5, 5, 7, 10, 7, 5, 5.5, 5, 7, 5, 4, 3, 3 };
float vy_init[N] = { 0, 0, 1, 4, 10, 6, 5, 6, 7.5, 9, 10, 11, 15, 13, 13, 14, 15, 17, 16, 16, 15, 14 };
float vz_init[N] = { 0 };


//--------------------------------------------------------------------------------
void loadTexture()
{
    glGenTextures(8, txId); 				// Create a Texture object
    
    glBindTexture(GL_TEXTURE_2D, txId[0]);		//Use this texture
    loadBMP("HeliTopTex.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, txId[1]);		//Use this texture
    loadBMP("CannonTex.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, txId[2]);		//Use this texture
    loadBMP("SkyBoxBack.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, txId[3]);		//Use this texture
    loadBMP("SkyBoxFront.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, txId[4]);		//Use this texture
    loadBMP("SkyBoxLeft.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, txId[5]);		//Use this texture
    loadBMP("SkyBoxRight.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, txId[6]);		//Use this texture
    loadBMP("SkyBoxTop.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, txId[7]);		//Use this texture
    loadBMP("SkyBoxBottom.bmp");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

//-- Ground Plane --------------------------------------------------------
void floor()
{
    float white[4] = { 1., 1., 1., 1. };
    float black[4] = { 0 };
    glColor4f(0.7, 0.7, 0.7, 1.0);  //The floor is gray in colour
    glNormal3f(0.0, 1.0, 0.0);

    //sets specular colour to black
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);

    //The floor is made up of several tiny squares on a 200x200 grid. Each square has a unit size. Slightly under y=0 so shadow shows.
    glBegin(GL_QUADS);
    for (int i = -100; i < 400; i++)
    {
        for (int j = -220; j < 100; j++)
        {
            glVertex3f(i, -0.001, j);
            glVertex3f(i, -0.001, j + 1);
            glVertex3f(i + 1, -0.001, j + 1);
            glVertex3f(i + 1, -0.001, j);
        }
    }
    glEnd();

    //Done to display a diffusely reflecting surface
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);

}

//-------------------------------------------------------------------
void initialise(void)
{
    float grey[4] = { 0.2, 0.2, 0.2, 1.0 };
    float white[4] = { 1.0, 1.0, 1.0, 1.0 };
    float black[4] = { 0, 0, 0, 1 };

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    glEnable(GL_SMOOTH);

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    //Converts light to spotlight with cutoff angle 20 degrees
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 20.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,0.01);

    glClearColor(1.0, 1.0, 1.0, 0.0); //White Background

    loadTexture();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, 1.0, 20.0, 500.0);
}

//-------------------------------------------------------------------
void normal(float x1, float y1, float z1,
    float x2, float y2, float z2,
    float x3, float y3, float z3)
{
    float nx, ny, nz;
    nx = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
    ny = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
    nz = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);

    glNormal3f(nx, ny, nz);
}

//-------------------------------------------------------------------
void drawSkybox() {
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glColor4f(1, 1, 1, 1);

    glBindTexture(GL_TEXTURE_2D, txId[2]);
    glBegin(GL_QUADS);
    ////////////////////// BACK WALL ///////////////////////
    glTexCoord2f(0., 1.);   glVertex3f(-170, 100, -380);
    glTexCoord2f(0., 0.);   glVertex3f(-170, -0.003, -380);
    glTexCoord2f(1., 0.);  glVertex3f(470, -0.003, -380);
    glTexCoord2f(1., 1.);  glVertex3f(470, 100, -380);
    glEnd();
    
 
    glBindTexture(GL_TEXTURE_2D, txId[3]);
    glBegin(GL_QUADS);
    ////////////////////// FRONT WALL ///////////////////////
        glTexCoord2f(0., 1.); glVertex3f(470, 100, 260);
        glTexCoord2f(0., 0.); glVertex3f(470, -0.003, 260);
        glTexCoord2f(1., 0.);  glVertex3f(-170, -0.003, 260);
        glTexCoord2f(1., 1.);  glVertex3f(-170, 100, 260);
    glEnd();
               
    glBindTexture(GL_TEXTURE_2D, txId[4]);  
    glBegin(GL_QUADS);
    ////////////////////// LEFT WALL ///////////////////////
        glTexCoord2f(0., 1.);  glVertex3f(-170, 100, 260);
        glTexCoord2f(0., 0.);  glVertex3f(-170, -0.003, 260);
        glTexCoord2f(1., 0.); glVertex3f(-170, -0.003, -380);
        glTexCoord2f(1., 1.); glVertex3f(-170, 100, -380);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, txId[5]);  
    glBegin(GL_QUADS);
    //////////////////////// RIGHT WALL ///////////////////////
        glTexCoord2f(0., 1.);  glVertex3f(470, 100, -380);
        glTexCoord2f(0., 0.);  glVertex3f(470, -0.003, -380);
        glTexCoord2f(1., 0.); glVertex3f(470, -0.003, 260);
        glTexCoord2f(1., 1.); glVertex3f(470, 100, 260);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, txId[6]);
    glBegin(GL_QUADS);
    //////////////////////// TOP WALL ///////////////////////
    glTexCoord2f(1., 0.);  glVertex3f(-170, 100, 260);
    glTexCoord2f(1., 1.);  glVertex3f(-170, 100, -380);
    glTexCoord2f(0., 1.); glVertex3f(470, 100, -380);
    glTexCoord2f(0., 0.); glVertex3f(470, 100, 260);
    glEnd();    

    glBindTexture(GL_TEXTURE_2D, txId[7]);
    glBegin(GL_QUADS);
    //////////////////////// BOTTOM WALL ///////////////////////
        glTexCoord2f(1., 0.);  glVertex3f(-170, -0.003, -380);
        glTexCoord2f(1., 1.);  glVertex3f(-170, -0.003, 260);
        glTexCoord2f(0., 1.); glVertex3f(470, -0.003, 260);
        glTexCoord2f(0., 0.); glVertex3f(470, -0.003, -380);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}

//-------------------------------------------------------------------
void drawExhibitBoundaries() {
    glColor3f(1, 0, 0);

    //Heli Boundary - Back
    glPushMatrix();
    glTranslatef(0, 0.5, -80);
    glScalef(161, 1, 1);
    glutSolidCube(1);
    glPopMatrix();

    //Heli Boundary - Front
    glPushMatrix();
    glTranslatef(0, 0.5, 80);
    glScalef(161, 1, 1);
    glutSolidCube(1);
    glPopMatrix();

    //Heli Boundary - Left
    glPushMatrix();
    glTranslatef(-80, 0.5, 0);
    glScalef(1, 1, 161);
    glutSolidCube(1);
    glPopMatrix();

    //Heli Boundary - Right
    glPushMatrix();
    glTranslatef(80, 0.5, 0);
    glScalef(1, 1, 161);
    glutSolidCube(1);
    glPopMatrix();

    //Cannon Boundary - Back
    glPushMatrix();
    glTranslatef(150, 0.5, -200);
    glScalef(201, 1, 1);
    glutSolidCube(1);
    glPopMatrix();

    //Cannon Boundary - Front
    glPushMatrix();
    glTranslatef(150, 0.5, -100);
    glScalef(201, 1, 1);
    glutSolidCube(1);
    glPopMatrix();

    //Cannon Boundary - Left
    glPushMatrix();
    glTranslatef(50, 0.5, -150);
    glScalef(1, 1, 101);
    glutSolidCube(1);
    glPopMatrix();

    //Cannon Boundary - Right
    glPushMatrix();
    glTranslatef(250, 0.5, -150);
    glScalef(1, 1, 101);
    glutSolidCube(1);
    glPopMatrix();

    //Star Boundary - Back
    glPushMatrix();
    glTranslatef(300, 0.5, -80);
    glScalef(161, 1, 1);
    glutSolidCube(1);
    glPopMatrix();

    //Star Boundary - Front
    glPushMatrix();
    glTranslatef(300, 0.5, 80);
    glScalef(161, 1, 1);
    glutSolidCube(1);
    glPopMatrix();

    //Star Boundary - Left
    glPushMatrix();
    glTranslatef(220, 0.5, 0);
    glScalef(1, 1, 161);
    glutSolidCube(1);
    glPopMatrix();

    //Star Boundary - Right
    glPushMatrix();
    glTranslatef(380, 0.5, 0);
    glScalef(1, 1, 161);
    glutSolidCube(1);
    glPopMatrix();

}

//-------------------------------------------------------------------
void drawMuseum() {

    glColor3f(0.5, 0.5, 0.5);
    
    glPushMatrix(); //Back Wall
        glTranslatef(150, 25, -220);
        glScalef(500, 50, 1);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix(); //Left Wall
        glTranslatef(-100, 25, -60);
        glScalef(1, 50, 320);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix(); //Right Wall
        glTranslatef(400, 25, -60);
        glScalef(1, 50, 320);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix(); //Front Right Wall
        glTranslatef(0, 25, 100);
        glScalef(200, 50, 1);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix(); //Front Left Wall
        glTranslatef(300, 25, 100);
        glScalef(200, 50, 1);
        glutSolidCube(1);
    glPopMatrix();

    glPushMatrix(); //Ceiling
        glTranslatef(150, 49.5, -60);
        glScalef(500, 1, 320);
        glutSolidCube(1);
    glPopMatrix();

    drawExhibitBoundaries();
}

//-------------------------------------------------------------------
void drawStar(int shadow) {
    if (shadow != 1) glColor4f(0, 0, 1, 1.0);
    glutSolidSphere(1, 50, 50);

    if (shadow != 1) glColor4f(1, 0, 0, 1.0);
    glutSolidCone(1, 2, 10, 10);

    if (shadow != 1) glColor4f(0, 1, 0, 1.0);
    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    glutSolidCone(1, 2, 10, 10);
    glPopMatrix();

    if (shadow != 1) glColor4f(1, 1, 0, 1.0);
    glPushMatrix();
    glRotatef(-90, 0, 1, 0);
    glutSolidCone(1, 2, 10, 10);
    glPopMatrix();

    if (shadow != 1) glColor4f(0, 1, 1, 1.0);
    glPushMatrix();
    glRotatef(180, 0, 1, 0);
    glutSolidCone(1, 2, 10, 10);
    glPopMatrix();

    if (shadow != 1) glColor4f(1, 0, 1, 1.0);
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glutSolidCone(1, 2, 10, 10);
    glPopMatrix();

    if (shadow != 1) glColor4f(0.5, 0.5, 1, 1.0);
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(1, 2, 10, 10);
    glPopMatrix();
}

//-------------------------------------------------------------------
void drawHelicopterSide() {
    const int n = 30;

    float v1[] = { 2.5, 5, 0 };
    float v2[] = { 2.5, 0, 5 };
    float v3[] = { 2.5, 0, 15 };
    float v4[] = { 2.5, 2.5, 17.5 };
    float v5[] = { 2.5, 5, 25 };
    float v6[] = { 2.5, 7.5, 17.5 };
    float v7[] = { 2.5, 10, 15 };
    float v8[] = { 2.5, 10, 5 };
    float v9[] = { 7.5, 5, 5 };
    float v10[] = { 7.5, 5, 15 };

    float v11[] = { -2.5, 5, 0 };
    float v12[] = { -2.5, 0, 5 };
    float v13[] = { -2.5, 0, 15 };
    float v14[] = { -2.5, 2.5, 17.5 };
    float v15[] = { -2.5, 5, 25 };
    float v16[] = { -2.5, 7.5, 17.5 };
    float v17[] = { -2.5, 10, 15 };
    float v18[] = { -2.5, 10, 5 };
    float v19[] = { -7.5, 5, 5 };
    float v20[] = { -7.5, 5, 15 };

    float* vertices[] = { v1, v2, v9, v1, v9, v8, v2, v3, v9, v3, v10, v9, v3, v4, v10, v4, v5, v10, v5, v6, v10, v6, v7, v10, v7, v9, v10, v7, v8, v9 };

    glColor4f(0, 0.5, 0.5, 1.0);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < n; i += 3) {
        glVertex3f(vertices[i][0], vertices[i][1], vertices[i][2]);
        glVertex3f(vertices[i + 1][0], vertices[i + 1][1], vertices[i + 1][2]);
        glVertex3f(vertices[i + 2][0], vertices[i + 2][1], vertices[i + 2][2]);
    }
    glEnd();
}

//-------------------------------------------------------------------
void drawHelicopterTop() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[0]);
    const int n = 16;

    float v1[] = { 2.5, 5, 0 };
    float v2[] = { 2.5, 0, 5 };
    float v3[] = { 2.5, 0, 15 };
    float v4[] = { 2.5, 2.5, 17.5 };
    float v5[] = { 2.5, 5, 25 };
    float v6[] = { 2.5, 7.5, 17.5 };
    float v7[] = { 2.5, 10, 15 };
    float v8[] = { 2.5, 10, 5 };
    float v9[] = { 7.5, 5, 5 };
    float v10[] = { 7.5, 5, 15 };

    float v11[] = { -2.5, 5, 0 };
    float v12[] = { -2.5, 0, 5 };
    float v13[] = { -2.5, 0, 15 };
    float v14[] = { -2.5, 2.5, 17.5 };
    float v15[] = { -2.5, 5, 25 };
    float v16[] = { -2.5, 7.5, 17.5 };
    float v17[] = { -2.5, 10, 15 };
    float v18[] = { -2.5, 10, 5 };
    float v19[] = { -7.5, 5, 5 };
    float v20[] = { -7.5, 5, 15 };

    float* vertices[] = { v1, v11, v18, v8, v8, v18, v17, v7, v7, v17, v16, v6, v6, v16, v15, v5 };

    glBegin(GL_QUADS);
    for (int i = 0; i < n; i += 4) {
        glTexCoord2f(0, 0);
        glVertex3f(vertices[i][0], vertices[i][1], vertices[i][2]);
        glTexCoord2f(0, 1);
        glVertex3f(vertices[i + 1][0], vertices[i + 1][1], vertices[i + 1][2]);
        glTexCoord2f(1, 1);
        glVertex3f(vertices[i + 2][0], vertices[i + 2][1], vertices[i + 2][2]);
        glTexCoord2f(1, 0);
        glVertex3f(vertices[i + 3][0], vertices[i + 3][1], vertices[i + 3][2]);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

//-------------------------------------------------------------------
void drawHelicopterPropeller() {
    const int n = 4;

    glColor4f(0, 0, 0, 1.0);
    glBegin(GL_TRIANGLES);
    glVertex3f(0, 10, 0);
    glVertex3f(5, 10, -15);
    glVertex3f(0, 10, -15);

    glVertex3f(0, 10.5, 0);
    glVertex3f(5, 10.5, -15);
    glVertex3f(0, 10.5, -15);
    glEnd();

    float vx[] = { 0, 5, 0, 0 }; float wx[] = { 0, 5, 0, 0 };
    float vy[] = { 10, 10, 10, 10 }; float wy[] = { 10.5, 10.5, 10.5, 10.5 };
    float vz[] = { 0, -15, -15, 0 }; float wz[] = { 0, -15, -15, 0 };
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i < n; i++) {
        glVertex3f(vx[i], vy[i], vz[i]);
        if (i > 0) {
            normal(wx[i - 1], wy[i - 1], wz[i - 1],
                vx[i], vy[i], vz[i],
                wx[i], wy[i], wz[i]);
        }
        glVertex3f(wx[i], wy[i], wz[i]);
    }
    glEnd();



}

//-------------------------------------------------------------------
void drawHelicopterPropellers() {
    glPushMatrix();
        glTranslatef(0, 0, 10);
        glRotatef(90, 0, 1, 0);
        drawHelicopterPropeller();
        glRotatef(90, 0, 1, 0);
        drawHelicopterPropeller();
        glRotatef(90, 0, 1, 0);
        drawHelicopterPropeller();
        glRotatef(90, 0, 1, 0);
        drawHelicopterPropeller();
    glPopMatrix();

}

//-------------------------------------------------------------------
void drawHelicopter() {
    drawHelicopterSide();
    drawHelicopterTop();

    glPushMatrix();
        glScalef(-1., 1., 1);
        drawHelicopterSide();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, 10, 0);
        glScalef(1., -1., 1);
        drawHelicopterTop();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0, heliHgt, 10);
        glRotatef(propellerAngle, 0, 1, 0);
        glTranslatef(0, -heliHgt, -10);
        drawHelicopterPropellers();
    glPopMatrix();

}

//-------------------------------------------------------------------
void drawCannon() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[1]);

    float vx[N], vy[N], vz[N];
    float wx[N], wy[N], wz[N];

    for (int i = 0; i < N; i++)		//Initialize data everytime
    {
        vx[i] = vx_init[i];
        vy[i] = vy_init[i];
        vz[i] = vz_init[i];
    }

    //Repeats for whole circle 360 degs
    for (int j = 0; j < 36; j++) {
        //Get the new set of points rotated by 10 degs (0.175 rads)
        for (int i = 0; i < N; i++) {
            wx[i] = (vx[i] * cos(0.175)) + (vz[i] * sin(0.175));
            wy[i] = vy[i];
            wz[i] = (vz[i] * cos(0.175)) - (vx[i] * sin(0.175));
        }

        glColor4f(1, 1, 1, 1);
        //Creates triangle strip
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i < N; i++) {
            if (i > 0) {
                normal(vx[i - 1], vy[i - 1], vz[i - 1],
                    wx[i], wy[i], wz[i],
                    vx[i], vy[i], vz[i]);
            }
            glTexCoord2f((float)j / 37, (float)i / (N - 1));
            glVertex3f(vx[i], vy[i], vz[i]);

            if (i > 0) {
                normal(wx[i - 1], wy[i - 1], wz[i - 1],
                    wx[i], wy[i], wz[i],
                    vx[i], vy[i], vz[i]);
            }
            glTexCoord2f((float)(j + 1) / 37, (float)i / (N - 1));
            glVertex3f(wx[i], wy[i], wz[i]);
        }
        glEnd();

        //Copys W array to V for next iteration
        for (int k = 0; k < N; k++) {
            vx[k] = wx[k];
            vy[k] = wy[k];
            vz[k] = wz[k];
        }
    }
    glDisable(GL_TEXTURE_2D);
}


//-------------------------------------------------------------------
void display(void)
{
    float lgt_pos[] = {210.0f, 48.0f, 80.0f, 1.0f};  //light0 position
    float lgt1_pos[] = { 0.0f, 5.0f, 0.0f, 1.0f }; //Spotlight (light1) position
    float lgt1_dir[] = { 0, -1, -1 }; // spotlight dir
    float shadowMat[16] = { lgt_pos[1],0,0,0, -lgt_pos[0],0,-lgt_pos[2],-1, 0,0,lgt_pos[1],0, 0,0,0,lgt_pos[1] };

    glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye_x, eye_y, eye_z,  look_x, 10, look_z, 0, 1, 0);

    glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position

    glDisable(GL_TEXTURE_2D);

    floor();

    drawMuseum();
    
    glDisable(GL_LIGHTING);
    glPushMatrix(); //Draw Shadow Object
        glMultMatrixf(shadowMat);
        glTranslatef(300, 10, 0);
        glScalef(5, 5, 5);
        glColor4f(0.2, 0.2, 0.2, 1.0);
        drawStar(1);
    glPopMatrix();

    glEnable(GL_LIGHTING);
    glPushMatrix(); //Draw Actual Object
        glTranslatef(300, 10, 0);
        glScalef(5, 5, 5);
        glColor4f(1, 0.2, 0.2, 1.0);
        drawStar(0);
    glPopMatrix();

    glPushMatrix();
        glRotatef(heliAngle, 0, 1, 0);
        glTranslatef(30, heliHgt, -10);
        drawHelicopter();
        glLightfv(GL_LIGHT1, GL_POSITION, lgt1_pos); //light1 position
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lgt1_dir);  //light1 direction
    glPopMatrix();

    glPushMatrix();
        glTranslatef(125, 1.5, -150);
        glPushMatrix();
            glTranslatef(0, 15, 0);
            glRotatef(-45, 0, 0, 1);
            glTranslatef(0, -15, 0);
            drawCannon();
        glPopMatrix();

        glColor4f(0, 0.5, 1, 1);
        glPushMatrix();
            glTranslatef(ballVal, 24 - 0.01 * (pow(ballVal - 30, 2)), 0);
            glutSolidSphere(3, 20, 20);
        glPopMatrix();

        glColor4f(1, 0, 0, 1);
        glPushMatrix();
            glTranslatef(82, 0, 0);
            glScalef(5, 5, 5);
            glRotatef(-25, 0, 0, 1);
            glutSolidTetrahedron();
        glPopMatrix();
    glPopMatrix();

    drawSkybox();

    glFlush();
}

//Add a timer to move heli -------
void moveHeli(int value)
{
    propellerAngle += 10;
    if (propellerAngle == 360) {
        propellerAngle = 0;
    }

    if (value == 0) {
        heliHgt += 0.5;
        if (heliHgt == 20) {
            value = 1;
        }
        else {
            value = 0;
        }
    }


    else if (value == 1) {
        heliAngle++;
        if (heliAngle == 360) {
            heliAngle = 0;
            value = 2;
        }
        else {
            value = 1;
        }


    }
    else if (value == 2) {
        heliHgt -= 0.5;
        if (heliHgt == 0) {
            value = 0;
        }
        else {
            value = 2;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(20, moveHeli, value);
}

//Add a timer to fire ball -------
void fireBall(int value)
{
    if (ballVal < 79 && value == 0) {
        ballVal += 1;
        value = 0;
        glutPostRedisplay();
        glutTimerFunc(10, fireBall, value);
    }
    else if (ballVal > 0) {
        ballVal -= 1;
        value = 1;
        glutPostRedisplay();
        glutTimerFunc(10, fireBall, value);
    }
}

//--------------------------------------------------------------------------------
int checkCollision(){
    int collision = 0;
    //Front Walls
    if ((eye_z < 123 && eye_z > 77) && ((eye_x > -123 && eye_x < 123) ||  (eye_x > 177 && eye_x < 423))) {
        collision = 1;
    }
    //1st 2 exhibits
    else if ((eye_z < 100 && eye_z > -90) && ((eye_x > -123 && eye_x < 80) || (eye_x > 220 && eye_x < 423))) {
        collision = 1;
    } 
    //last exhibit
    else if ((eye_z < -90 && eye_z > -243) && (eye_x > -123 && eye_x < 423)) {
        collision = 1;
    }
    //back skybox wall
    else if (eye_z > 237) {
        collision = 1;
    }
    //right skybox wall
    else if (eye_x > 447) {
        collision = 1;
    }
    //front skybox wall
    else if (eye_z < -357) {
        collision = 1;
    }
    //left skybox wall
    else if (eye_x < -147) {
        collision = 1;
    }
    //floor
    else if (eye_y < 6) {
        collision = 1;
    }
    //ceiling
    else if (eye_y > 50) {
        collision = 1;
    }


    return collision;
}

//--------------------------------------------------------------------------------
void special(int key, int x, int y)
{
    float prev_eye_x = eye_x;
    float prev_eye_z = eye_z;

    if(key == GLUT_KEY_LEFT) angle -= 0.1;  //Change direction
    else if(key == GLUT_KEY_RIGHT) angle += 0.1;
    else if(key == GLUT_KEY_DOWN)
    {  //Move backward
        eye_x -= 2 * sin(angle);
        eye_z += 2 * cos(angle);
    }
    else if(key == GLUT_KEY_UP)
    { //Move forward
        eye_x += 2 * sin(angle);
        eye_z -= 2 * cos(angle);
    }

    if (checkCollision() == 1) {
        eye_x = prev_eye_x;
        eye_z = prev_eye_z;
    }

    look_x = eye_x + 100*sin(angle);
    look_z = eye_z - 100*cos(angle);
    glutPostRedisplay();
}

//-------------------------------------------------------------------
void keyboard(unsigned char key, int x, int y)
{
    float prev_eye_y = eye_y;

    if (key == 'w' || key == 'W') eye_y += 0.7;
    if (key == 's' || key == 'S') eye_y -= 0.7;

    if (checkCollision() == 1) {
        eye_y = prev_eye_y;
    }

    look_x = eye_x + 100 * sin(angle);
    look_z = eye_z - 100 * cos(angle);

    if (key == ' ' && ballVal == 0) glutTimerFunc(10, fireBall, 0);

    glutPostRedisplay();
}

//-------------------------------------------------------------------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE| GLUT_DEPTH);
   glutInitWindowSize (1000, 1000);
   glutInitWindowPosition (300, 0);
   glutCreateWindow ("Museum");
   initialise();
   glutDisplayFunc(display);
   glutSpecialFunc(special);
   glutKeyboardFunc(keyboard);
   glutTimerFunc(20, moveHeli, 0);
   glutMainLoop();
   return 0;
}
