#pragma once
#include <cmath>

#define M_PI		3.14159265358979323846
#define EMPTY_VALUE	-2147483648

struct mat4x4;

struct vec2d {
	float u = 0;
	float v = 0;
	float w = 1;
};

struct vec3d {
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1; // Need a 4th term to perform sensible matrix vector multiplication

	vec3d operator+(const vec3d& other) const;
	vec3d operator-(const vec3d& other) const;
	vec3d operator*(float k) const;
	vec3d operator/(float k) const;

	static vec3d vectorNormalise(vec3d& v);
	static vec3d vectorCrossProduct(vec3d& v1, vec3d& v2);
	static vec3d matrixMultiplyByVector(const mat4x4& m, vec3d& i);
	static float vectorDotProduct(vec3d& v1, vec3d& v2);
	static float vectorLength(vec3d& v);

	static vec3d interpolateVertexes(const vec3d& v0, const vec3d& v1, const vec3d& v2, const vec3d& barycentricCoords);
	static vec3d calculateBarycentricCoordinates(const vec3d& v1, const vec3d& v2, const vec3d& v3, const vec3d& point);

	static vec3d calculateBarycentricCoordinatesArea(vec3d A, vec3d B, vec3d C, vec3d P);
};

struct mat4x4 {
	float m[4][4] = { 0 };

	mat4x4 operator*(const mat4x4& other) const;

	static mat4x4 matrixMakeRotationX(float fAngleRad);
	static mat4x4 matrixMakeRotationY(float fAngleRad);
	static mat4x4 matrixMakeRotationZ(float fAngleRad);
	static mat4x4 matrixMakeRotation(float angleX, float angleY, float angleZ);
	static mat4x4 matrixMakeTranslation(float x, float y, float z);
	static mat4x4 matrixMakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);
	static mat4x4 matrixMakeScale(float scaleX, float scaleY, float scaleZ);
	static mat4x4 matrixMakeViewPort(float width, float height);
	static mat4x4 matrixPointAt(vec3d& pos, vec3d& target, vec3d& up);
	static mat4x4 transposition(mat4x4& m);
	static float determinant3x3(float a, float b, float c, float d, float e, float f, float g, float h, float i);
	static int getMinor(const mat4x4& matrix, int row, int col);
	static mat4x4 inverse(const mat4x4& matrix, int det);
	static bool invertMatrix(const mat4x4& input, mat4x4& output);
	static float	  determinant4x4(const mat4x4& matrix);
};

struct triangle {
	vec3d vertex[3];
	vec3d normal[3];
	vec3d texture[3];
};





