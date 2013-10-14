#ifndef UTILITY_H
#define UTILITY_H

#include "glm/glm.hpp"
using namespace glm;

#define M_PI 3.141592654f

bool IntersectTriangle( const vec3& orig, const vec3& dir,
					   const vec3& v0, const vec3& v1, const vec3& v2,
					   float* t, float* u, float* v );

vec3 trackball_ptov(int x, int y, int width, int height);

#endif