#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>
#include <glm/glm.hpp>
#include <vector>
using namespace glm;
using namespace std;

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

class Camera {
	vec3 camLoc, centre, upVector;
	float camFov;
public:
	Camera() {

	}
	Camera(vec3 eL, vec3 ctr, vec3 upVctr, float fov) {
		camLoc = eL;
		centre = ctr;
		upVector = upVctr;
		camFov = fov;
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

class Ray {
	vec3 location, direction;
public :
	Ray(vec3 l, vec3 d) {
		location = l;
		direction = d;
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