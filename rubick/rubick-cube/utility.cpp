#include "utility.h"

//--------------------------------------------------------------------------------------
// Given a ray origin (orig) and direction (dir), and three vertices of a triangle, this
// function returns TRUE and the interpolated texture coordinates if the ray intersects 
// the triangle
//--------------------------------------------------------------------------------------

#define EPSILON 0.0001f
#define CULL_FACE

bool IntersectTriangle( const vec3& orig, const vec3& dir,
					   const vec3& v0, const vec3& v1, const vec3& v2,
					   float* t, float* u, float* v )
{
	// Find vectors for two edges sharing vert0
	vec3 edge1 = v1 - v0;
	vec3 edge2 = v2 - v0;

	// Begin calculating determinant - also used to calculate U parameter
	vec3 pvec = cross(dir, edge2);

	// If determinant is near zero, ray lies in plane of triangle
	float det = dot(edge1, pvec);

#ifdef CULL_FACE
	if(det < EPSILON)
		return false;
#else
	if(det > -EPSILON && det < EPSILON)
		return false;
#endif

	vec3 tvec = orig - v0;
	// Calculate U parameter and test bounds
	*u = dot(tvec, pvec);
	if( *u < 0.0f || *u > det )
		return false;

	// Prepare to test V parameter
	vec3 qvec = cross(tvec, edge1);

	// Calculate V parameter and test bounds
	*v = dot(dir, qvec);
	if( *v < 0.0f || *u + *v > det )
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	*t = dot(edge2, qvec);
	float fInvDet = 1.0f / det;
	*t *= fInvDet;
	*u *= fInvDet;
	*v *= fInvDet;

	return true;
}


vec3 trackball_ptov(int x, int y, int width, int height)
{
	/* project x,y onto a hemisphere centered within width, height , note z is up here*/
	vec3 v;
	v.x = (2.0f*x - width) / width;
	v.y = (height - 2.0f*y) / height;    
	float d = sqrt(v.x*v.x + v.y*v.y);
	v.z = cos(M_PI/2.0f * ((d < 1.0f) ? d : 1.0f));

	return normalize(v);
}