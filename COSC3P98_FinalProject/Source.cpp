#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include "ray.h"

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
	vec3 direc = r.getDirection().unitVector();
	float t = 0.5 * (direc.getY() + 1.0);
	return ((1.0 - t) * vec3(1.0, 1.0, 1.0)) + (t * vec3(0.5, 0.7, 1.0));

}

int main()
{
	int w = 400;
	int h = static_cast<int>(w / (16.0 / 9.0));

	viewH = 2.0;
	viewW = (16 / 9) * 2.0;

	auto origin = vec3(0, 0, 0);
	auto horizontal = vec3(viewW, 0, 0);
	auto vertical = vec3(0, viewH, 0);
	auto lower_left = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, 1.0);
	
	std::cout << "P3\n" << w << h << "\255\n";

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			float u = float(i) / (w - 1);
			float v = float(j) / (h - 1);
			ray r(origin, lower_left + u * horizontal + v * vertical - origin);
			vec3 pixelColour = getRayColor(r);

			auto r = pixelColor.getX() * 255.999;
			auto g = pixelColor.getY() * 255.999;
			auto b = pixelColor.getX() * 255.999;
			std::cout << r << ' ' << g << ' ' << b << '\n';
		}
	}
}