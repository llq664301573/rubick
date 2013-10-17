#include "Picker.h"
#include "constant.h"
#include "Cube.h"
#include "Face.h"
#include "global.h"
#include "utility.h"



Picker::Picker():angle(0)
{
	face[0] = face[1] = NULL;
}

bool Picker::tryRotary()
{
	if(face[0] == NULL || face[1] == NULL)
		return false;

	if(face[0] == face[1])
	{
		face[0] = face[1] = NULL;
		return false;
	}
	else
	{
		const vec3 axis[] = {vec3(1, 0, 0), vec3(0, 1, 0), vec3(0, 0, 1)};
		const vec3 size = Cube::size;
		bool doubling[] = {size.y != size.z, size.x != size.z, size.x != size.y};
		float vec3::* fields[] = {&vec3::x, &vec3::y, &vec3::z};

		const vec3& v0 = face[0]->center;
		const vec3& v1 = face[1]->center;
		vec3 dir = normalize(v1 - v0);

		int type = (v0.x == v1.x) + (v0.y == v1.y) + (v0.z == v1.z);
		if(face[0]->index == face[1]->index)//同一侧
		{
			if(type != 2)
			{
				face[0] = face[1] = NULL;
				return false;
			}

			int rotationAxis[][3] = {
				0xff, -3, 2,
				3, 0xff, -1,
				-2, 1, 0xff
			};

			int index = face[0]->index / 2;

			for (int i=0;i<3;i++)
			{
				if(dir == axis[i] || dir == -axis[i])
				{
					int rotationAxisID = rotationAxis[index][i];

					bool reverse = false;
					if(rotationAxisID < 0)
						reverse = !reverse;
					if(dir == -axis[i])
						reverse = !reverse;
					if(face[0]->index & 1)
						reverse = !reverse;

					rotationAxisID = abs(rotationAxisID);
					rotationAxisID--;

					rotation.pivot = vec3();
					rotation.pivot.*fields[rotationAxisID] = v0.*fields[rotationAxisID];

					rotation.axis = axis[rotationAxisID];
					if(reverse)
						rotation.axis = -rotation.axis;

					angle = doubling[rotationAxisID] ? 180 : 90;

					return true;
				}
			}
		}
		else
		{
			if(type != 1)
			{
				face[0] = face[1] = NULL;
				return false;
			}

			for (int i=0;i<3;i++)
			{
				if(dot(dir, axis[i]) == 0)
				{
					rotation.pivot = vec3();
					rotation.pivot.*fields[i] = v0.*fields[i];

					float result = dot(axis[i], cross(v0 - rotation.pivot, v1 - rotation.pivot));
					if(result > 0)
						rotation.axis = axis[i];
					else if(result < 0)
						rotation.axis = -axis[i];

					angle = doubling[i] ? 180 : 90;

					return true;
				}
			}
		}
	}

	face[0] = face[1] = NULL;
	return false;
}

void Picker::picking(double xpos, double ypos, int width, int height)
{
	if(face[0] == NULL || face[1] == NULL)
	{
		vec4 v;
		v.x = (2.0f*xpos - width) / width;
		v.y = (height - 2.0f*ypos) / height;  
		v.z = -1.0f;//[-1, 1] 由近及远
		v.w = 1;

		// Transform the screen space Picker ray into 3D space
		//v = inverse(projection * view) * v;
		v = inverse(projection) * v;
		v = inverse(view) * v;

		vec3 vPickRayOrig = vec3(v.x / v.w, v.y / v.w, v.z / v.w);//屏幕鼠标点世界空间坐标
		vec3 vPickRayDir = vPickRayOrig - eye;

		for (int i=0;i<faces.size();i++)
		{
			Face *face = faces[i];
			Cube *cube = face->cube;
			const float *pbuffer = vertex_buffer_data + face->index * 18;//3*3*2

			for (int i=0;i<2;i++)//每个面两个三角形
			{
				vec3 vertex[3];
				for (int k=0;k<3;k++)//三角形上三点
				{
					vec4 v = vec4(pbuffer[i*9+k*3], pbuffer[i*9+k*3+1], pbuffer[i*9+k*3+2], 1);
					v = camera * scroll * cube->modelMatrix * v;//变换到世界空间

					vertex[k] = vec3(v.x, v.y, v.z);
				}

				float t, u, v;
				if(IntersectTriangle(vPickRayOrig, vPickRayDir, vertex[0], vertex[1], vertex[2], &t, &u, &v))
				{
					if(this->face[0] == NULL)
						this->face[0] = face;
					else
						this->face[1] = face;
					
					return ;
				}
			}
		}
	}
}