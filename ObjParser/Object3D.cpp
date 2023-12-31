#include "Object3D.h"
#include <iostream>



void Object3D::AddVertexGeometric(vec3d& v) {
	vertices.push_back(v);
}

void Object3D::AddVertexTexture(vec2d& vt) {
	textures.push_back(vt);
}

void Object3D::AddVertexNormal(vec3d& vn) {
	normals.push_back(vn);
}

void Object3D::AddTriangle(const std::vector<int> &vertexIndices) {
	triangle tri;
	int vertexTriangleIndex = 0;
	int structIndex = 0;
	while (vertexTriangleIndex < vertexIndices.size()) {

		// Set Vertex Geometric
		int vertexIndex = vertexIndices.at(vertexTriangleIndex++);
		if (vertexIndex < 0) {
			vertexIndex += vertices.size();
		}
		if (((vertexIndex - 1) >= 0) && ((vertexIndex - 1) < vertices.size())) {
			tri.vertex[structIndex] = vertices.at(vertexIndex - 1);
		}
		else {
			std::cerr << "Object3D::AddTriangle::No vertex added for the triangle\n";
			return;
		}

		// Set Vertex Texture
		vertexIndex = vertexIndices.at(vertexTriangleIndex++);
		if (vertexIndex != EMPTY_VALUE) {
			if (vertexIndex < 0) {
				vertexIndex += textures.size();
			}
			if (((vertexIndex - 1) >= 0) && ((vertexIndex - 1) < textures.size())) {
				tri.texture[structIndex] = textures.at(vertexIndex - 1);
			}
		}

		// Set Vertex Normal
		vertexIndex = vertexIndices.at(vertexTriangleIndex++);
		if (vertexIndex != EMPTY_VALUE) {
			if (vertexIndex < 0) {
				vertexIndex += normals.size();
			}
			if (((vertexIndex - 1) >= 0) && ((vertexIndex - 1) < normals.size())) {
				tri.normal[structIndex] = normals.at(vertexIndex - 1);
			}
		}
		structIndex++;
	}
	triangles.push_back(tri);
}
