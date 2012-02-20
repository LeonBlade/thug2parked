#include <stdio.h>
#include <stdlib.h>

#include "gl.h"
#include "camera.h"
#include "texture.h"
#include "font.h"
#include "t2cap.h"

#define WIDTH 1280
#define HEIGHT 720

T2CAP_GROUND *t2ground;

Camera camera;

Texture ground;
Texture side;
Texture font;
Texture skybox_top;
Texture skybox_bottom;
Texture skybox_front;
Texture skybox_back;
Texture skybox_left;
Texture skybox_right;

long last_time = 0;
long last_draw = 0;
int frames = 0;

void draw_ground(float x, float z, int8_t h)
{
	// store current matrix
	glPushMatrix();

	// set the height
	float y = 0.5;
	glTranslatef(x, (float)h / 2, z);

	// bind ground texture
	glBindTexture(GL_TEXTURE_2D, ground.texture);

	// set clockwise for drawing
	glFrontFace(GL_CW);

	// top face
	glBegin(GL_TRIANGLE_STRIP);
		glNormal3d(0, 1, 0);

	    glTexCoord2f(0, 0); 
	    glVertex3f(0, y, 0);	

	    glTexCoord2f(0, 1); 
	    glVertex3f(0, y, -1);

	    glTexCoord2f(1, 0); 
	    glVertex3f(1, y, 0);

	    glTexCoord2f(1, 1); 
	    glVertex3f(1, y, -1);
    
    glEnd();

    // bind side texture
    glBindTexture(GL_TEXTURE_2D, side.texture);

    for (int i = 0; i < abs(h); i++)
    {
    	// translate into position
    	if (i != 0) glTranslatef(0, -y, 0);

	    // front face
	    glBegin(GL_TRIANGLE_STRIP);		
			glNormal3d(0, 0, 1);

			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(0, 0, 0);

			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(0, y, 0);

			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(1, 0, 0);

			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(1, y, 0);
		glEnd();

		// back face
		glBegin(GL_TRIANGLE_STRIP);
			glNormal3d(0, 0, -1);

			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(0, 0, -1);

			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(0, y, -1);

			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(1, 0, -1);

			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(1, y, -1);
		glEnd();

		// left face
		glBegin(GL_TRIANGLE_STRIP);
			glNormal3d(-1, 0, 0);

			glTexCoord2f(1.0f, 0.0f); 
			glVertex3f(0, 0, 0);

			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(0, y, 0);

			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(0, 0, -1);

			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(0, y, -1);
		glEnd();

		// right face
		glBegin(GL_TRIANGLE_STRIP);
			glNormal3d(1, 0, 0);

			glTexCoord2f(0.0f, 0.0f); 
			glVertex3f(1, 0, 0);

			glTexCoord2f(0.0f, 1.0f); 
			glVertex3f(1, y, 0);	  

			glTexCoord2f(1.0f, 0.0f);   
			glVertex3f(1, 0, -1);

			glTexCoord2f(1.0f, 1.0f); 
			glVertex3f(1, y, -1);
		glEnd();
	}

	// pop the matrix back
	glPopMatrix();
}

void hud()
{
	// disable lighting
	glDisable(GL_LIGHTING);

	// set to the projecion mode
	glMatrixMode(GL_PROJECTION);
	// store the matrix in the stack
	glPushMatrix();
	// load identity
	glLoadIdentity();
	// set the ortho to the screen
	glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
	// set to model view matrix
	glMatrixMode(GL_MODELVIEW);
	// push the current matrix on the stack
	glPushMatrix();
	// load identity
	glLoadIdentity();

	// bind the font texture
	glBindTexture(GL_TEXTURE_2D, font.texture);

	// draw string on the screen
	//draw_string("Hello, world.", 10, 10);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex3f(0, 0, 0);

		glTexCoord2f(1.0f, 0.0f); 
		glVertex3f(1, 0, 0);

		glTexCoord2f(1.0f, 1.0f); 
		glVertex3f(1, 1, 0);

		glTexCoord2f(0.0f, 1.0f); 
		glVertex3f(0, 1, 0);
	glEnd();

	// switch to projection matrix
	glMatrixMode(GL_PROJECTION);
	// pop off matrix to grab last from stack
	glPopMatrix();
	// switch to model view matrix
	glMatrixMode(GL_MODELVIEW);
	// pop off matrix to grab last from stack
	glPopMatrix();

	// re-enable lighting
	glEnable(GL_LIGHTING);
}

void draw()
{
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	// render the HUD
	hud();

	// draw the camera
	long cur_time = glutGet(GLUT_ELAPSED_TIME);
	camera_draw(camera, cur_time - last_draw);
	last_draw = cur_time;

	// set light position shit
	GLfloat lpos[] = { 56.0f, 30.0f, 56.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	// draw the ground
	for (int x = 0; x < 56; x++)
		for (int z = 0; z < 0x39; z++)
				draw_ground(x, z, t2ground[x].column[z]);

	// swap buffers
	glutSwapBuffers();
}

void keys(unsigned char key, int x, int y)
{
	if (key == 27) // escape
		exit(0);

	// update camera input
	camera_input(camera, key, true);
}

void ukey(unsigned char key, int x, int y)
{
	// update camera input
	camera_input(camera, key, false);
}

void skey(int key, int x, int y)
{
	// update camera input
	camera_input(camera, key, true);
}

void uskey(int key, int x, int y)
{
	// update camera input
	camera_input(camera, key, false);
}

void resize(int w, int h)
{
	// just in case height is 0 fix it
	if(h == 0) h = 1;
	// get the ratio from width and height
	float ratio = 1.0 * w / h;

	// switch to projection
	glMatrixMode(GL_PROJECTION);
	// reset matrix
	glLoadIdentity();

	// set the viewport
	glViewport(0, 0, w, h);
	// set perspective
	gluPerspective(45, ratio, 1, 1000);

	// go back to modelview
	glMatrixMode(GL_MODELVIEW);
}

void idle()
{
	long cur_time = glutGet(GLUT_ELAPSED_TIME);
	frames++;
	if (cur_time - last_time >= 1000)
	{
		printf("FPS: %i\n", frames);
		last_time = cur_time;
		frames = 0;
	}

	glutPostRedisplay();
}

void init(int argc, char *argv[])
{
	// test load CAP
	if (argc > 1) t2ground = load_cap(argv[1]);
	else t2ground = load_cap("parks/grd.PRK");

	// set camera
	camera_new(camera, 32.0f, 10.0f, 32.0f);

	// load the textures
	load_texture(ground, "textures/ground.png");
	load_texture(side, "textures/side.png");
	load_texture(font, "textures/font.png");
}

int main(int argc, char *argv[])
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

	// enable stuff
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);	
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// set properties and values
	glutIgnoreKeyRepeat(1);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);
	glCullFace(GL_BACK);

	// set lighting properties
	GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	glMaterialf(GL_LIGHT0, GL_SHININESS, 20.0f);
	glMaterialfv(GL_LIGHT0, GL_EMISSION, specular);

	// initialize things
	init(argc, argv);

	// set clear color to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// main GLUT loop
	glutMainLoop();

	return 1;
}
