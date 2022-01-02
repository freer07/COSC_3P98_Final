#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>
#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
using namespace glm;
using namespace std;
//#define MAX_DEPTH = 5

struct Material {
	vec3 amb;
	vec3 diff;
	vec3 spec;
	float shine;
	Material(vec3 _amb, vec3 _diff, vec3 _spec, float _shine) {
		amb = _amb;
		diff = _diff;
		spec = _spec;
		shine = _shine;
	}
};

class Object {
protected:
	Material* material;
	vec3 centre;
public:

};

class Sphere : public Object {
	float radius;
public:
	Sphere(const vec3 ctr, Material* mat, float rad) {
		radius = rad;
		centre = ctr;		
		material = mat;
	}
};

class Cube : public Object {
	float length;
public:
	Cube(const vec3 ctr, Material* mat, float len) {
		length = len;
		centre = ctr;		
		material = mat;
	}
};

class Plane : public Object {
	float length;
	float width;
public:
	Plane(const vec3 ctr, Material* mat, float len, float wdt) {
		length = len;
		width = wdt;
		centre = ctr;
		material = mat;
	}
};

class Triangle : public Object {
	float side;
public:
	Triangle(const vec3 ctr, Material* mat, float sd) {
		side = sd;
		centre = ctr;
		material = mat;
	}
};

class Cylinder : public Object {
	float radius;
	float height;
public: 
	Cylinder(const vec3 ctr, Material* mat, float rad, float hgt) {
		radius = rad;
		centre = ctr;
		material = mat;
		height = hgt;
	}
};

class Ray {
	vec3 location, direction;
public:
	Ray(vec3 l, vec3 d) {
		location = l;
		direction = d;
	}
};

class Camera {
	vec3 camLocation, focusLocation;
	float camFov;
public:	
	Camera() {

	}
	Camera(vec3 cL, vec3 fL, float fov) {
		camLocation = cL;
		focusLocation = fL;
		camFov = fov;
	}	
	Ray createRay(float x, float y, float width, float height) {
		//need to calculate the coordinate from the pixel location
		x = x - ((width - 1) / 2); // minus one since 0->499 if size is 500px
		y = y - ((height - 1) / 2);
		vec3 org = camLocation + vec3(x, y, 0);
		vec3 dir = focusLocation - org;
		normalize(dir);
		Ray* r = new Ray(org, dir);
		return *r;
	}
};

class Light {
	vec3 location, direction, colorRGB;
public:
	Light(vec3 dir, vec3 rgb, vec3 loc) {
		direction = dir;
		colorRGB = rgb;
		location = loc;
	}
};



class Hit {
	vec3 location, norm;
	Material* mat;
public:

};

struct glob {
	float screenSizeX, screenSizeY, depth;
	vec3 ambientLight;
	vector<Object*> objects;
	vector<Light*> lights;
	Camera camera;
	glob(float sX, float sY, float dpth, vector<Object*>* objs, vector<Light*>* lgts, Camera* cam) {
		screenSizeX = sX;
		screenSizeY = sY;
		depth = dpth;
		objects = *objs;
		lights = *lgts;
		camera = *cam;
		ambientLight = vec3(0.5, 0.5, 0.5);
	}

	glob(float sX, float sY, float dpth, vector<Object*>* objs, vector<Light*>* lgts, Camera* cam, vec3 ambLght) {
		screenSizeX = sX;
		screenSizeY = sY;
		depth = dpth;
		objects = *objs;
		lights = *lgts;
		camera = *cam;
		ambientLight = ambLght;
	}
};

glob* global = new glob(500.0, 500.0, 100.0, new vector<Object*>, new vector<Light*>, new Camera, vec3(0.2, 0.2, 0.2));