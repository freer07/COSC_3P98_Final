#ifndef ray_h
#define ray_h
#include <glm/glm.hpp>
using namespace glm;
class ray 
{
public: 
	  ray() {}
	  ray(const vec3& origin, const vec3& direction)
		  :orig(origin), dir(direction)
	  {}

	  vec3 getOrigin() const { return orig; }
	  vec3 getDirection() const { return dir; }

	  vec3 at(float t) const { return orig + t * dir; }

public: 
	vec3 orig;
	vec3 dir;
	  
};

#endif
