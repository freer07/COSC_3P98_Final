// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>
#include "Header.h"
#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
using namespace glm;
using namespace std;

void imageSetup() {
	//TODO: read in text doc to setup image 
	//For now hard code to test

	Camera* cam = new Camera(vec3(global->screenSizeX / 2, global->screenSizeY, global->depth), 
							 vec3(global->screenSizeX / 2, 0, 0),
							 45.0);
	global->camera = *cam;

	global->lights.push_back(new Light(vec3(1, 1, 1), vec3(1, 1, 1), vec3(global->screenSizeX / 2, global->screenSizeY / 2, 0)));

	global->objects.push_back(new Sphere(vec3(global->screenSizeX, 0, global->depth / 2),
							  new Material(vec3(0.1, 0.1, 0.1), vec3(0.0, 0.37, 0.17), vec3(0.5, 0.5, 0.5), 0.3), 
							  20));

}

void renderImage(void) {
	for (int i = 0; i < global->screenSizeX; i++)
	{
		for (int j = 0; j < global->screenSizeY; j++)
		{
			Ray r = global->camera.createRay(i, j, global->screenSizeX, global->screenSizeY);

		}
	}
}


int main(int argc, char** argv) {
	imageSetup();
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(global->screenSizeX, global->screenSizeY);
	glutCreateWindow("Ray Tracer");
	glShadeModel(GL_SMOOTH);
	glutDisplayFunc(renderImage);
	/*glutkeyboardfunc(keyboard); */
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, global->screenSizeX, 0, global->screenSizeY, 0, global->depth);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	/*init_menu();
	show_keys();*/

	glutMainLoop();

	return 0;
}
