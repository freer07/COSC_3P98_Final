// Alexander Freer 6452551
// Yanis Souiki 6284392

//This is a collection of helper functions for the vec3 object primarly implemented from ?Ray Tracing in One Weekend.? raytracing.github.io/books/RayTracingInOneWeekend.html  
#pragma once
#include <glm/glm.hpp>
#include <cstdlib>
using namespace glm;

inline double random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max - min) * random_double();
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u[0] + v[0], u[1] + v[1], u[2] + v[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u[0] - v[0], u[1] - v[1], u[2] - v[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u[0] * v[0], u[1] * v[1], u[2] * v[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t * v[0], t * v[1], t * v[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1 / t) * v;
}

inline static vec3 random() {
    return vec3(random_double(), random_double(), random_double());
}

inline static vec3 random(double min, double max) {
    return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

vec3 randomUnitVec3() {
    while (true) {
        vec3 p = random(-1, 1);
        if (length(p)*length(p) >= 1) continue;
        return normalize(p);
    }
}

bool nearZero(vec3 v) {
    // Return true if the vector is close to zero in all dimensions.
    const auto s = 1e-8;
    return (fabs(v[0]) < s) && (fabs(v[1]) < s) && (fabs(v[2]) < s);
}

vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

vec3 refract(const vec3& u, const vec3& n, double eta) {
    auto cos = fmin(dot(-u, n), 1.0);
    vec3 perp = eta * (u + (cos * n));
    vec3 para = -sqrt(fabs(1.0 - ((double)length(perp) * (double)length(perp)))) * n;
    return perp + para;
}