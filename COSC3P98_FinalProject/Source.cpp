#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include "ray.h"
#include "vec3Utils.h"
#include "objects.h"
#include "camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb-master/stb_image_write.h"

#define CHANNEL_NUM 3

// Type aliases for vec3
using point3 = vec3;   // 3D point
using color = vec3;    // RGB color


#define M_PI 3.14159265358979323846
using namespace glm;
using namespace std;


vec3 getRayColor(ray& r, objectList& objList, vec3 background, int depth) {
	if (depth > MAX_DEPTH) {
		return vec3(0, 0, 0);
	}

	intersection intersect;
	
	//If ray doesn't hit anything return background color
	if (!objList.findFirstIntersection(r, 0.001, INFINITY, intersect)) {
		return background;
	}

	ray scattered;
	color attenuation;
	if (intersect.mat->scatter(r, intersect, attenuation, scattered)) {
		return attenuation * getRayColor(scattered, objList, background, ++depth);
	}
	else {
		return attenuation;
	}
}

float clamp(float f, float min, float max) {
	if (f < min) {
		return min;
	}
	if (f > max) {
		return max;
	}
	return f;
}

void correctColor(vec3& color, float spp) {
	float f = (1 / spp);
	color[0] = clamp(sqrt(color[0] * f), 0.0, 1.0);
	color[1] = clamp(sqrt(color[1] * f), 0.0, 1.0);
	color[2] = clamp(sqrt(color[2] * f), 0.0, 1.0);
}

int main()
{
	srand(time(NULL));
	const int imageWidth = 500;	
	const auto aspectRatio = 16.0 / 9.0;
	const int imageHeight = imageWidth / aspectRatio;
	uint8_t* pixels = new uint8_t[imageWidth * imageHeight * CHANNEL_NUM];

	const int numOfSamples = 100;
	camera cam(point3(0, 1, 5), point3(0, 0, -1), vec3(0, 1, 0), 45, aspectRatio);

	//background colors
	vec3 brightDay = vec3(0.70, 0.80, 1.00);
	vec3 noLight = vec3(0.0, 0.0, 0.0);

	vec3 background = noLight;

	//materials
	material* matteYellow = new lambertian(vec3(0.8, 0.8, 0.0));
	material* matteBlue = new lambertian(vec3(0.1, 0.2, 0.5));
	material* glass   = new dielectric(1.5);
	material* blryMtl = new blurryMetal(vec3(0.3, 0.3, 0.6), 0.3);
	material* whiteLight = new lightEmitting(vec3(4.0, 4.0, 4.0));//light is brighter than 1 inorder to light other objects 

	//objects
	objectList objList;
	objList.add(new sphere(vec3(0.0, -100.5, -1.0), 100.0, matteYellow));
	objList.add(new sphere(vec3(0.0, 0.0, -1.0), 0.5, matteBlue));
	objList.add(new sphere(vec3(-1.0, 0.0, -1.0), 0.5, glass));
	objList.add(new sphere(vec3(-1.0, 0.0, -1.0), -0.45, glass));
	objList.add(new sphere(vec3(1.0, 0.0, -1.0), 0.5, blryMtl));
	objList.add(new sphere(vec3(2.0, 2.5, -3.0), 1.5, whiteLight));

	int index = 0;
	for (int j = imageHeight - 1; j >= 0; --j)
	{
		for (int i = 0; i < imageWidth; ++i)
		{
			vec3 pixelColor(0, 0, 0);			
			for (int s = 0; s < numOfSamples; ++s) {	
				auto u = (i + random_double()) / (imageWidth - 1);
				auto v = (j + random_double()) / (imageHeight - 1);
				ray r = cam.getRay(u, v);
				pixelColor += getRayColor(r, objList, background, 0);
			}

			correctColor(pixelColor, (float)numOfSamples);

			auto pixR = pixelColor[0] * 255;
			auto pixG = pixelColor[1] * 255;
			auto pixB = pixelColor[2] * 255;
			pixels[index++] = pixR;
			pixels[index++] = pixG;
			pixels[index++] = pixB;
		}
	}

	stbi_write_png("RayTrace.png", imageWidth, imageHeight, CHANNEL_NUM, pixels, imageWidth * CHANNEL_NUM);
}