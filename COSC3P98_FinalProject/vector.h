#ifndef vector.h
#define vector.h

#include <cmath>
class vector
{
public:
		vec3() : e{ 0,0,0 } {}
		vec3(float e0, float e1, float e2) : e{ e0, e1, e2 } {}

		float getX() const { return e[0]; }
		float getY() const { return e[1]; }
		float getZ() const { return e[2]; }

		vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
		double operator[](int i) const { return e[i]; }

		vec3& operator+=(const vec3& v)//adds vector v to this vector
		{
			e[0] += v.e[0];
			e[1] += v.e[1];
			e[2] += v.e[2];
			return *this;
		}

		vec3& operator*=(const double s)//Multiplies this vector by some scalar s
		{
			e[0] *= s;
			e[1] *= s;
			e[2] *= s;
			return *this;
		}

		vec3& operator/=(const double s)
		{
			return *this *= 1 / s;
		}


		float dot(const vec3& v)//returns dot product of this vector and some vector v
		{
			return (e[0] * v.e[0]) + (e[1] * v.e[1]) + (e[2] * v.e[2]);
		}

		vec3 cross(const vec3& v)//returns cross product of this vector and some vector v
		{
			double tempX = (e[1] * v.e[2]) - (e[2] * v.e[1]);
			double tempY = (e[2] * v.e[0]) - (e[0] * v.e[2]);
			double tempZ = (e[0] * v.e[1]) - (e[1] * v.e[0]);
		}


		double length() const {
			return sqrt((e[0] * e[0]) + (e[1] * e[1]) + (e[2] * e[2]));
		}
		vec3 unitVector()
		{
			return this / this.length();

		}

};

#endif