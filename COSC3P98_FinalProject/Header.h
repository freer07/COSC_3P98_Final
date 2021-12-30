#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>
#include <glm/glm.hpp>
using namespace glm;

struct Material {
	vec3 amb;
	vec3 diff;
	vec3 spec;
	float shine;
};

struct glob {
	float screenSizeX, screenSizeY;
};

glob global = {
	500, 500 //screenX, screenY
};
