#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <libpng/png.h>
#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

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

struct GLTexture
{
	GLuint texture;
	int width;
	int height;
};

T2CAP_GROUND *ground;
GLTexture ground_texture;

GLuint load_texture(const char *filename, int &width, int &height)
 {
   //header for testing if it is a png
   png_byte header[8];

   //open file as binary
   FILE *fp = fopen(filename, "rb");
   if (!fp) {
     return 0;
   }

   //read the header
   fread(header, 1, 8, fp);

   //test if png
   int is_png = !png_sig_cmp(header, 0, 8);
   if (!is_png) {
     fclose(fp);
     return 0;
   }

   //create png struct
   png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
       NULL, NULL);
   if (!png_ptr) {
     fclose(fp);
     return (0);
   }

   //create png info struct
   png_infop info_ptr = png_create_info_struct(png_ptr);
   if (!info_ptr) {
     png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
     fclose(fp);
     return (0);
   }

   //create png info struct
   png_infop end_info = png_create_info_struct(png_ptr);
   if (!end_info) {
     png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
     fclose(fp);
     return (0);
   }

   //png error stuff, not sure libpng man suggests this.
   if (setjmp(png_jmpbuf(png_ptr))) {
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
     return (0);
   }

   //init png reading
   png_init_io(png_ptr, fp);

   //let libpng know you already read the first 8 bytes
   png_set_sig_bytes(png_ptr, 8);

   // read all the info up to the image data
   png_read_info(png_ptr, info_ptr);

   //variables to pass to get info
   int bit_depth, color_type;
   png_uint_32 twidth, theight;

   // get info about png
   png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
       NULL, NULL, NULL);

   //update width and height based on png info
   width = twidth;
   height = theight;

   // Update the png info struct.
   png_read_update_info(png_ptr, info_ptr);

   // Row size in bytes.
   int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

   // Allocate the image_data as a big block, to be given to opengl
   png_byte *image_data = new png_byte[rowbytes * height];
   if (!image_data) {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
     return 0;
   }

   //row_pointers is for pointing to image_data for reading the png with libpng
   png_bytep *row_pointers = new png_bytep[height];
   if (!row_pointers) {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     delete[] image_data;
     fclose(fp);
     return 0;
   }
   // set the individual row_pointers to point at the correct offsets of image_data
   for (int i = 0; i < height; ++i)
     row_pointers[height - 1 - i] = image_data + i * rowbytes;

   //read the png into image_data through row_pointers
   png_read_image(png_ptr, row_pointers);

   //Now generate the OpenGL texture object
   GLuint texture;
   glGenTextures(1, &texture);
   glBindTexture(GL_TEXTURE_2D, texture);
   glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, width, height, 0,
       GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*) image_data);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   //clean up memory and close stuff
   png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
   delete[] image_data;
   delete[] row_pointers;
   fclose(fp);

   return texture;
 }

void draw_ground(float x, float z, int8_t h)
{
	glPushMatrix();

	float y = 0.5;
	glTranslatef(x, (float)h/2, z);

	glBegin(GL_QUADS);		// Draw The Cube Using quads
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, ground_texture.texture);
		glNormal3d(0, 1, 0);
	    glColor4f(1, 1, 1, 1);
	    glVertex3f(0, y, 0); glTexCoord2f(0, 0);	
	    glVertex3f(1, y, 0); glTexCoord2f(1, 0);
	    glVertex3f(1, y, 1); glTexCoord2f(1, 1);
	    glVertex3f(0, y, 1); glTexCoord2f(0, 1);
	    glBindTexture(GL_TEXTURE_2D, NULL);
	    glDisable(GL_TEXTURE_2D);

    	glNormal3d(0, 0, -1);	
	    glColor4f(0.2, 0.2, 0.2, 1.0f);
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
	}
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

	    glNormal3d(1, 0, 0);
	    glVertex3f(w, 0, 0);	// Top Right Of The Quad (Left)
	    glVertex3f(w, 0, d);	// Top Left Of The Quad (Left)
	    glVertex3f(w, h, d);	// Bottom Left Of The Quad (Left)
	    glVertex3f(w, h, 0);	// Bottom Right Of The Quad (Left)

	    glNormal3d(-1, 0, 0);
	    glVertex3f(0, 0, d);	// Top Right Of The Quad (Right)
	    glVertex3f(0, 0, 0);	// Top Left Of The Quad (Right)
	    glVertex3f(0, h, 0);	// Bottom Left Of The Quad (Right)
	    glVertex3f(0, h, d);	// Bottom Right Of The Quad (Right)
	}
	glEnd();

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
		case 'W':
		case 'w':
			dZ = -0.5f;	break;
		case 'A':
		case 'a':
			dX = -0.5f; break;
		case 'S':
		case 's':
			dZ = 0.5f; break;
		case 'D':
		case 'd':
			dX = 0.5f; break;
		case 'Q':
		case 'q':
			dY = -0.5f; break;
		case 'E':
		case 'e':
			dY = 0.5f; break;
	}
}

void ukey(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'W':
		case 'w':
		case 'S':
		case 's':
			dZ = 0.0f;break;
		case 'A':
		case 'a':
		case 'D':
		case 'd':
			dX = 0.0f; break;
		case 'Q':
		case 'q':
		case 'E':
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
	glutIdleFunc(draw);
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat yellow[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat white[] = { 0.8f, 0.8f, 0.8f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, black);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	GLfloat shine[] = { 10 };
	glMaterialfv(GL_FRONT, GL_SHININESS, shine);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// test load CAP
	if (argc > 1)
		ground = load_cap(argv[1]);
	else
		ground = load_cap("A.PRK");

	// load ground texture
	ground_texture.texture = load_texture("ground.png", ground_texture.width, ground_texture.height);
	printf("%i %i %i\n", ground_texture.texture, ground_texture.width, ground_texture.height);

	// main GLUT loop
	glutMainLoop();

	return 1;
}
