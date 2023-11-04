#pragma once
#include <vector>
#include "ObjData.h"

#define OFFSET_X 0.5f
#define OFFSET_Y 0.5f
#define OFFSET_Z 0.5f

#define ROTATE_X 0.1f
#define ROTATE_Y 0.1f
#define ROTATE_Z 0.1f


class Object3D {
public:
	float rotateX = 0.f;
	float rotateY = 0.f;
	float rotateZ = 0.f;

	float scaleX = 3.3f;
	float scaleY = 3.3f;
	float scaleZ = 3.3f;

	float offsetX = 0.f;
	float offsetY = 0.f;
	float offsetZ = 0.f;

	float fov;
	float near;
	float far;

	std::vector<vec3d> vertices;
	std::vector<vec3d> normals;
	std::vector<vec2d> textures;
	std::vector<triangle> triangles;

	void AddVertexGeometric(vec3d &v);
	void AddVertexTexture(vec2d &vt);
	void AddVertexNormal(vec3d &vn);
	void AddTriangle(const std::vector<int>& vertexIndices);
};
