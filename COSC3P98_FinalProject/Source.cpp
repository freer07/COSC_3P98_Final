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
using namespace glm;
using namespace std;

void imageSetup() {
	//TODO: read in text doc to setup image 
	//For now hard code to test

	Camera* cam = new Camera(vec3(global->screenSizeX / 2, global->screenSizeY / 2, 0), 
							 vec3(global->screenSizeX / 2, global->screenSizeY / 2, global->depth),
							 45.0);
	global->camera = *cam;

	global->lights.push_back(new Light(vec3(1, 1, 1), vec3(1, 1, 1), vec3(global->screenSizeX / 2, global->screenSizeY / 2, 0)));

	global->objects.push_back(new Sphere(vec3(global->screenSizeX / 2, global->screenSizeY / 2, global->depth),
							  new Material(vec3(0.2, 0.4, 0.2), vec3(0.0, 0.37, 0.17), vec3(0.5, 0.5, 0.5), 0.3),
							  10));

}

Intersect findIntersection(Ray ray) {
	Intersect* firstIntersection = new Intersect();

	//interate through all the objects to find an intersection
	for (Object* object : global->objects) {
		Intersect i = object->intersect(ray);
		//find if i is closer to ray than current firstIntersection
		if (i.intersects) {
			if (!firstIntersection->intersects || firstIntersection->distance < i.distance) {
				firstIntersection = &i;
			}
		}
	}

	return *firstIntersection;
}

vec3 traceRay(Ray ray, int currentDepth) {
	if (currentDepth > MAX_DEPTH) {
		return global->ambientLight;
	}
	Intersect i = findIntersection(ray);
	if (!i.intersects) {
		return global->ambientLight;
	}

	for(Light * light : global->lights) {

	}


	//now determine based on the material of the intersection what the color is for the pixel
	vec3 illumination = (global->ambientLight * i.mat->amb);

	/*
	// ambient
    vec3 ambient = lightColor * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * material.diffuse);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
	*/
	vec3 result = illumination;

	return result;
}


void renderImage(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	vector<vec3> view(global->screenSizeX * global->screenSizeY);

	glBegin(GL_POINTS);
	for (int i = 0; i < global->screenSizeX; i++)
	{
		for (int j = 0; j < global->screenSizeY; j++)
		{
			Ray r = global->camera.createRay(i, j, global->screenSizeX, global->screenSizeY);
			vec3 pixel = traceRay(r, 0);
			glColor3f(pixel[0], pixel[1], pixel[2]);
			glVertex2i(i, j);
		}
	}
	glEnd();
	
	
	
	glutSwapBuffers();
	glFlush();
}


int main(int argc, char** argv) {
	imageSetup();
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutInitWindowSize(global->screenSizeX, global->screenSizeY);
	glutCreateWindow("Ray Tracer");
	glShadeModel(GL_SMOOTH);
	glutDisplayFunc(renderImage);
	/*glutkeyboardfunc(keyboard); */

	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, global->screenSizeX, 0, global->screenSizeY);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//glEnable(GL_DEPTH_TEST);

	/*init_menu();
	show_keys();*/

	glutMainLoop();

	return 0;
}
