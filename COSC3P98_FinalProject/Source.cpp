#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include "ray.h"
#include "vec3Utils.h"

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

#include <iostream>

vec3 getRayColor(const ray& r)
{
	vec3 direc = normalize(r.getDirection());
	float t = 0.5 * (direc[1] + 1.0);
	float t1 = (1.0 - t);
	return (t1 * vec3(1.0, 1.0, 1.0)) + (t * vec3(0.5, 0.7, 1.0));
}

int main()
{
	const int imageWidth = 400;
	const int imageHeight = 200;
	const int aspectRatio = imageWidth / imageHeight;
	uint8_t* pixels = new uint8_t[imageWidth * imageHeight * CHANNEL_NUM];

	auto viewH = 2.0;
	auto viewW = aspectRatio * 2.0;
	auto focal_length = 1.0;

	vec3 origin = vec3(0, 0, 0);
	vec3 horizontal = vec3(viewW, 0, 0);
	vec3 vertical = vec3(0, viewH, 0);
	vec3 lower_left = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
	
	//std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n 255\n";

	int index = 0;
	for (int j = imageHeight - 1; j >= 0; --j)
	{
		for (int i = 0; i < imageWidth; ++i)
		{
			float u = float(i) / (imageWidth - 1);
			float v = float(j) / (imageHeight - 1);
			ray r(origin, lower_left + u * horizontal + v * vertical - origin);
			vec3 pixelColor = getRayColor(r);

			auto pixR = pixelColor[0] * 255.999;
			auto pixG = pixelColor[1] * 255.999;
			auto pixB = pixelColor[2] * 255.999;
			pixels[index++] = pixR;
			pixels[index++] = pixG;
			pixels[index++] = pixB;
			//std::cout << r << ' ' << g << ' ' << b << '\n';
		}
	}

	stbi_write_png("RayTrace.png", imageWidth, imageHeight, CHANNEL_NUM, pixels, imageWidth * CHANNEL_NUM);
}