#pragma once
#include "ObjData.h"
#include <cmath>

#define M_PI 3.14159265358979323846

class mat4x4 {
public:

	float m[4][4] = { 0 };

	mat4x4 operator*(const mat4x4& other) const;

	// Matrices
	static mat4x4 MatrixMultiplyByMatrix(mat4x4& m1, mat4x4& m2);
	static mat4x4 MatrixMakeRotationX(float fAngleRad);
	static mat4x4 MatrixMakeRotationY(float fAngleRad);
	static mat4x4 MatrixMakeRotationZ(float fAngleRad);
	static mat4x4 MatrixMakeRotation(float angleX, float angleY, float angleZ);
	static mat4x4 MatrixMakeTranslation(float x, float y, float z);
	static mat4x4 MatrixMakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);
	static mat4x4 MatrixMakeScale(float scaleX, float scaleY, float scaleZ);
	static mat4x4 MatrixMakeViewPort(float width, float height);
	static mat4x4 MatrixPointAt(vec3d& pos, vec3d& target, vec3d& up);
	static mat4x4 Transposition(mat4x4& m);

	// Vectors
	static vec3d  MatrixMultiplyByVector(mat4x4& m, vec3d& i);
	static vec3d Vector_Add(vec3d& v1, vec3d& v2);
	static vec3d Vector_Sub(vec3d& v1, vec3d& v2);
	static vec3d Vector_Mul(vec3d& v1, float k);
	static vec3d Vector_Div(vec3d& v1, float k);
	static vec3d Vector_Normalise(vec3d& v);
	static vec3d Vector_CrossProduct(vec3d& v1, vec3d& v2);
	static float Vector_DotProduct(vec3d& v1, vec3d& v2);
	static float Vector_Length(vec3d& v);
};

