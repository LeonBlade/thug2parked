#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "gl.h"
#include "texture.h"
#include "draw.h"
#include "font.h"
#include "t2cap.h"

#define WIDTH 1280
#define HEIGHT 720

struct GLCamera
{
	float x;
	float y;
	float z;
	float lx;
	float ly;
	float lz;
};

GLCamera camera = 
{ 
	32.0f, 10.0f, 32.0f, 
	0.0f, 0.0f, -1.0f
};

float yaw = 0.0f;
float pitch = 0.0f;
float dX = 0.0f, 
	  dY = 0.0f, 
	  dZ = 0.0f, 
	  dAX = 0.0f, 
	  dAY = 0.0f;

T2CAP_GROUND *ground;
Texture ground_texture;
Texture font;

void draw_ground(float x, float z, int8_t h)
{
	glPushMatrix();

	float y = 0.5;
	glTranslatef(x, (float)h/2, z);

	glBindTexture(GL_TEXTURE_2D, ground_texture.texture);
	glBegin(GL_QUADS);		// Draw The Cube Using quads
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glNormal3d(0, -1, 0);
	    glColor4f(1, 1, 1, 1);
	    glVertex3f(0, y, 0); glTexCoord2f(0, 0);	
	    glVertex3f(1, y, 0); glTexCoord2f(1.0f, 0);
	    glVertex3f(1, y, 1); glTexCoord2f(1.0f, 1.0f);
	    glVertex3f(0, y, 1); glTexCoord2f(0, 1.0f);
	    

	    glColor4f(0.5, 0.5, 0.5, 1.0f);
    	glNormal3d(0, 0, -1);	
	    glVertex3f(0, 0, 0);
	    glVertex3f(1, 0, 0);
	    glVertex3f(1, 0, 0);
	    glVertex3f(0, 0, 0);

	    glNormal3d(0, 0, -1);	
	    glVertex3f(0, 0, 0);	// Top Right Of The Quad (Front)
	    glVertex3f(1, 0, 0);	// Top Left Of The Quad (Front)
	    glVertex3f(1, y, 0);	// Bottom Left Of The Quad (Front)
	    glVertex3f(0, y, 0);	// Bottom Right Of The Quad (Front)

	    glNormal3d(0, 0, 1);
	    glVertex3f(0, 0, 1);	// Top Right Of The Quad (Back)
	    glVertex3f(1, 0, 1);	// Top Left Of The Quad (Back)
	    glVertex3f(1, y, 1);	// Bottom Left Of The Quad (Back)
	    glVertex3f(0, y, 1);	// Bottom Right Of The Quad (Back)

	    glNormal3d(1, 0, 0);
	    glVertex3f(1, y, 0);	// Top Right Of The Quad (Left)
	    glVertex3f(1, y, 1);	// Top Left Of The Quad (Left)
	    glVertex3f(1, 0, 1);	// Bottom Left Of The Quad (Left)
	    glVertex3f(1, 0, 0);	// Bottom Right Of The Quad (Left)

	    glNormal3d(-1, 0, 0);
	    glVertex3f(0, y, 1);	// Top Right Of The Quad (Right)
	    glVertex3f(0, y, 0);	// Top Left Of The Quad (Right)
	    glVertex3f(0, 0, 0);	// Bottom Left Of The Quad (Right)
	    glVertex3f(0, 0, 1);	// Bottom Right Of The Quad (Right)
	    glEnable(GL_TEXTURE_2D);
	    glEnable(GL_BLEND);
	glEnd();

	glPopMatrix();
}

void draw_cube(float x, float y, float z, float w = 1, float h = 1, float d = 1)
{
	glPushMatrix();
	glTranslatef(x, y, z);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glBegin(GL_QUADS);		// Draw The Cube Using quads
	{
	    glNormal3d(0, -1, 0);
	    glVertex3f(0, 0, d);	// Top Right Of The Quad (Top)
	    glVertex3f(w, 0, d);	// Top Left Of The Quad (Top)
	    glVertex3f(w, 0, 0);	// Bottom Left Of The Quad (Top)
	    glVertex3f(0, 0, 0);	// Bottom Right Of The Quad (Top)

	    glNormal3d(0, 1, 0);
	    glVertex3f(0, h, 0);	// Top Right Of The Quad (Bottom)
	    glVertex3f(w, h, 0);	// Top Left Of The Quad (Bottom)
	    glVertex3f(w, h, d);	// Bottom Left Of The Quad (Bottom)
	    glVertex3f(0, h, d);	// Bottom Right Of The Quad (Bottom)

	    glNormal3d(0, 0, -1);	
	    glVertex3f(0, 0, 0);	// Top Right Of The Quad (Front)
	    glVertex3f(w, 0, 0);	// Top Left Of The Quad (Front)
	    glVertex3f(w, h, 0);	// Bottom Left Of The Quad (Front)
	    glVertex3f(0, h, 0);	// Bottom Right Of The Quad (Front)

	    glNormal3d(0, 0, 1);
	    glVertex3f(0, h, d);	// Top Right Of The Quad (Back)
	    glVertex3f(w, h, d);	// Top Left Of The Quad (Back)
	    glVertex3f(w, 0, d);	// Bottom Left Of The Quad (Back)
	    glVertex3f(0, 0, d);	// Bottom Right Of The Quad (Back)

	    glNormal3d(-1, 0, 0);
	    glVertex3f(w, 0, 0);	// Top Right Of The Quad (Left)
	    glVertex3f(w, 0, d);	// Top Left Of The Quad (Left)
	    glVertex3f(w, h, d);	// Bottom Left Of The Quad (Left)
	    glVertex3f(w, h, 0);	// Bottom Right Of The Quad (Left)

	    glNormal3d(1, 0, 0);
	    glVertex3f(0, 0, d);	// Top Right Of The Quad (Right)
	    glVertex3f(0, 0, 0);	// Top Left Of The Quad (Right)
	    glVertex3f(0, h, 0);	// Bottom Left Of The Quad (Right)
	    glVertex3f(0, h, d);	// Bottom Right Of The Quad (Right)
	}
	glEnd();

	glPopMatrix();
}

void hud()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, font.texture);
	glBegin(GL_QUADS);
	glColor4f(1, 1, 1, 1);
    glVertex2f(0, 0); glTexCoord2f(0, 0);	
    glVertex2f(0, 32); glTexCoord2f(1, 0);
    glVertex2f(32, 32); glTexCoord2f(1, 1);
    glVertex2f(32, 0); glTexCoord2f(0, 1);
    glEnd();
	// draw_string("Hello, world.", 10, 10);
	//Quad *chars = get_chars();
	//draw_quad(chars[(int)'A'], 0, 0, 0);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void draw()
{
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (dZ)
	{
		camera.x += -dZ * camera.lx * 0.1f;
		camera.z += -dZ * camera.lz * 0.1f;
	}
	if (dX)
	{
		float _lx = -cos(yaw);
		float _lz = -sin(yaw);
		camera.x += -dX * _lx * 0.1f;
		camera.z += -dX * _lz * 0.1f;
	}
	if (dY)
	{
		camera.y += dY * 0.1f;
	}
	if (dAX)
	{
		yaw += dAX;
		camera.lx = sin(yaw);
		camera.lz = -cos(yaw);
	}
	if (dAY)
	{
		camera.ly += dAY;
	}
	
	hud();

	// set camera
	gluLookAt(
		camera.x, camera.y, camera.z,
		camera.x + camera.lx, camera.y + camera.ly, camera.z + camera.lz,
		0.0f, 1.0f, 0.0f
	);

	// set light position shit
	GLfloat lpos[] = { 28, 30, 28, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	for (int x = 0; x < 56; x++)
		for (int z = 0; z < 0x39; z++)
				draw_ground(x, z, ground[x].column[z]);

	// swap buffers
	glutSwapBuffers();
}

void keys(unsigned char key, int x, int y)
{
	if (key == 27) // escape
		exit(0);
	switch (key)
	{
		case 'w':
			dZ = -0.5f;	break;
		case 'a':
			dX = -0.5f; break;
		case 's':
			dZ = 0.5f; break;
		case 'd':
			dX = 0.5f; break;
		case 'q':
			dY = -0.5f; break;
		case 'e':
			dY = 0.5f; break;
	}
}

void ukey(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w':
		case 's':
			dZ = 0.0f;break;
		case 'a':
		case 'd':
			dX = 0.0f; break;
		case 'q':
		case 'e':
			dY = 0.0f; break;
	}
}

void skey(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:		dAX = -0.01f; break;
		case GLUT_KEY_RIGHT: 	dAX = 0.01f; break;
		case GLUT_KEY_UP:		dAY = 0.01f; break;
		case GLUT_KEY_DOWN:		dAY = -0.01f; break;
	}
}

void uskey(int key, int x, int y)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:
		case GLUT_KEY_RIGHT: dAX = 0.0f; break;
		case GLUT_KEY_UP:	
		case GLUT_KEY_DOWN:	dAY = 0.0f; break;
	}
}

void resize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

        // Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

int time_interval = 0;
long current_time = 0;
long previous_time = 0;
int frame_count = 0;
int fps = 0;

void idle()
{
	frame_count++;
	current_time = glutGet(GLUT_ELAPSED_TIME);
	int time_interval = current_time - previous_time;
	if (time_interval > 1000)
	{
		fps = frame_count / (time_interval / 1000.0f);
		printf("FPS: %i\n", fps);
		previous_time = current_time;
		frame_count = 0;
	}

	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	// initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("THUG2 Park Editor");

	// register callbacks
	glutDisplayFunc(draw);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);
	glutKeyboardFunc(keys);
	glutKeyboardUpFunc(ukey);
	glutSpecialFunc(skey);
	glutSpecialUpFunc(uskey);

	// other stuff
	glutIgnoreKeyRepeat(1);

	// enable stuff
	glClearDepth(1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_COLOR_MATERIAL);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat white[] = { 0.5f, 0.5f, 0.5f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, black);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	GLfloat shine[] = { 10 };
	glMaterialfv(GL_FRONT, GL_SHININESS, shine);

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);

	// test load CAP
	if (argc > 1)
		ground = load_cap(argv[1]);
	else
		ground = load_cap("parks/A.PRK");

	// init font
	init_font();

	// load ground texture
	load_texture(ground_texture, "textures/ground.png");
	load_texture(font, "textures/font.png");

	glClearColor(0.0,0.0,0.0,0.0);

	// main GLUT loop
	glutMainLoop();

	return 1;
}
