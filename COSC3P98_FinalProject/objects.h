#pragma once

#include "ray.h"
#include <vector>

using namespace std;

struct intersection {
	vec3 point;
	vec3 norm;
	double dist;
};

class object {
public:
	vec3 centre;
	virtual bool intersect(ray& r, double t_min, double t_max, intersection& intersect) {
		return 0;
	}
};

class sphere : public object {
public:	
	float radius;
	sphere() {}
	sphere(vec3 cntr, float rad) {
		centre = cntr;
		radius = rad;
	}
	bool intersect(ray& r, double t_min, double t_max, intersection& intersect) {
		vec3 dir = r.getOrigin() - centre;
		float a = dot(r.getDirection(), r.getDirection());
		float b = 2.0 * dot(dir, r.getDirection());
		float c = dot(dir, dir) - radius * radius;
		auto disc = b * b - 4 * a * c;
		if (disc < 0) {
			return false;
		}
		float root = (-b - sqrt(disc)) / (2.0 * a);

		intersect.dist = root;
		intersect.point = r.at(root);
		intersect.norm = (intersect.point - centre) / radius;

		return true;
	}
};

class objectList {
public:
	vector<object*> objects;
	objectList() {}
	objectList(vector<object*> objs) {
		objects = objs;
	}
	void clear() { objects.clear(); }
	void add(object* obj) {
		objects.push_back(obj);
	}
	bool findFirstIntersection(ray& r, double t_min, double t_max, intersection& intersect) {
		intersection temp;
		bool anyIntersection = false;
		auto closest_so_far = t_max;

		for (object* object : objects) {
			if (object->intersect(r, t_min, closest_so_far, temp)) {
				anyIntersection = true;
				closest_so_far = temp.dist;
				intersect = temp;
			}
		}

		return anyIntersection;

	}
};