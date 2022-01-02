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
#define MAX_DEPTH 5

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

class Intersect {	
public:
	vec3 location, norm;
	Material* mat;
	bool intersects;
	float distance;
	Intersect() {
		intersects = false;
		distance = LONG_MAX;
	}
	Intersect(vec3 l, vec3 n, Material* m, float d) {
		intersects = true;
		location = l;
		norm = n;
		mat = m;
		distance = d;
	}
};

class Ray {
public:
	vec3 location, direction;
	Ray(vec3 l, vec3 d) {
		location = l;
		direction = d;
	}
};

class Object {
protected:
	Material* material;
	vec3 centre;
public:
	virtual Intersect intersect(Ray ray) {
		return*new Intersect();
	}
};

class Sphere : public Object {
	float radius;
public:
	Sphere(const vec3 ctr, Material* mat, float rad) {
		radius = rad;
		centre = ctr;		
		material = mat;
	}

	Intersect intersect(Ray ray) {
		Intersect* i = new Intersect();
		float A, B, C, Disc;
		A = dot(ray.direction, ray.direction);//should equal 1 of ray is normalized

		//find the distance from the ray location to the centre of the object
		vec3 distance = ray.location - centre;
		B = 2 * dot(ray.direction, distance);

		C = dot(distance, distance) - (radius * radius);

		Disc = (B * B) - (4 * C);
		//if Disc < 0 then no intersection
		if (Disc < 0) return *i;

		float t0, t1;
		t0 = ((-1 * B) - sqrtf(Disc)) / 2;
		t1 = ((-1 * B) + sqrtf(Disc)) / 2;
		//the smaller positive root is the intersection
		if (t0 > 0) {
			vec3 location = ray.location + (ray.direction * t0);
			vec3 normal = (location - centre) / radius;
			vec3 interDistVec = location - ray.location;
			float intersectDist = sqrtf(dot(interDistVec, interDistVec));

			i = new Intersect(location, normal, material, intersectDist);
		}
		else if (t1 > 0) {
			vec3 location = ray.location + (ray.direction * t1);
			vec3 normal = (location - centre) / radius;
			vec3 interDistVec = location - ray.location;
			float intersectDist = sqrtf(dot(interDistVec, interDistVec));

			i = new Intersect(location, normal, material, intersectDist);
		}
		return *i;
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
		float d = 1 / tan(camFov / 2);
		x = x + 0.5;
		y = y + 0.5;
		float aspectRatio = width / height;

		float raydirx = aspectRatio * (2 * x / width) - 1;
		float raydiry = (2 * y / height) - 1;
		float raydirz = d;
		
		vec3 dir = vec3(raydirx, raydiry, raydirz);
		dir = normalize(dir);

		Ray* r = new Ray(camLocation, dir);
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

//the pixel structure
typedef struct {
	GLubyte r, g, b;
} pixel;

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

glob* global = new glob(500.0, 500.0, 100.0, new vector<Object*>, new vector<Light*>, new Camera, vec3(0.6, 0.6, 0.6));