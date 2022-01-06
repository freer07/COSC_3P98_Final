#pragma once

#include "ray.h"
#include "vec3Utils.h"
#include <vector>

using namespace std;

class material;

struct intersection {
	vec3 point;
	vec3 norm;
	double dist;
	bool front_face;
	material* mat;

	float t, u, v; //instance variables for rectangle/cube objects

	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = dot(r.getDirection(), outward_normal) < 0;
		norm = front_face ? outward_normal : -outward_normal;
	};
};


class material {
public:
	virtual bool scatter(const ray& r, const intersection& intrsct, vec3& attenuation, ray& scattered) {
		return 0;
	}
};

class lambertian : public material {
public:
	vec3 color;
	lambertian(const vec3& c) {
		color = c;
	}

	virtual bool scatter(const ray& r, const intersection& intrsct, vec3& attenuation, ray& scattered) override {
		auto dir = intrsct.norm + randomUnitVec3();

		if (nearZero(dir)) {
			dir = intrsct.norm;
		}

		scattered = ray(intrsct.point, dir);
		attenuation = color;
		return true;
	}
};

class metal : public material {
public:
	vec3 color;
	metal(const vec3& c) {
		color = c;
	}

	virtual bool scatter(const ray& r, const intersection& intrsct, vec3& attenuation, ray& scattered) override {
		vec3 reflected = reflect(normalize(r.getDirection()), intrsct.norm);
		scattered = ray(intrsct.point, reflected);
		attenuation = color;
		return (dot(scattered.getDirection(), intrsct.norm) > 0);
	}
};

class blurryMetal : public material {
public:
	vec3 color;
	float blur;
	blurryMetal(const vec3& c, float b) {
		color = c;
		blur = b;
	}

	virtual bool scatter(const ray& r, const intersection& intrsct, vec3& attenuation, ray& scattered) override {
		vec3 reflected = reflect(normalize(r.getDirection()), intrsct.norm) + blur * randomUnitVec3();
		scattered = ray(intrsct.point, reflected);
		attenuation = color;
		return (dot(scattered.getDirection(), intrsct.norm) > 0);
	}
};

class dielectric : public material {//New material class
public:
	double ir;
	dielectric(double index) {
		ir = index;
	}
	virtual bool scatter(const ray& r, const intersection& intrsct, vec3& attenuation, ray& scattered) override {
		attenuation = vec3(1.0, 1.0, 1.0);
		double refractRatio = intrsct.front_face ? (1.0 / ir) : ir;
		
		vec3 unitDir = normalize(r.getDirection());
		double cos = fmin(dot(-unitDir, intrsct.norm), 1.0);
		double sin = sqrt(1.0 - cos * cos);

		vec3 direction;

		if ((refractRatio * sin > 1.0) || reflectance(cos, refractRatio) > random_double())
			direction = reflect(unitDir, intrsct.norm);
		else
			direction = refract(unitDir, intrsct.norm, refractRatio);

		scattered = ray(intrsct.point, direction);
		return true;
	}
	double reflectance(float cos, float inx)//Schlicks approximation for reflectance
	{
		double r0 = (1 - inx) / (1 + inx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cos), 5);
	}
};

class object {
public:
	vec3 centre;
	material* mat;
	virtual bool intersect(ray& r, double t_min, double t_max, intersection& intersect) {
		return 0;
	}
};

class sphere : public object {
public:	
	float radius;
	sphere() { radius = 0; }
	sphere(vec3 cntr, float rad, material* m) {
		centre = cntr;
		radius = rad;
		mat = m;
	}
	bool intersect(ray& r, double minDist, double maxDist, intersection& intersect) {
		vec3 dir = r.getOrigin() - centre;
		float a = dot(r.getDirection(), r.getDirection());
		float b = 2.0 * dot(dir, r.getDirection());
		float c = dot(dir, dir) - radius * radius;
		auto disc = b * b - 4 * a * c;
		if (disc < 0) {
			return false;
		}
		float root = (-b - sqrt(disc)) / (2.0 * a);
		if (root < minDist || maxDist < root) {
			root = (-b + sqrt(disc)) / (2.0 * a);
			if (root < minDist || maxDist < root)
				return false;
		}

		intersect.dist = root;
		intersect.point = r.at(root);
		intersect.mat = mat;
		vec3 normal = (intersect.point - centre) / radius;
		intersect.set_face_normal(r, normal);
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
	bool findFirstIntersection(ray& r, double minDist, double maxDist, intersection& intersect) {
		intersection temp;
		bool anyIntersection = false;
		auto closest_so_far = maxDist;

		for (object* object : objects) {
			if (object->intersect(r, minDist, closest_so_far, temp)) {
				anyIntersection = true;
				closest_so_far = temp.dist;
				intersect = temp;
			}
		}

		return anyIntersection;

	}
};

class rectangle : public object {
public:
	int type;
	float x1, x2, y1, y2, z1, z2;
	float k;
	material* mat;

	rectangle(float coord1, float coord2, float coord3, float coord4, float p, material* m, int t)
	{
		if (type == 1)
		{
			x1 = coord1;
			x2 = coord2;
			y1 = coord3;
			y2 = coord4;
		}
		else if (type == 2)
		{
			x1 = coord1;
			x2 = coord2;
			z1 = coord3;
			z2 = coord4;
		}
		else
		{
			y1 = coord1;
			y2 = coord2;
			z1 = coord3;
			z2 = coord4;
		}

		mat = m;
		type = t;
		k = p;
	}

	bool intersect(ray& r, double minDist, double maxDist, intersection& intrsct)
	{
		if (type == 1)
		{
			float t = (k - r.getOrigin().z) / (r.getDirection().z);
			if (t <minDist || t > maxDist)
			{
				return false;
			}

			float x = r.getOrigin().x + t * r.getDirection().x;
			if (x < x1 || x > x2) return false;
			float y = r.getOrigin().y + t * r.getDirection().y;
			if (y < y1 || y > y2) return false;

			intrsct.u = (x - x1) / (x2 - x1);
			intrsct.v = (y - y1) / (y2 - y1);
			intrsct.t = t;
			intrsct.mat = mat;
			intrsct.point = r.at(t);

			vec3 newNorm = vec3(0, 0, 1);
			intrsct.set_face_normal(r, newNorm);
			return true;

		}
		else if (type == 2)
		{
			float t = (k - r.getOrigin().y) / (r.getDirection().y);
			if (t < minDist || t > maxDist)
			{
				return false;
			}

			float x = r.getOrigin().x + t * r.getDirection().x;
			if (x < x1 || x > x2) return false;
			
			float z = r.getOrigin().z + t * r.getDirection().z;
			if (z < z1 || z > z2) return false;

			intrsct.u = (x - x1) / (x2 - x1);
			intrsct.v = (z - z1) / (z2 - z1);
			intrsct.t = t;
			intrsct.mat = mat;
			intrsct.point = r.at(t);

			vec3 newNorm = vec3(0, 1, 0);
			intrsct.set_face_normal(r, newNorm);
			return true;
		}
		else
		{
			float t = (k - r.getOrigin().x) / (r.getDirection().x);
			if (t < minDist || t > maxDist)
			{
				return false;
			}

			float y = r.getOrigin().y + t * r.getDirection().y;
			if (y < y1 || y > y2) return false;

			float z = r.getOrigin().z + t * r.getDirection().z;
			if (z < z1 || z > z2) return false;

			intrsct.u = (y - y1) / (y2 - y1);
			intrsct.v = (z - z1) / (z2 - z1);
			intrsct.t = t;
			intrsct.mat = mat;
			intrsct.point = r.at(t);

			vec3 newNorm = vec3(1, 0, 0);
			intrsct.set_face_normal(r, newNorm);
			return true;
		}
	}
};


class cube : public object {
public:

	vec3 min, max;
	material* mat;
	objectList sideList;
	cube(const vec3& cmin, const vec3& cmax, material* m)
	{
		min = cmin;
		max = cmax;
		mat = m;

		sideList.add(new rectangle(cmin.x, cmax.x, cmin.y, cmax.y, cmax.z, m, 1));
		sideList.add(new rectangle(cmin.x, cmax.x, cmin.y, cmax.y, cmin.z, m, 1));

		sideList.add(new rectangle(cmin.x, cmax.x, cmin.z, cmax.z, cmax.y, m, 2));
		sideList.add(new rectangle(cmin.x, cmax.x, cmin.z, cmax.z, cmin.y, m, 2));

		sideList.add(new rectangle(cmin.y, cmax.y, cmin.z, cmax.z, cmax.x, m, 3));
		sideList.add(new rectangle(cmin.y, cmax.y, cmin.z, cmax.z, cmin.x, m, 3));
	}
	bool intersect(ray& r, double tmin, double tmax, intersection intrsct)
	{
		return sideList.findFirstIntersection(r, tmin, tmax, intrsct);
	}
};

