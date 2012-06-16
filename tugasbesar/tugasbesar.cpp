//
//  main.cpp
//  TugasBesar_SeoulTower
//
//  Created by Muhammad Ilham on 6/4/12.
//  Copyright (c) 2012 UNIKOM. All rights reserved.
//
#include <windows.h>
#include <GL\glut.h>
#include <iostream>
#include <stdio.h>
#include <cmath>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#endif

#define checkImageWidth 64 
#define checkImageHeight 64

static GLubyte checkImage[checkImageHeight][checkImageWidth][4]; 
static GLubyte otherImage[checkImageHeight][checkImageWidth][4];

static GLuint texName[2]; 
void makeCheckImages(void) 
{ 
	int i, j, c; 
	for 
		(i = 0; i < checkImageHeight; i++) 
	{
		for (j = 0; j < checkImageWidth; j++) 
		{ 
			c = ((((i&0x8)==0)^((j&0x8))==0))*255; 
			checkImage[i][j][0] = (GLubyte) c; 
			checkImage[i][j][1] = (GLubyte) c; 
			checkImage[i][j][2] = (GLubyte) c; 
			checkImage[i][j][3] = (GLubyte) 255; 
			c = ((((i&0x10)==0)^((j&0x10))==0))*255; 
			otherImage[i][j][0] = (GLubyte) c; 
			otherImage[i][j][1] = (GLubyte) 0; 
			otherImage[i][j][2] = (GLubyte) 0; 
			otherImage[i][j][3] = (GLubyte) 255;
		} 
	} 
}

static int kameraX = 0, kameraY = 0, kameraZ = 10, sudut = 0, geser = 0;
GLfloat qaBlack[]       = {0.0, 0.0, 0.0, 1.0};
GLfloat qaGreen[]       = {0.0, 1.0, 0.0, 1.0};
GLfloat qaRed[]         = {1.0, 0.0, 0.0, 1.0};
GLfloat qaBlue[]        = {0.0, 0.0, 1.0, 1.0};
GLfloat qaWhite[]       = {1.0, 1.0, 1.0, 1.0};
GLfloat qaGray[]        = {0.5, 0.5, 0.5, 1.0};
GLfloat qaLowAmbient[]  = {0.2, 0.2, 0.2, 1.0};
GLfloat qaFullAmbient[] = {1.0, 1.0, 1.0, 1.0};

void init(void)
{
	glClearColor(0.30, 0.30, 0.30, 0.50);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -3.0, 3.0);
    
    //Lighting setup
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    
    //Set intensitas & warna lighting
    GLfloat ambientLight[]      = {0.2, 0.2, 0.2, 1.0};
    GLfloat diffuseLight[]      = {0.8, 0.8, 0.8, 1.0};
    GLfloat specularLight[]     = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    
    //Posisi lighting
    GLfloat lightPosition[]      = {kameraX + 50.0, kameraY + 50.0, kameraZ + 0.0, 0.8};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    
    //glEnable(GL_DEPTH_TEST);

	//coba texturing:
	glShadeModel(GL_FLAT); 
	glEnable(GL_DEPTH_TEST); 
	makeCheckImages(); 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
	glGenTextures(2, texName); 
	glBindTexture(GL_TEXTURE_2D, texName[0]); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, 
		checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage); 
	glBindTexture(GL_TEXTURE_2D, texName[1]); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, 
		checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, otherImage); 
	glEnable(GL_TEXTURE_2D); 
}

void DrawCircle(float cx, float cy, float r, int num_segments) 
{ 
	float theta = 2 * 3.1415926 / float(num_segments); 
	float tangetial_factor = tanf(theta);//calculate the tangential factor 
    
	float radial_factor = cosf(theta);//calculate the radial factor 
	
	float x = r;//we start at angle = 0 
    
	float y = 0;
    
	glBegin(GL_LINE_LOOP); 
	for(int ii = 0; ii < num_segments; ii++) 
	{ 
		glVertex2f(x + cx, y + cy);//output vertex 
        
		//calculate the tangential vector 
		//remember, the radial vector is (x, y) 
		//to get the tangential vector we flip those coordinates and negate one of them 
        
		float tx = -y; 
		float ty = x; 
        
		//add the tangential vector 
        
		x += tx * tangetial_factor; 
		y += ty * tangetial_factor; 
        
		//correct using the radial factor 
        
		x *= radial_factor; 
		y *= radial_factor; 
	} 
	glEnd(); 
}
void plazaBawah(){
    //atap plaza atas
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glRotatef(270, 1, 0, 0);
    gluCylinder(gluNewQuadric(), 5, 2, 0.05, 50, 1);
    glPopMatrix();
    //plaza atas
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glRotatef(90, 1, 0, 0);
    gluCylinder(gluNewQuadric(), 5, 5, 2.75, 50, 1);    
    glPopMatrix();
    //atap plaza bawah
    glPushMatrix();
    glTranslatef(0, -2.75, 0);
    glRotatef(90, 1, 0, 0);
    gluCylinder(gluNewQuadric(), 5, 2, 0.05, 50, 1);
    glPopMatrix();
    //plaza bawah
    glPushMatrix();
    glTranslatef(0, -2.75, 0);
    glRotatef(90, 1, 0, 0);    
    gluCylinder(gluNewQuadric(), 3, 3, 1.25, 50, 1);
    glPopMatrix();
    //tiang sekeliling plaza bawah
    int i;
    for (i=0; i<12; i++) {//buat 12 tiang
        glPushMatrix();
        glRotatef(i*30, 0, 1, 0);
        glTranslatef(4, -2.75, 0);
        glRotatef(90, 1, 0, 0);
        gluCylinder(gluNewQuadric(), 0.4, 0.4, 1.25, 50, 1);
        glPopMatrix();
    }
}
void bangunanBawah(){    
    
    //bangunan bawah
    glPushMatrix();
    glTranslatef(2, -6, 0);
    //glRotatef(90, 1, 0, 0);
    glScalef(5, 1, 3);
    glutSolidCube(4);
    glPopMatrix();
    
}

void badanMenara(){   
    
    
    glPushMatrix();
    glTranslatef(0, 40, 0);
    glRotatef(90, 1, 0, 0);
    gluCylinder(gluNewQuadric(), 2, 2,40, 50, 1);
    glPopMatrix();
    
    //bulet jaring-jaring
    glPushMatrix();
    glTranslatef(0, 6, 0);
    glRotatef(270, 1, 0, 0);
    gluCylinder(gluNewQuadric(), 4, 2, 0.05, 50, 1);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 9, 0);
    glRotatef(270, 1, 0, 0);
    gluCylinder(gluNewQuadric(), 4, 2, 0.05, 50, 1);
    glPopMatrix();
    
    //atap atas
    glPushMatrix();
    glTranslatef(0, 40, 0);
    glRotatef(270, 1, 0, 0);
    gluCylinder(gluNewQuadric(), 2, 0.0, 0.05, 50, 1);
    glPopMatrix();
}

void tempatRestoran(){
    
    //lantai
    glPushMatrix();
    glTranslatef(0, 30, 0);
    glRotatef(90, 1, 0, 0);
    gluCylinder(gluNewQuadric(), 5, 2, 1, 50, 1);
    glPopMatrix();
    //resto bawah
    glPushMatrix();
    glTranslatef(0, 34, 0);
    glRotatef(90, 1, 0, 0);
    gluCylinder(gluNewQuadric(), 5, 5, 4, 50, 1);
    glPopMatrix();
    //atap resto bawah
    glPushMatrix();
    glTranslatef(0, 34, 0);
    glRotatef(270, 1, 0, 0);
    gluCylinder(gluNewQuadric(), 5, 2, 0.05, 50, 1);
    glPopMatrix();
    //resto atas
    glPushMatrix();
    glTranslatef(0, 38, 0);
    glRotatef(90, 1, 0, 0);
    gluCylinder(gluNewQuadric(), 4, 4, 4, 50, 1);
    glPopMatrix();
    //atap resto atas
    glPushMatrix();
    glTranslatef(0, 38, 0);
    glRotatef(270, 1, 0, 0);
    gluCylinder(gluNewQuadric(), 4, 2, 0.05, 50, 1);
    glPopMatrix();
}

void antenaMenara(){
    //atap atas
    glPushMatrix();
    glTranslatef(0, 40, 0);
    glRotatef(270, 1, 0, 0);
    glutSolidCone(1.2, 40, 4, 1);
    glPopMatrix();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    glLoadIdentity();
    gluLookAt ((GLdouble)kameraX, (GLdouble)kameraY, (GLdouble)kameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
    //Set ambient, diffuse & specular lighting
    glMaterialfv(GL_FRONT, GL_AMBIENT, qaGray);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, qaGray);
    glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
    glMaterialf(GL_FRONT, GL_SHININESS, 60.0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, qaLowAmbient);
    
    //buat objek 3D disini
    glPushMatrix();
    glRotatef((GLfloat) sudut, 0.0, 1.0, 0.0);
    glTranslatef(0, (GLfloat)geser-3, 0);
    glScalef(0.2, 0.2, 0.2);
    bangunanBawah();
    plazaBawah();
    tempatRestoran();
    badanMenara();
    antenaMenara();
    glPopMatrix();
    
	glutSwapBuffers();

	//texturing:
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glBindTexture(GL_TEXTURE_2D, texName[0]); 
	glBegin(GL_QUADS); glTexCoord2f(0.0, 0.0); 
	glVertex3f(-2.0, -1.0, 0.0); 
	glTexCoord2f(0.0, 1.0); 
	glVertex3f(-2.0, 1.0, 0.0); 
	glTexCoord2f(1.0, 1.0); 
	glVertex3f(0.0, 1.0, 0.0); 
	glTexCoord2f(1.0, 0.0); 
	glVertex3f(0.0, -1.0, 0.0); 
	glEnd(); 
	glBindTexture(GL_TEXTURE_2D, texName[1]); 
	glBegin(GL_QUADS); glTexCoord2f(0.0, 0.0); 
	glVertex3f(1.0, -1.0, 0.0); 
	glTexCoord2f(0.0, 1.0); 
	glVertex3f(1.0, 1.0, 0.0); 
	glTexCoord2f(1.0, 1.0); 
	glVertex3f(2.41421, 1.0, -1.41421); 
	glTexCoord2f(1.0, 0.0); 
	glVertex3f(2.41421, -1.0, -1.41421); 
	glEnd(); 
	glFlush(); 

}
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
    
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
    {
        case '+'://zoom in
            kameraZ = kameraZ - 1;
			glutPostRedisplay();
            break;
        case '-'://zoom out
            kameraZ = kameraZ + 1;
			glutPostRedisplay();
            break;
        case '1'://default
            kameraX = 0;
            kameraY = 0;
            kameraZ = 10;
            glutPostRedisplay();
            break;
        case '2'://dari kanan
            kameraX = 10;
            kameraY = 0;
            kameraZ = 0;
            glutPostRedisplay();
            break;
        case '3'://dari kanan
            kameraX = -10;
            kameraY = 0;
            kameraZ = 0;
            glutPostRedisplay();
            break;
        case '4'://dari kanan
            kameraX = 0;
            kameraY = 0;
            kameraZ = -10;
            glutPostRedisplay();
            break;
        case 'a':
            sudut = sudut + 5;
            glutPostRedisplay();
            break;
        case 'd':
            sudut = sudut - 5;
            glutPostRedisplay();
            break;
        case 'w':
            geser = geser + 1;
            glutPostRedisplay();
            break;
        case 's':
            geser = geser - 1;
            glutPostRedisplay();
            break;
        
    }
}
void specialKeyboard(int key, int x, int y) {
    switch (key)
    {
        case GLUT_KEY_RIGHT:
            kameraX = kameraX + 1;
			glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT:
            kameraX = kameraX - 1;
			glutPostRedisplay();
            break;
        case GLUT_KEY_UP:
            kameraY = kameraY + 1;
			glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN:
            kameraY = kameraY - 1;
			glutPostRedisplay();
            break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("N Seoul Tower");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutMainLoop();
    return 0;
}
