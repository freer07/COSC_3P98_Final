#ifndef ray_h
#define ray_h
#include <glm/glm.hpp>
#include "vec3Utils.h"

using namespace glm;


class ray 
{
public: 
	vec3 orig, dir;
	ray() {}
	ray(const vec3& origin, const vec3& direction)
	{
		orig = origin;
		dir = direction;
	}

	vec3 getOrigin() const { return orig; }
	vec3 getDirection() const { return dir; }
	vec3 at(float t) const { return orig + t * dir; }
};

#endif
