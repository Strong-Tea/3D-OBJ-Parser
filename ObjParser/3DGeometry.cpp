#include "3DGeometry.h"
#include <algorithm>
#include <iostream>


mat4x4 mat4x4::operator*(const mat4x4& other) const
{
	mat4x4 result;
	for (int c = 0; c < 4; c++) {
		for (int r = 0; r < 4; r++) {
			result.m[r][c] = m[r][0] * other.m[0][c] + m[r][1] * other.m[1][c] + m[r][2] * other.m[2][c] + m[r][3] * other.m[3][c];
		}
	}
	return result;
}


mat4x4 mat4x4::matrixMakeRotationY(float fAngleRad) {
	mat4x4 matRotY;
	matRotY.m[0][0] = cosf(fAngleRad);
	matRotY.m[0][2] = -sinf(fAngleRad);
	matRotY.m[1][1] = 1.f;
	matRotY.m[2][0] = sinf(fAngleRad);
	matRotY.m[2][2] = cosf(fAngleRad);
	matRotY.m[3][3] = 1.f;

	return matRotY;
}


mat4x4 mat4x4::matrixMakeRotationX(float fAngleRad) {
	mat4x4 matRotX;
	matRotX.m[0][0] = 1.f;
	matRotX.m[1][1] = cosf(fAngleRad);
	matRotX.m[1][2] = sinf(fAngleRad);
	matRotX.m[2][1] = -sinf(fAngleRad);
	matRotX.m[2][2] = cosf(fAngleRad);
	matRotX.m[3][3] = 1.f;

	return matRotX;
}

mat4x4 mat4x4::matrixMakeRotationZ(float fAngleRad) {
	mat4x4 matRotZ;
	matRotZ.m[0][0] = cosf(fAngleRad);
	matRotZ.m[0][1] = sinf(fAngleRad);
	matRotZ.m[1][0] = -sinf(fAngleRad);
	matRotZ.m[1][1] = cosf(fAngleRad);
	matRotZ.m[2][2] = 1.f;
	matRotZ.m[3][3] = 1.f;

	return matRotZ;
}


mat4x4 mat4x4::matrixMakeRotation(float angleX, float angleY, float angleZ) {
	mat4x4 matRotX = matrixMakeRotationX(angleY);
	mat4x4 matRotY = matrixMakeRotationY(angleX);
	mat4x4 matRotZ = matrixMakeRotationZ(angleZ);
	mat4x4 matRot = matRotZ * matRotX * matRotY;
	return matRot;
}


mat4x4 mat4x4::matrixMakeTranslation(float x, float y, float z) {
	mat4x4 matrix;
	matrix.m[0][0] = 1.f;
	matrix.m[3][0] = x;
	matrix.m[1][1] = 1.f;
	matrix.m[3][1] = y;
	matrix.m[2][2] = 1.f;
	matrix.m[3][2] = z;
	matrix.m[3][3] = 1.f;
	return matrix;
}

mat4x4 mat4x4::matrixMakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar) {
	mat4x4 mat;
	float fFovRad = 1.f / tanf(fFovDegrees * 0.5f / 180.0f * M_PI);
	mat.m[0][0] = 1.f / fAspectRatio * fFovRad;
	mat.m[1][1] = fFovRad;
	mat.m[2][2] = fFar / (fNear - fFar);
	mat.m[3][2] = (fNear * fFar) / (fNear - fFar);
	mat.m[2][3] = -1.0f;
	return mat;
}


mat4x4 mat4x4::matrixMakeScale(float scaleX, float scaleY, float scaleZ) {
	mat4x4 matrix;
	matrix.m[0][0] = scaleX;
	matrix.m[1][1] = scaleY;
	matrix.m[2][2] = scaleZ;
	matrix.m[3][3] = 1.0f;
	return matrix;
}


mat4x4 mat4x4::matrixMakeViewPort(float width, float height) {
	mat4x4 matrix;
	float xmin = 0.f;
	float ymin = 0.f;
	matrix.m[0][0] = width * 0.5f;
	matrix.m[3][0] = xmin + width * 0.5f;
	matrix.m[1][1] = -height * 0.5;
	matrix.m[3][1] = ymin + height * 0.5f;
	matrix.m[2][2] = 1;
	matrix.m[3][3] = 1;
	return matrix;
}


mat4x4 mat4x4::matrixPointAt(vec3d& eye, vec3d& target, vec3d& up) {
	vec3d sub = eye - target;
	vec3d Z = vec3d::vectorNormalise(sub);

	vec3d cross = vec3d::vectorCrossProduct(up, Z);
	vec3d X = vec3d::vectorNormalise(cross);

	vec3d Y = vec3d::vectorCrossProduct(Z, X);

	mat4x4 result;

	result.m[0][0] = X.x;
	result.m[1][0] = X.y;
	result.m[2][0] = X.z;
	result.m[3][0] = -vec3d::vectorDotProduct(X, eye);

	result.m[0][1] = Y.x;
	result.m[1][1] = Y.y;
	result.m[2][1] = Y.z;
	result.m[3][1] = -vec3d::vectorDotProduct(Y, eye);

	result.m[0][2] = Z.x;
	result.m[1][2] = Z.y;
	result.m[2][2] = Z.z;
	result.m[3][2] = -vec3d::vectorDotProduct(Z, eye);

	result.m[0][3] = 0.f;
	result.m[1][3] = 0.f;
	result.m[2][3] = 0.f;
	result.m[3][3] = 1.f;
	return result;
}


mat4x4 mat4x4::transposition(mat4x4& mat) {
	mat4x4 result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.m[i][j] = mat.m[j][i];
		}
	}
	return result;
}

float mat4x4::determinant3x3(float a, float b, float c, float d, float e, float f, float g, float h, float i) {
	return a * (e * i - f * h) - b * (d * i - f * g) + c * (d * h - e * g);
}


int mat4x4::getMinor(const mat4x4& matrix, int row, int col)
{
	mat4x4 minor;
	for (int i = 0, r = 0; i < 4; ++i) {
		if (i == row) continue;

		for (int j = 0, c = 0; j < 4; ++j) {
			if (j == col) continue;

			minor.m[r][c] = matrix.m[i][j];
			++c;
		}

		++r;
	}

	int det = mat4x4::determinant3x3(minor.m[0][0], minor.m[0][1], minor.m[0][2],
		minor.m[1][0], minor.m[1][1], minor.m[1][2],
		minor.m[2][0], minor.m[2][1], minor.m[2][2]);

	return det;
}

mat4x4 mat4x4::inverse(const mat4x4& matrix, int det)
{
	mat4x4 invMatrix;
	mat4x4 cof;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			double minor = getMinor(matrix, i, j);
			cof.m[i][j] = ((i + j) % 2 == 0 ? 1 : -1) * minor;
		}
	}

	cof = mat4x4::transposition(cof);

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			invMatrix.m[j][i] = cof.m[i][j] / det;
		}
	}

	return invMatrix;
}

float mat4x4::determinant4x4(const mat4x4& mat) {
	return
		mat.m[0][3] * mat.m[1][2] * mat.m[2][1] * mat.m[3][0] - mat.m[0][2] * mat.m[1][3] * mat.m[2][1] * mat.m[3][0] -
		mat.m[0][3] * mat.m[1][1] * mat.m[2][2] * mat.m[3][0] + mat.m[0][1] * mat.m[1][3] * mat.m[2][2] * mat.m[3][0] +
		mat.m[0][2] * mat.m[1][1] * mat.m[2][3] * mat.m[3][0] - mat.m[0][1] * mat.m[1][2] * mat.m[2][3] * mat.m[3][0] -
		mat.m[0][3] * mat.m[1][2] * mat.m[2][0] * mat.m[3][1] + mat.m[0][2] * mat.m[1][3] * mat.m[2][0] * mat.m[3][1] +
		mat.m[0][3] * mat.m[1][0] * mat.m[2][2] * mat.m[3][1] - mat.m[0][0] * mat.m[1][3] * mat.m[2][2] * mat.m[3][1] -
		mat.m[0][2] * mat.m[1][0] * mat.m[2][3] * mat.m[3][1] + mat.m[0][0] * mat.m[1][2] * mat.m[2][3] * mat.m[3][1] +
		mat.m[0][3] * mat.m[1][1] * mat.m[2][0] * mat.m[3][2] - mat.m[0][1] * mat.m[1][3] * mat.m[2][0] * mat.m[3][2] -
		mat.m[0][3] * mat.m[1][0] * mat.m[2][1] * mat.m[3][2] + mat.m[0][0] * mat.m[1][3] * mat.m[2][1] * mat.m[3][2] +
		mat.m[0][1] * mat.m[1][0] * mat.m[2][3] * mat.m[3][2] - mat.m[0][0] * mat.m[1][1] * mat.m[2][3] * mat.m[3][2] -
		mat.m[0][2] * mat.m[1][1] * mat.m[2][0] * mat.m[3][3] + mat.m[0][1] * mat.m[1][2] * mat.m[2][0] * mat.m[3][3] +
		mat.m[0][2] * mat.m[1][0] * mat.m[2][1] * mat.m[3][3] - mat.m[0][0] * mat.m[1][2] * mat.m[2][1] * mat.m[3][3] -
		mat.m[0][1] * mat.m[1][0] * mat.m[2][2] * mat.m[3][3] + mat.m[0][0] * mat.m[1][1] * mat.m[2][2] * mat.m[3][3];
}

bool mat4x4::invertMatrix(const mat4x4& input, mat4x4& output) {
	double inv[16], det;
	int i;

	inv[0] = input.m[1][1] * input.m[2][2] * input.m[3][3] -
		input.m[1][1] * input.m[2][3] * input.m[3][2] -
		input.m[2][1] * input.m[1][2] * input.m[3][3] +
		input.m[2][1] * input.m[1][3] * input.m[3][2] +
		input.m[3][1] * input.m[1][2] * input.m[2][3] -
		input.m[3][1] * input.m[1][3] * input.m[2][2];

	inv[4] = -input.m[1][0] * input.m[2][2] * input.m[3][3] +
		input.m[1][0] * input.m[2][3] * input.m[3][2] +
		input.m[2][0] * input.m[1][2] * input.m[3][3] -
		input.m[2][0] * input.m[1][3] * input.m[3][2] -
		input.m[3][0] * input.m[1][2] * input.m[2][3] +
		input.m[3][0] * input.m[1][3] * input.m[2][2];

	inv[8] = input.m[1][0] * input.m[2][1] * input.m[3][3] -
		input.m[1][0] * input.m[2][3] * input.m[3][1] -
		input.m[2][0] * input.m[1][1] * input.m[3][3] +
		input.m[2][0] * input.m[1][3] * input.m[3][1] +
		input.m[3][0] * input.m[1][1] * input.m[2][3] -
		input.m[3][0] * input.m[1][3] * input.m[2][1];

	inv[12] = -input.m[1][0] * input.m[2][1] * input.m[3][2] +
		input.m[1][0] * input.m[2][2] * input.m[3][1] +
		input.m[2][0] * input.m[1][1] * input.m[3][2] -
		input.m[2][0] * input.m[1][2] * input.m[3][1] -
		input.m[3][0] * input.m[1][1] * input.m[2][2] +
		input.m[3][0] * input.m[1][2] * input.m[2][1];

	inv[1] = -input.m[0][1] * input.m[2][2] * input.m[3][3] +
		input.m[0][1] * input.m[2][3] * input.m[3][2] +
		input.m[2][1] * input.m[0][2] * input.m[3][3] -
		input.m[2][1] * input.m[0][3] * input.m[3][2] -
		input.m[3][1] * input.m[0][2] * input.m[2][3] +
		input.m[3][1] * input.m[0][3] * input.m[2][2];

	inv[5] = input.m[0][0] * input.m[2][2] * input.m[3][3] -
		input.m[0][0] * input.m[2][3] * input.m[3][2] -
		input.m[2][0] * input.m[0][2] * input.m[3][3] +
		input.m[2][0] * input.m[0][3] * input.m[3][2] +
		input.m[3][0] * input.m[0][2] * input.m[2][3] -
		input.m[3][0] * input.m[0][3] * input.m[2][2];

	inv[9] = -input.m[0][0] * input.m[2][1] * input.m[3][3] +
		input.m[0][0] * input.m[2][3] * input.m[3][1] +
		input.m[2][0] * input.m[0][1] * input.m[3][3] -
		input.m[2][0] * input.m[0][3] * input.m[3][1] -
		input.m[3][0] * input.m[0][1] * input.m[2][3] +
		input.m[3][0] * input.m[0][3] * input.m[2][1];

	inv[13] = input.m[0][0] * input.m[2][1] * input.m[3][2] -
		input.m[0][0] * input.m[2][2] * input.m[3][1] -
		input.m[2][0] * input.m[0][1] * input.m[3][2] +
		input.m[2][0] * input.m[0][2] * input.m[3][1] +
		input.m[3][0] * input.m[0][1] * input.m[2][2] -
		input.m[3][0] * input.m[0][2] * input.m[2][1];

	inv[2] = input.m[0][1] * input.m[1][2] * input.m[3][3] -
		input.m[0][1] * input.m[1][3] * input.m[3][2] -
		input.m[1][1] * input.m[0][2] * input.m[3][3] +
		input.m[1][1] * input.m[0][3] * input.m[3][2] +
		input.m[3][1] * input.m[0][2] * input.m[1][3] -
		input.m[3][1] * input.m[0][3] * input.m[1][2];

	inv[6] = -input.m[0][0] * input.m[1][2] * input.m[3][3] +
		input.m[0][0] * input.m[1][3] * input.m[3][2] +
		input.m[1][0] * input.m[0][2] * input.m[3][3] -
		input.m[1][0] * input.m[0][3] * input.m[3][2] -
		input.m[3][0] * input.m[0][2] * input.m[1][3] +
		input.m[3][0] * input.m[0][3] * input.m[1][2];

	inv[10] = input.m[0][0] * input.m[1][1] * input.m[3][3] -
		input.m[0][0] * input.m[1][3] * input.m[3][1] -
		input.m[1][0] * input.m[0][1] * input.m[3][3] +
		input.m[1][0] * input.m[0][3] * input.m[3][1] +
		input.m[3][0] * input.m[0][1] * input.m[1][3] -
		input.m[3][0] * input.m[0][3] * input.m[1][1];

	inv[14] = -input.m[0][0] * input.m[1][1] * input.m[3][2] +
		input.m[0][0] * input.m[1][2] * input.m[3][1] +
		input.m[1][0] * input.m[0][1] * input.m[3][2] -
		input.m[1][0] * input.m[0][2] * input.m[3][1] -
		input.m[3][0] * input.m[0][1] * input.m[1][2] +
		input.m[3][0] * input.m[0][2] * input.m[1][1];

	inv[3] = -input.m[0][1] * input.m[1][2] * input.m[2][3] +
		input.m[0][1] * input.m[1][3] * input.m[2][2] +
		input.m[1][1] * input.m[0][2] * input.m[2][3] -
		input.m[1][1] * input.m[0][3] * input.m[2][2] -
		input.m[2][1] * input.m[0][2] * input.m[1][3] +
		input.m[2][1] * input.m[0][3] * input.m[1][2];

	inv[7] = input.m[0][0] * input.m[1][2] * input.m[2][3] -
		input.m[0][0] * input.m[1][3] * input.m[2][2] -
		input.m[1][0] * input.m[0][2] * input.m[2][3] +
		input.m[1][0] * input.m[0][3] * input.m[2][2] +
		input.m[2][0] * input.m[0][2] * input.m[1][3] -
		input.m[2][0] * input.m[0][3] * input.m[1][2];

	inv[11] = -input.m[0][0] * input.m[1][1] * input.m[2][3] +
		input.m[0][0] * input.m[1][3] * input.m[2][1] +
		input.m[1][0] * input.m[0][1] * input.m[2][3] -
		input.m[1][0] * input.m[0][3] * input.m[2][1] -
		input.m[2][0] * input.m[0][1] * input.m[1][3] +
		input.m[2][0] * input.m[0][3] * input.m[1][1];

	inv[15] = input.m[0][0] * input.m[1][1] * input.m[2][2] -
		input.m[0][0] * input.m[1][2] * input.m[2][1] -
		input.m[1][0] * input.m[0][1] * input.m[2][2] +
		input.m[1][0] * input.m[0][2] * input.m[2][1] +
		input.m[2][0] * input.m[0][1] * input.m[1][2] -
		input.m[2][0] * input.m[0][2] * input.m[1][1];

	det = input.m[0][0] * inv[0] + input.m[0][1] * inv[4] + input.m[0][2] * inv[8] + input.m[0][3] * inv[12];

	if (det == 0)
		return false;

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		output.m[i / 4][i % 4] = static_cast<float>(inv[i] * det);

	return true;
}


vec3d vec3d::operator+(const vec3d& other) const {
	return { x + other.x, y + other.y, z + other.z };
}


vec3d vec3d::operator-(const vec3d& other) const {
	return { x - other.x, y - other.y, z - other.z };
}


vec3d vec3d::operator*(float k) const {
	return { x * k, y * k, z * k };
}


vec3d vec3d::operator/(float k) const {
	return { x / k, y / k, z / k };
}


vec3d vec3d::matrixMultiplyByVector(const mat4x4& m, vec3d& i) {
	vec3d outVec;
	outVec.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
	outVec.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
	outVec.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
	float w  = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

	if (w != 0.0f)
	{
		outVec.x /= w; outVec.y /= w; outVec.z /= w;
	}

	return outVec;
}


vec3d vec3d::interpolateVertexes(const vec3d& v0, const vec3d& v1, const vec3d& v2, const vec3d& barycentricCoords) {
	vec3d interpolatedVertexes;
	interpolatedVertexes.x = barycentricCoords.x * v0.x + barycentricCoords.y * v1.x + barycentricCoords.z * v2.x;
	interpolatedVertexes.y = barycentricCoords.x * v0.y + barycentricCoords.y * v1.y + barycentricCoords.z * v2.y;
	interpolatedVertexes.z = barycentricCoords.x * v0.z + barycentricCoords.y * v1.z + barycentricCoords.z * v2.z;

	if (interpolatedVertexes.x < 0 || interpolatedVertexes.y < 0) {
		int a = 10;
	}
	return interpolatedVertexes;
}


vec3d vec3d::calculateBarycentricCoordinates(const vec3d& v1, const vec3d& v2, const vec3d& v3, const vec3d& point) {

	float det = (v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y);
	float alpha = ((v2.y - v3.y) * (point.x - v3.x) + (v3.x - v2.x) * (point.y - v3.y)) / det;
	float beta = ((v3.y - v1.y) * (point.x - v3.x) + (v1.x - v3.x) * (point.y - v3.y)) / det;
	float gamma = 1.0f - alpha - beta;
	return { alpha,beta,gamma,1 };
	/*
				var barycentric = new Vector3();
			var det = (v2.Y - v3.Y) * (v1.X - v3.X) + (v3.X - v2.X) * (v1.Y - v3.Y);
			barycentric.X = ((v2.Y - v3.Y) * (p.X - v3.X) + (v3.X - v2.X) * (p.Y - v3.Y)) / det;
			barycentric.Y = ((v3.Y - v1.Y) * (p.X - v3.X) + (v1.X - v3.X) * (p.Y - v3.Y)) / det;
			barycentric.Z = 1.0f - barycentric.X - barycentric.Y;
			return barycentric;
	*/
	//float det_ABC = v1.x * (v2.y * v3.z - v3.y * v2.z) - v2.x * (v1.y * v3.z - v3.y * v1.z) + v3.x * (v1.y * v2.z - v2.y * v1.z);
	//float alpha = ((point.x * (v2.y * v3.z - v3.y * v2.z))	  - (v2.x * (point.y * v3.z - v3.y * point.z)) + (v3.x * (point.y * v2.z - v2.y * point.z))) / det_ABC;
	//float beta  = ((v1.x * (point.y * v3.z - v3.y * point.z)) - (point.x * (v1.y * v3.z - v3.y * v1.z))	   + (v3.x * (v1.y * point.z - point.y * v1.z))) / det_ABC;
	//float gamma = ((v1.x * (v2.y * point.z - point.y * v2.z)) - (v2.x * (v1.y * point.z - point.y * v1.z)) + (point.x * (v1.y * v2.z - v2.y * v1.z)))	 / det_ABC;
	//return { abs(alpha), abs(beta), gamma };
}

vec3d vec3d::calculateBarycentricCoordinatesArea(vec3d A, vec3d B, vec3d C, vec3d P) {

	vec3d BA = B - A;
	vec3d CA = C - A;
	vec3d normal = vectorCrossProduct(BA, CA);
	normal = vectorNormalise(normal);

	float normalLength = vectorLength(normal);
	if (std::abs(normalLength) < 1e-6) {

		return { 1 / 3, 1 / 3, 1 / 3 };
	}

	float area_ABC = 0.5 * std::abs(A.x * (B.y * C.z - C.y * B.z) + B.x * (C.y * A.z - A.y * C.z) + C.x * (A.y * B.z - B.y * A.z));

	float area_BCP = 0.5 * std::abs(P.x * (B.y * C.z - C.y * B.z) + B.x * (C.y * P.z - P.y * C.z) + C.x * (P.y * B.z - B.y * P.z));
	float area_CAP = 0.5 * std::abs(A.x * (P.y * C.z - C.y * P.z) + P.x * (C.y * A.z - A.y * C.z) + C.x * (A.y * P.z - P.y * A.z));
	float area_ABP = 0.5 * std::abs(A.x * (B.y * P.z - P.y * B.z) + B.x * (P.y * A.z - A.y * P.z) + P.x * (A.y * B.z - B.y * A.z));
	if (area_ABC == 0) {
		return { 1 / 3, 1 / 3, 1 / 3 };
	}
	float alpha = area_BCP / area_ABC;
	float beta = area_CAP / area_ABC;
	float gamma = 1 - (alpha + beta);//;area_ABP / area_ABC;

	return { alpha, beta, gamma };
}


float vec3d::vectorDotProduct(vec3d& v1, vec3d& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}


float vec3d::vectorLength(vec3d& v) {
	return sqrtf(vectorDotProduct(v, v));
}


vec3d vec3d::vectorNormalise(vec3d& v) {
	float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (length != 0.0f) {
		float invLength = 1.0f / length;
		vec3d vec = { v.x * invLength, v.y * invLength, v.z * invLength, 1.f };
		return vec;
	}
	else {
		vec3d vec = { 0.f, 0.f, 0.f, 1.f };
		return vec;
	}
}


vec3d vec3d::vectorCrossProduct(vec3d& v1, vec3d& v2) {
	vec3d v;
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}