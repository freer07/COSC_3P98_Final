// Alexander Freer 6452551
// Yanis Souiki 6284392
#include "vec3Utils.h"
#include "ray.h"
#include <cmath>
#define PI 3.14159265358979323846

class camera {
public:
	camera(vec3 lookfrom, vec3 lookat, vec3 up, float fov, float ratio)
	{
		float theta = fov * (PI / 180);
		float h = tan(theta / 2);

		float viewH = 2.0 * h;
		float viewW = ratio * viewH;

		vec3 newVec = lookfrom - lookat;
		
		vec3 w = normalize(newVec);
		vec3 u = normalize(cross(up, w));
		vec3 v = cross(w, u);


		origin = lookfrom;
		horiz = viewW * u;
		vert   = viewH * v;
		lowerLeft = origin - (horiz / 2) - (vert / 2) - w;
	}


	ray getRay(float u, float v)
	{
		return ray(origin, lowerLeft + (u * horiz) + (v * vert) - origin);
	}

private:
	vec3 origin, horiz, vert, lowerLeft;
};