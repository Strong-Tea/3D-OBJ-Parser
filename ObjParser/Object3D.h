#pragma once
#include <vector>
#include "3DGeometry.h"

#define SPEED_MOVE   20
#define SPEED_ROTATE 5
#define SPEED_MOVE_LIGHT 5


class Object3D {
public:
	float rotateX = 0.f;
	float rotateY = 0.f;
	float rotateZ = 0.f;

	float scaleX = 5.0f;
	float scaleY = 5.0f;
	float scaleZ = 5.0f;

	float offsetX = 0.f;
	float offsetY = 0.f;
	float offsetZ = 0.f;

	std::vector<vec3d>    vertices;
	std::vector<vec3d>    normals;
	std::vector<vec3d>    textures;
	std::vector<triangle> triangles;

	void AddVertexGeometric(vec3d &v);
	void AddVertexTexture(vec3d &vt);
	void AddVertexNormal(vec3d &vn);
	void AddTriangle(const std::vector<int>& vertexIndices);

	float lambertLighting(vec3d& normal, vec3d& lightDirection);
	float flatShading(vec3d& normal, vec3d& lightDirection, vec3d& viewerDirection, float ambient);
};
