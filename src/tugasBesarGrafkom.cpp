#include <iostream>
//#include <stdio.h>
//#include <cmath>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif

static GLUquadricObj *quadObj = gluNewQuadric();
GLdouble eyeX = 0.0, eyeY = 0.0, eyeZ = 10.0, centerX = 0.0, centerY = 0.0, centerZ = 0.0, upX = 0.0, upY = 1.0, upZ = 0.0;
GLfloat sudut=0;

BOOL	light;												// Lighting ON / OFF
BOOL	lp;													// L Pressed?
BOOL	fp;													// F Pressed?

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };			// Ambient Light Values ( NEW )
GLfloat LightDiffuse[]= { 0.5f, 0.5f, 0.5f, 1.0f };			// Diffuse Light Values ( NEW )
GLfloat LightSpecular[]= { 1.0f, 1.0f, 1.0f, 1.0f };		// Diffuse Light Values ( NEW )
GLfloat LightPosition[]= { 0.0f, 20.0f, 0.0f, 1.0f };		// Light Position ( NEW )

GLfloat LightAmbient2[]= { 0.2f, 0.2f, 0.2f, 1.0f };			// Ambient Light Values ( NEW )
GLfloat LightDiffuse2[]= { 0.8f, 0.8f, 0.8f, 1.0f };			// Diffuse Light Values ( NEW )
GLfloat LightSpecular2[]= { 1.0f, 0.0f, 0.0f, 1.0f };		// Diffuse Light Values ( NEW )
GLfloat LightPosition2[]= { 1.0f, 100.0f, 5.0f, 1.0f };		// Light Position ( NEW )

GLuint  filter;												// Which Filter To Use


struct Image {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};
typedef struct Image Image;
//unsigned int skybox[6];
unsigned int texture[6];
enum {
	TEX_BAWAH=0,
	TEX_ATAS,
	TEX_ANTENA,
	TEX_OBSERVATORY,
	TEX_JARING,
	TEX_TANAH
}; 																//constants for the skybox faces, so we don't have to remember so much number
																//the ids for the textures
typedef struct
{    float x;
    float y;
}point2D_t;

typedef struct
{
    float r,g,b;
} color_t;

															//mengambil gambar BMP
int ImageLoad(char *filename, Image *image) {
	FILE *file;
	unsigned long size;											// ukuran image dalam bytes
	unsigned long i;											// standard counter.
	unsigned short int plane;									// number of planes in image

	unsigned short int bpp;										// jumlah bits per pixel
	char temp;													// temporary color storage for var warna sementara untuk memastikan filenya ada


	if ((file = fopen(filename, "rb")) == NULL) {
		printf("File Not Found : %s\n", filename);
		return 0;
	}
																// mencari file header bmp
	fseek(file, 18, SEEK_CUR);
																// read the width
	if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
		printf("Error reading width from %s.\n", filename);
		return 0;
	}
																//printf("Width of %s: %lu\n", filename, image->sizeX);
																// membaca nilai height
	if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
		printf("Error reading height from %s.\n", filename);
		return 0;
	}
	//printf("Height of %s: %lu\n", filename, image->sizeY);
																//menghitung ukuran image(asumsi 24 bits or 3 bytes per pixel).

	size = image->sizeX * image->sizeY * 3;
																// read the planes
	if ((fread(&plane, 2, 1, file)) != 1) {
		printf("Error reading planes from %s.\n", filename);
		return 0;
	}
	if (plane != 1) {
		printf("Planes from %s is not 1: %u\n", filename, plane);
		return 0;
	}
																// read the bitsperpixel
	if ((i = fread(&bpp, 2, 1, file)) != 1) {
		printf("Error reading bpp from %s.\n", filename);

		return 0;
	}
	if (bpp != 24) {
		printf("Bpp from %s is not 24: %u\n", filename, bpp);
		return 0;
	}
																// seek past the rest of the bitmap header.
	fseek(file, 24, SEEK_CUR);
																// read the data.
	image->data = (char *) malloc(size);
	if (image->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		return 0;
	}
	if ((i = fread(image->data, size, 1, file)) != 1) {
		printf("Error reading image data from %s.\n", filename);
		return 0;
	}
	for (i = 0; i < size; i += 3) {								// membalikan semuan nilai warna (gbr - > rgb)
		temp = image->data[i];
		image->data[i] = image->data[i + 2];
		image->data[i + 2] = temp;
	}
	// we're done.
	return 1;
}

//mengambil tekstur
Image * loadTexture() {
	Image *image1;
	// alokasi memmory untuk tekstur
	image1 = (Image *) malloc(sizeof(Image));
	if (image1 == NULL) {
		printf("Error allocating space for image");
		exit(0);
	}
	if (!ImageLoad("bawah.bmp", image1)) {
		exit(1);

	}

	return image1;
}
//mengambil tekstur
Image * loadTextureDua(char filename[]) {
	Image *image2;
	// alokasi memmory untuk tekstur
	image2 = (Image *) malloc(sizeof(Image));
	if (image2 == NULL) {
		printf("Error allocating space for image");
		exit(0);
	}

	if(filename == "left"){
		if (!ImageLoad("left.bmp", image2)) {
			exit(1);
		}
	}else if(filename == "right"){
		if (!ImageLoad("right.bmp", image2)) {
			exit(1);
		}
	}else if(filename == "top"){
		if (!ImageLoad("top.bmp", image2)) {
			exit(1);
		}
	}else if(filename == "bottom"){
		if (!ImageLoad("bottom.bmp", image2)) {
			exit(1);
		}
	}else if(filename == "front"){
		if (!ImageLoad("front.bmp", image2)) {
			exit(1);
		}
	}else if(filename == "back"){
		if (!ImageLoad("back.bmp", image2)) {
			exit(1);
		}
	}else if(filename == "antena"){
		if (!ImageLoad("antena4.bmp", image2)) {
			exit(1);
		}
	}else if(filename == "atas"){
		if (!ImageLoad("atas2.bmp", image2)) {
			exit(1);
		}
	}else if(filename == "bawah"){
		if (!ImageLoad("bawah.bmp", image2)) {
			exit(1);
		}
	}else if(filename == "observatory"){
		if (!ImageLoad("observatory.bmp", image2)) {
			exit(1);
		}
	}else if(filename == "jaring"){
		if (!ImageLoad("jaringc.bmp", image2)) {
			exit(1);
		}
	}else if(filename == "tanah"){
		if (!ImageLoad("wood.bmp", image2)) {
			exit(1);
		}
	}

	return image2;
}



//load all of the textures, to the skybox array
void initskybox(){

	Image *image1 = loadTextureDua("bawah");
	Image *image2 = loadTextureDua("antena");
	Image *image3 = loadTextureDua("atas");
	Image *image4 = loadTextureDua("observatory");
	Image *image5 = loadTextureDua("jaring");
	Image *image6 = loadTextureDua("tanah");

	if (image1 == NULL) {
		printf("Image was not returned from loadTexture\n");
		exit(0);
	}

	glGenTextures(6, texture);

	glBindTexture(GL_TEXTURE_2D, texture[TEX_BAWAH]);					//binding texture untuk membuat texture 2D
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //menyesuaikan ukuran textur ketika image lebih besar dari texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //menyesuaikan ukuran textur ketika image lebih kecil dari texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);

	glBindTexture(GL_TEXTURE_2D, texture[TEX_ANTENA]);					//binding texture untuk membuat texture 2D
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //menyesuaikan ukuran textur ketika image lebih besar dari texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //menyesuaikan ukuran textur ketika image lebih kecil dari texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image2->sizeX, image2->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image2->data);

	glBindTexture(GL_TEXTURE_2D, texture[TEX_ATAS]);					//binding texture untuk membuat texture 2D
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //menyesuaikan ukuran textur ketika image lebih besar dari texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //menyesuaikan ukuran textur ketika image lebih kecil dari texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image3->sizeX, image3->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image3->data);

	glBindTexture(GL_TEXTURE_2D, texture[TEX_OBSERVATORY]);					//binding texture untuk membuat texture 2D
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //menyesuaikan ukuran textur ketika image lebih besar dari texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //menyesuaikan ukuran textur ketika image lebih kecil dari texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image4->sizeX, image4->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image4->data);

	glBindTexture(GL_TEXTURE_2D, texture[TEX_JARING]);					//binding texture untuk membuat texture 2D
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //menyesuaikan ukuran textur ketika image lebih besar dari texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //menyesuaikan ukuran textur ketika image lebih kecil dari texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image5->sizeX, image5->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image5->data);

	glBindTexture(GL_TEXTURE_2D, texture[TEX_TANAH]);					//binding texture untuk membuat texture 2D
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //menyesuaikan ukuran textur ketika image lebih besar dari texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //menyesuaikan ukuran textur ketika image lebih kecil dari texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image6->sizeX, image6->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image6->data);


	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);				// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);				// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_SPECULAR,LightSpecular);			// Setup The Specular Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);			// Position The Light
	glEnable(GL_LIGHT1);										// Enable Light One

	glLightfv(GL_LIGHT2, GL_AMBIENT, LightAmbient);				// Setup The Ambient Light
	glLightfv(GL_LIGHT2, GL_DIFFUSE, LightDiffuse);				// Setup The Diffuse Light
	glLightfv(GL_LIGHT2, GL_SPECULAR,LightSpecular);			// Setup The Specular Light
	glLightfv(GL_LIGHT2, GL_POSITION,LightPosition);			// Position The Light
	//glEnable(GL_LIGHT2);										// Enable Light One
	glEnable(GL_LIGHTING);
}
void killskybox(){												//delete all of the textures from the skybox array (to avoid memory leaks)

	//glDeleteTextures(6,&skybox[0]);
	glDeleteTextures(3,&texture[TEX_BAWAH]);
}

void iconn(){
	glPushMatrix();
	glColor3f(1,0,0);
	glTranslatef(-2,-5,10);
	glScalef(0.2,0.6,0.1);
	glutSolidCube(10);
	glPopMatrix();
}

void iconn2(){
	glPushMatrix();
	glColor3f(1,0,0);
	glTranslatef(-0.2,-5,10);
	glScalef(0.1,0.4,0.1);
	glRotatef(45,0,0,1);
	glutSolidCube(10);
	glPopMatrix();
}


void drawSkybox(float size){
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
			glPushMatrix();
			glRotatef(270, 1.0, 0.0, 0.0);
			glBindTexture(GL_TEXTURE_2D,texture[TEX_OBSERVATORY]);
			gluSphere(quadObj, size, 30, 30);
			glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}


void plazaBawah(){
	//glDisable(GL_LIGHTING);

    //atap plaza atas
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glRotatef(270, 1, 0, 0);
    gluCylinder(quadObj, 5, 2, 0.05, 50, 1);
    glPopMatrix();

    //atap plaza bawah
    glPushMatrix();
	glTranslatef(0, -2.75, 0);
	glRotatef(90, 1, 0, 0);
	gluCylinder(quadObj, 5, 2, 0.05, 50, 1);
	glPopMatrix();

    glEnable(GL_TEXTURE_2D);

		//plaza atas
		glPushMatrix();
		glTranslatef(0, 0, 0);
		glRotatef(90, 1, 0, 0);
		glBindTexture(GL_TEXTURE_2D,texture[TEX_BAWAH]);
		gluCylinder(quadObj, 5, 5, 2.75, 50, 1);
		glPopMatrix();

		//plaza bawah
		glPushMatrix();
		glTranslatef(0, -2.75, 0);
		glRotatef(90, 1, 0, 0);
		gluCylinder(quadObj, 3, 3, 1.25, 50, 1);
		glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    //tiang sekeliling plaza bawah
    int i;
    for (i=0; i<12; i++) {										//buat 12 tiang
        glPushMatrix();
        glRotatef(i*30, 0, 1, 0);
        glTranslatef(4, -2.75, 0);
        glRotatef(90, 1, 0, 0);
        gluCylinder(quadObj, 0.4, 0.4, 1.25, 50, 1);
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
	//glDisable(GL_LIGHTING);
	//glEnable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(0, 40, 0);
    glRotatef(90, 1, 0, 0);
    gluQuadricTexture(quadObj, GL_TRUE);
    gluCylinder(quadObj, 2, 2, 40.0, 50, 1);
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);

		//bulet jaring-jaring
		glPushMatrix();
		glTranslatef(0, 6, 0);
		glRotatef(270, 1, 0, 0);
		glBindTexture(GL_TEXTURE_2D,texture[TEX_JARING]);
		gluCylinder(quadObj, 4, 2, 0.05, 50, 1);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 9, 0);
		glRotatef(270, 1, 0, 0);
		gluCylinder(quadObj, 4, 2, 0.05, 50, 1);
		glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    //atap atas
    glPushMatrix();
    glTranslatef(0, 40, 0);
    glRotatef(270, 1, 0, 0);
    gluCylinder(quadObj, 2, 0.0, 0.0, 50, 1);
    glPopMatrix();

	//glDisable(GL_TEXTURE_2D);
}

void tempatRestoran(){
	//glDisable(GL_LIGHTING);
    //lantai
    glPushMatrix();
    glTranslatef(0, 30, 0);
    glRotatef(90, 1, 0, 0);
    gluCylinder(quadObj, 5, 2, 1, 50, 1);
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);

		//resto bawah
		glPushMatrix();
		glTranslatef(0, 34, 0);
		glRotatef(90, 1, 0, 0);
		glBindTexture(GL_TEXTURE_2D,texture[TEX_ATAS]);
		gluCylinder(quadObj, 5, 5, 4, 50, 1);
		glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    //atap resto bawah
    glPushMatrix();
    glTranslatef(0, 34, 0);
    glRotatef(270, 1, 0, 0);
    gluCylinder(quadObj, 5, 2, 0.0, 50, 1);
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);

		//resto atas
		glPushMatrix();
		glTranslatef(0, 38, 0);
		glRotatef(90, 1, 0, 0);
		gluCylinder(quadObj, 4, 4, 4, 50, 1);
		glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    //atap resto atas
    glPushMatrix();
    glTranslatef(0, 38, 0);
    glRotatef(270, 1, 0, 0);
    gluCylinder(quadObj, 4, 2, 0.0, 50, 1);
    glPopMatrix();
}

void antenaMenara(){
	//glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D,texture[TEX_ANTENA]);
		glPushMatrix();
		glTranslatef(0, 40, 0);
		glRotatef(270, 1, 0, 0);
		gluCylinder(quadObj, 1.2, 1.2, 10, 4, 1);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 50, 0);
		glRotatef(270, 1, 0, 0);
		gluCylinder(quadObj, 0.8, 0.8, 10, 4, 1);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 60, 0);
		glRotatef(270, 1, 0, 0);
		gluCylinder(quadObj, 0.4, 0.4, 10, 4, 1);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 70, 0);
		glRotatef(270, 1, 0, 0);
		gluCylinder(quadObj, 0.2, 0.2, 10, 4, 1);
		glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void init(void){												// Inisialisasi OpenGL
	glShadeModel(GL_SMOOTH);									// Enables Smooth Shading
	glClearColor(0.0, 0.0, 0.0, 0.0);							// Black Background
	glClearDepth(1.0f);											// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);									// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);										// The Type Of Depth Test To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// Really Nice Perspective Calculations

	initskybox();

}


void display(void){												// Tempat "menggambar"
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// Clear The Screen And The Depth Buffer
	glLoadIdentity();											// Reset The Current Modelview Matrix
	//glDisable(GL_DEPTH_TEST);

	glLoadIdentity();
	gluLookAt (eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);


	/*
	//Set ambient, diffuse & specular lighting
	glMaterialfv(GL_FRONT, GL_AMBIENT, qaGray);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaGray);
	glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 60.0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaLowAmbient);*/

	//buat objek 3D disini
	glPushMatrix();
	glRotatef(sudut, 0.0, 1.0, 0.0);
	drawSkybox(70.0);

	glPushMatrix();
	glTranslatef(0, -1.1, 0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture[TEX_TANAH]);
	glBegin(GL_QUADS);
	glTexCoord2f(1,1);
	glVertex3f(-20.0f, -0.41f, 20.0f);
	glTexCoord2f(0,1);
	glVertex3f(20.0f, -0.41f, 20.0f);
	glTexCoord2f(0,0);
	glVertex3f(20.0f, -0.41f, -20.0f);
	glTexCoord2f(1,0);
	glVertex3f(-20.0f, -0.41f, -20.0f);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);


	//glTranslatef(0, (GLfloat)geser-3, 0);
	glScalef(0.2, 0.2, 0.2);
	bangunanBawah();
	plazaBawah();
	tempatRestoran();
	badanMenara();
	antenaMenara();
	//iconn();
	//iconn2();
	glPopMatrix();

	glutSwapBuffers();


}
void reshape(int width, int height){
	if (height==0){												// Prevent A Divide By Zero By
		height=1;												// Making Height Equal One
	}
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);                        		// Select The Projection Matrix
	glLoadIdentity();                           				// Reset The Projection Matrix
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,200.0f); // Calculate The Aspect Ratio Of The Window
	glMatrixMode(GL_MODELVIEW);                     			// Select The Modelview Matrix
	glLoadIdentity();                          			 		// Reset The Modelview Matrix
}
void keyboard(unsigned char key, int x, int y){
	switch (key)
	{
		case 'd':
			if(eyeX<20){
				eyeX = eyeX + 0.2;
				centerX = centerX + 0.2;
			}
			glutPostRedisplay();
			break;
		case 'a':
			if(eyeX>-20){
				eyeX = eyeX - 0.2;
				centerX = centerX - 0.2;
			}
			glutPostRedisplay();
			break;
		case 'w':
			if(eyeY<20){
				eyeY = eyeY + 0.2;
				centerY = centerY + 0.2;
			}
			glutPostRedisplay();
			break;
		case 's':
			if(eyeY>-20){
				eyeY = eyeY - 0.2;
				centerY = centerY - 0.2;
			}
			glutPostRedisplay();
			break;
		case 'x':
			eyeZ = eyeZ + 0.2;
			centerZ = centerZ - 0.2;
			glutPostRedisplay();
			break;
		case 'z':
			eyeZ = eyeZ - 0.2;
			centerZ = centerZ + 0.2;
			glutPostRedisplay();
			break;
		case '-':
			eyeZ = eyeZ + 1;
			glutPostRedisplay();
			break;
		case '+':
			eyeZ = eyeZ - 1;
			glutPostRedisplay();
			break;
		case '4':
			if(sudut < 360){
				sudut = sudut + 10;
			}else if(sudut >= 360){
				sudut = 0;
			}
			glutPostRedisplay();
			break;
		case '6':
			if(sudut > -360){
				sudut = sudut - 10;
			}else if(sudut <= -360){
				sudut = 0;
			}
			glutPostRedisplay();
			break;
		case 27:
			killskybox();
			exit(0);
			break;
	}
}
void specialKeyboard(int key, int x, int y){
	switch (key)
	{
		case GLUT_KEY_RIGHT:
			if(eyeX<20){
				eyeX = eyeX + 1;
			}
			glutPostRedisplay();
			break;
		case GLUT_KEY_LEFT:
			if(eyeX>-20){
				eyeX = eyeX - 1;
			}
			glutPostRedisplay();
			break;
		case GLUT_KEY_UP:
			if(eyeY == -1){
				centerY = 0;
			}
			if(eyeY<20){
				eyeY = eyeY + 0.25;
			}
			glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:
			if(eyeY>-1){
				eyeY = eyeY - 0.25;
			}else if(centerY<20){
				centerY = centerY + 0.25;
			}
			glutPostRedisplay();
			break;
	}
}

int main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);								// Ukuran Window
	glutInitWindowPosition(100, 100);							// Posisi Window
	glutCreateWindow("N Seoul Tower");							// Nama Window
	init();														// Panggil inisialisasi
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	glutMainLoop();
	return 0;
}
