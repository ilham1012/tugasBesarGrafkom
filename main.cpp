//
//  main.cpp
//  TugasBesar_SeoulTower
//
//  Created by Muhammad Ilham on 6/4/12.
//  Copyright (c) 2012 UNIKOM. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <cmath>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif

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
    glutInitWindowSize(800, 600);
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
