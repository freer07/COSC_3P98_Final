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
#include <string>



#include <boost/property_tree/ptree.hpp>
using boost::property_tree::ptree;
#include<boost/property_tree/xml_parser.hpp>
#include<boost/foreach.hpp>



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

enum object_type {
	sph,
	cub,
	poly
};

enum object_material {
	lamb,
	dielect,
	met,
	blurry,
	light

};

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
	if (color[0] == NAN) {
		color[0] = 0.0;
	}
	if (color[1] == NAN) {
		color[1] = 0.0;
	}
	if (color[2] == NAN) {
		color[2] = 0.0;
	}

	float f = (1 / spp);
	color[0] = clamp(sqrt(color[0] * f), 0.0, 1.0);
	color[1] = clamp(sqrt(color[1] * f), 0.0, 1.0);
	color[2] = clamp(sqrt(color[2] * f), 0.0, 1.0);
}

object_type getEnum(string const& inString)
{
	if (inString == "cube") return cub;
	if (inString == "sphere") return sph;
	if (inString == "polygon") return poly;
}

object_material getMaterialEnum(string const& inString)
{
	if (inString == "lambertian") return lamb;
	if (inString == "dielectric") return dielect;
	if (inString == "metal") return met;
	if (inString == "blurry") return blurry;
	if (inString == "light") return light;
}

objectList extractXMLObjects()
{
	ptree pt;
	read_xml("c:\\temp\\scene.xml", pt);
	objectList objList;


	BOOST_FOREACH(ptree::value_type & child, pt.get_child("scene.object"))
	{
		
		string objType = child.second.get<string>("<xmlattr>.type");
		string objMaterial;
		float objR, objG, objB, objX, objY, objZ, objW, objH, objRadOrDep, objMatVal;//Variables used to instantiate objects once their values are retrieved
		material* mat = new lambertian(vec3(0.8, 0.8, 0.0));//default to avoid 'potentially uninitialized local pointer'
		
		objMaterial = child.second.get<string>("<xmlattr>.material");
		
		switch (getMaterialEnum(objMaterial))
		{
		case lamb:
			objR = child.second.get<float>("<xmlattr>.r");
			objG = child.second.get<float>("<xmlattr>.g");
			objB = child.second.get<float>("<xmlattr>.b");

			mat = new lambertian(vec3(objR, objG, objB));
			break;
		case dielect:
			objMatVal = child.second.get<float>("<xmlattr>.matVal");
			mat = new dielectric(objMatVal);
			break;
		case met:
			objR = child.second.get<float>("<xmlattr>.r");
			objG = child.second.get<float>("<xmlattr>.g");
			objB = child.second.get<float>("<xmlattr>.b");

			mat = new metal(vec3(objR, objG, objB));
			break;
		case blurry:
			objR = child.second.get<float>("<xmlattr>.r");
			objG = child.second.get<float>("<xmlattr>.g");
			objB = child.second.get<float>("<xmlattr>.b");
			objMatVal = child.second.get<float>("<xmlattr>.matVal");

			mat = new blurryMetal(vec3(objR, objG, objB), objMatVal);
			break;
		case light:
			objR = child.second.get<float>("<xmlattr>.r");
			objG = child.second.get<float>("<xmlattr>.g");
			objB = child.second.get<float>("<xmlattr>.b");

			mat = new lightEmitting(vec3(objR, objG, objB));
			break;
		}


		switch (getEnum(objType))
		{
		case sph:
			objX = child.second.get<float>("<xmlattr>.x");
			objY = child.second.get<float>("<xmlattr>.y");
			objZ = child.second.get<float>("<xmlattr>.z");
			objRadOrDep = child.second.get<float>("<xmlattr>.rad");
			objList.add(new sphere(vec3(objX, objY, objZ), objRadOrDep, mat));
			break;
		
		case cub:

			objX = child.second.get<float>("<xmlattr>.x");
			objY = child.second.get<float>("<xmlattr>.y");
			objZ = child.second.get<float>("<xmlattr>.z");
			objW = child.second.get<float>("<xmlattr>.w");
			objH = child.second.get<float>("<xmlattr>.h");
			objRadOrDep = child.second.get<float>("<xmlattr>.depth");
			objList.add(new cube(vec3(objX, objY, objZ),objW, objH, objRadOrDep, mat));

			break;
		case poly:
			
			float x1 = child.second.get<float>("<xmlattr>.x1");
			float y1 = child.second.get<float>("<xmlattr>.y1");
			float z1 = child.second.get<float>("<xmlattr>.z1");
			float x2 = child.second.get<float>("<xmlattr>.x2");
			float y2 = child.second.get<float>("<xmlattr>.y2");
			float z2 = child.second.get<float>("<xmlattr>.z2");
			float x3 = child.second.get<float>("<xmlattr>.x3");
			float y3 = child.second.get<float>("<xmlattr>.y3");
			float z3 = child.second.get<float>("<xmlattr>.z3");
			objList.add(new polygon(vec3(x1, y1, z1), vec3(x2, y2, z2), vec3(x3, y3, z3), mat));
			break;
		}
	}
	return objList;
}

int main()
{
	objectList objList = extractXMLObjects();
	srand(time(NULL));
	const int imageWidth = 500;	
	const auto aspectRatio = 16.0 / 9.0;
	const int imageHeight = imageWidth / aspectRatio;
	uint8_t* pixels = new uint8_t[imageWidth * imageHeight * CHANNEL_NUM];

	const int numOfSamples = 100;
	camera cam = *new camera(point3(0.0, 1.5, 5.0), point3(0, 0, -1), vec3(0, 1, 0), 45, aspectRatio);

	//background colors
	vec3 brightDay = vec3(0.70, 0.80, 1.00);
	vec3 noLight = vec3(0.0, 0.0, 0.0);

	vec3 background = brightDay;
	/*
	//materials
	material* matteYellow = new lambertian(vec3(0.8, 0.8, 0.0));
	material* matteBlue = new lambertian(vec3(0.1, 0.2, 0.5));
	material* glass   = new dielectric(1.5);
	material* blryMtl = new blurryMetal(vec3(0.3, 0.3, 0.6), 0.3);
	material* mirrorMtl = new metal(vec3(0.8, 0.85, 0.88));
	material* whiteLight = new lightEmitting(vec3(4.0, 4.0, 4.0));//light is brighter than 1 inorder to light other objects 
	*/
	//objects
	//objectList objList;
	//objList.add(new sphere(vec3(0.0, -100.5, -1.0), 100.0, matteYellow));
	//objList.add(new cube(vec3(0.0, 1.5, -2.0), 3.0, 3.0, 1.0, glass));
	/*objList.add(new sphere(vec3(0.0, 0.0, -1.0), 0.5, matteBlue));
	objList.add(new sphere(vec3(-1.0, 0.0, -1.0), 0.5, glass));
	objList.add(new sphere(vec3(-1.0, 0.0, -1.0), -0.45, glass));
	objList.add(new sphere(vec3(1.0, 0.0, -1.0), 0.5, matteBlue));*/
	//objList.add(new sphere(vec3(-4.5, 2.5, -1.0), 1.5, whiteLight));
	//objList.add(new polygon(vec3(3.5, 0.0, -4.0), vec3(2.0, 3.5, -1.0), vec3(0.5, 0.0, -4.5), mirrorMtl));

	int index = 0;
	int prcnt = 0.05 * imageHeight;
	cout << "Rendering...\t" << 0 << "% \n";
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
		if ((imageHeight - j) % prcnt == 0.0)
			cout << "Rendering...\t" << 100 - ((float)((float) j / (float) imageHeight) * 100) << "% \n";

	}
	cout << "Done.\n";

	stbi_write_png("XMLRayTrace.png", imageWidth, imageHeight, CHANNEL_NUM, pixels, imageWidth * CHANNEL_NUM);
}
