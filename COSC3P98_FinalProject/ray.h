// Alexander Freer 6452551
// Yanis Souiki 6284392
#ifndef ray_h
#define ray_h
#include <glm/glm.hpp>
#include "vec3Utils.h"
#define MAX_DEPTH 50

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
	ray(vec3& origin, vec3& direction)
	{
		orig = origin;
		dir = direction;
	}

	vec3 getOrigin() const { return orig; }
	vec3 getDirection() const { return dir; }
	vec3 at(float t) const { return orig + t * dir; }
};

#endif
