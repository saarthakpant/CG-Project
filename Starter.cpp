#include <math.h>
#include <stdio.h>
#include <gl/glut.h>


#define res			1					//0=160x120 1=340x240 4=640x480
#define SW			160*res				//screen width
#define SH			120*res				//screen height
#define SW2			(SW/2)				//half of screen width
#define SH2			(SH/2)				//half of screen height
#define pixelScale	4/res				//OpenGL pixel scale
#define GLSW		(SW*pixelScale)		//OpenGL window width
#define GLSH		(SH*pixelScale)		//OpenGL window height	
#define numSect		4					//number of sectors  
#define numWall		16					//number of walls

//----------------------------------------

typedef struct
{
	int fr1, fr2;  // frame 1 frame 2, to create constant frame rate
}time; time T;

typedef struct
{
	int w, s, a, d;		//move up, down, left, right
	int sl, sr;			//strafe left, right
	int m;				//look up down
}keys; keys K;

typedef struct
{
	float cos[360];        //Save sin cos in values 0-360 degrees 
	float sin[360];
}math; math M;

typedef struct
{
	int x, y, z;             //player position. Z is up
	int a;                 //player angle of rotation left right
	int l;                 //variable to look up and down
}player; player P;

typedef struct
{
	int x1, y1;             //bottom line point 1
	int x2, y2;             //bottom line point 2
	int c;                 //wall color
}walls; walls W[30];

typedef struct
{
	int ws, we;             //wall number start and end
	int z1, z2;             //height of bottom and top 
	int d;                 //add y distances to sort drawing order
	int c1, c2;             //bottom and top color
	int surf[SW];          //to hold points for surfaces
	int surface;           //is there a surfaces to draw
}sectors; sectors S[30];

//-----------------------------------------------


void pixel(int x, int y, int c)
{
	int rgb[3];
	if (c == 0) { rgb[0] = 255; rgb[1] = 255; rgb[2] = 0; } //Yellow	
	if (c == 1) { rgb[0] = 160; rgb[1] = 160; rgb[2] = 0; } //Yellow darker	
	if (c == 2) { rgb[0] = 0; rgb[1] = 255; rgb[2] = 0; } //Green	
	if (c == 3) { rgb[0] = 0; rgb[1] = 160; rgb[2] = 0; } //Green darker	
	if (c == 4) { rgb[0] = 0; rgb[1] = 255; rgb[2] = 255; } //Cyan	
	if (c == 5) { rgb[0] = 0; rgb[1] = 160; rgb[2] = 160; } //Cyan darker
	if (c == 6) { rgb[0] = 160; rgb[1] = 100; rgb[2] = 0; } //brown	
	if (c == 7) { rgb[0] = 110; rgb[1] = 50; rgb[2] = 0; } //brown darker
	if (c == 8) { rgb[0] = 0; rgb[1] = 60; rgb[2] = 130; } //background if(c==0){rgb[0] }
	glColor3ub(rgb[0], rgb[1], rgb[2]);
	glBegin(GL_POINTS);
	glVertex2i(x * pixelScale + 2, y * pixelScale + 2);
	glEnd();
}


void movePlayer()
{
	if (K.a == 1 && K.m == 0) { printf("left\n"); }
	if (K.d == 1 && K.m == 0) { printf("right\n"); }
	if (K.w == 1 && K.m == 0) { printf("up\n"); }
	if (K.s == 1 && K.m == 0) { printf("down\n"); }

	if (K.sr == 1) { printf("strafe right\n"); }
	if (K.sl == 1) { printf("strafe left\n"); }

	if (K.a == 1 && K.m == 1) { printf("look up\n"); }
	if (K.d == 1 && K.m == 1) { printf("look down\n"); }
	if (K.w == 1 && K.m == 1) { printf("move up\n"); }
	if (K.s == 1 && K.m == 1) { printf("move down\n"); }
}

void clearBackground()
{
	int x, y;
	for (y = 0; y < SH; y++)
	{
		for (x = 0; x < SW; x++) { pixel(x, y, 8); }
	}
}

int tick;
void draw3D()
{
	int x, y, c = 0;
	for (y = 0; y < SH2; y++)
	{
		for (x = 0; x < SW2; x++)
		{
			pixel(x, y, c);
			c += 1; if (c > 8) { c = 0; }
		}
	}
	//frame rate
	tick += 1; if (tick > 20) { tick = 0; } pixel(SW2, SH2 + tick, 0);
}

void display()
{
	int x, y;
	if (T.fr1 - T.fr2 >= 50) {
		clearBackground();
		movePlayer();
		draw3D();


		T.fr2 = T.fr1;
		glutSwapBuffers();
		glutReshapeWindow(GLSW, GLSH);
	}
	T.fr1 = glutGet(GLUT_ELAPSED_TIME);
	glutPostRedisplay();
}


void KeysDown(unsigned char key, int x, int y) {
	if (key == 'w' == 1) { K.w = 1; }
	if (key == 's' == 1) { K.s = 1; }
	if (key == 'a' == 1) { K.a = 1; }
	if (key == 'd' == 1) { K.d = 1; }
	if (key == 'm' == 1) { K.m = 1; }
	if (key == ',' == 1) { K.sr = 1; }
	if (key == '.' == 1) { K.sl = 1; }
}

void KeysUp(unsigned char key, int x, int y) {
	if (key == 'w' == 1) { K.w = 0; }
	if (key == 's' == 1) { K.s = 0; }
	if (key == 'a' == 1) { K.a = 0; }
	if (key == 'd' == 1) { K.d = 0; }
	if (key == 'm' == 1) { K.m = 0; }
	if (key == ',' == 1) { K.sr = 0; }
	if (key == '.' == 1) { K.sl = 0; }
}

void init() {
	int x;
	//store sin/cos in degrees

	for (x = 0; x < 360; x++) {
		M.cos[x] = cos(x / 180.0 * M_PI);

	}
}



int main(int argc,char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(GLSW / 2, GLSH / 2);
	glutInitWindowSize(GLSW, GLSH);
	glutCreateWindow("DOOOMMMM");
	glPointSize(pixelScale);
	gluOrtho2D(0, GLSW, 0, GLSH);
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(KeysDown);
	glutKeyboardUpFunc(KeysUp);
	glutMainLoop();
	return 0;

}