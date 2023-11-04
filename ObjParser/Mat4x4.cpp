#include "Mat4x4.h"


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

vec3d mat4x4::MatrixMultiplyByVector(mat4x4& m, vec3d& i)
{
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

mat4x4 mat4x4::MatrixMultiplyByMatrix(mat4x4& m1, mat4x4& m2)
{
	mat4x4 matrix;
	for (int c = 0; c < 4; c++)
		for (int r = 0; r < 4; r++)
			matrix.m[r][c] = m1.m[r][0] * m2.m[0][c] + m1.m[r][1] * m2.m[1][c] + m1.m[r][2] * m2.m[2][c] + m1.m[r][3] * m2.m[3][c];
	return matrix;
}

mat4x4 mat4x4::MatrixMakeRotationY(float fAngleRad)
{
	mat4x4 matRotX;
	matRotX.m[0][0] = 1.f;
	matRotX.m[1][1] = cos(fAngleRad);
	matRotX.m[1][2] = -sin(fAngleRad);
	matRotX.m[2][1] = sin(fAngleRad);
	matRotX.m[2][2] = cos(fAngleRad);
	matRotX.m[3][3] = 1.f;
	return matRotX;
}

mat4x4 mat4x4::MatrixMakeRotation(float angleX, float angleY, float angleZ)
{
	mat4x4 matRotX = MatrixMakeRotationX(angleX);
	mat4x4 matRotY = MatrixMakeRotationY(angleY);
	mat4x4 matRotZ = MatrixMakeRotationZ(angleZ);
	mat4x4 matRot = MatrixMultiplyByMatrix(matRotX, matRotY);
	matRot = MatrixMultiplyByMatrix(matRot, matRotZ);
	return matRot;
}

mat4x4 mat4x4::MatrixMakeRotationX(float fAngleRad)
{
	mat4x4 matRotY;
	matRotY.m[0][0] = cos(fAngleRad);
	matRotY.m[0][2] = sin(fAngleRad);
	matRotY.m[1][1] = 1.f;
	matRotY.m[2][0] = -sin(fAngleRad);
	matRotY.m[2][2] = cos(fAngleRad);
	matRotY.m[3][3] = 1.f;

	return matRotY;
}

mat4x4 mat4x4::MatrixMakeRotationZ(float fAngleRad)
{
	mat4x4 matRotZ;
	matRotZ.m[0][0] = cosf(fAngleRad);
	matRotZ.m[0][1] = -sinf(fAngleRad);
	matRotZ.m[1][0] = sinf(fAngleRad);
	matRotZ.m[1][1] = cosf(fAngleRad);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;
	return matRotZ;
}

mat4x4 mat4x4::MatrixMakeTranslation(float x, float y, float z)
{
	mat4x4 matrix;
	matrix.m[0][0] = 1.f;
	matrix.m[0][3] = x;
	matrix.m[1][1] = 1.f;
	matrix.m[1][3] = y;
	matrix.m[2][2] = 1.f;
	matrix.m[2][3] = z;
	matrix.m[3][3] = 1.f;
	return matrix;
}

mat4x4 mat4x4::MatrixMakeProjection(float fFovDegrees, float fAspectRatio, float fNear, float fFar)
{
	mat4x4 mat;
	float fFovRad = 1.0f / tanf(fFovDegrees * 0.5f / 180.0f * M_PI);
	mat.m[0][0] = 1.f / fAspectRatio * fFovRad;
	mat.m[1][1] = fFovRad;
	mat.m[2][2] = fFar / (fNear - fFar);
	mat.m[2][3] = (fNear * fFar) / (fNear - fFar);
	mat.m[3][2] = -1.0f;
	return mat;
}

mat4x4 mat4x4::MatrixMakeScale(float scaleX, float scaleY, float scaleZ) {
	mat4x4 matrix;
	matrix.m[0][0] = scaleX;
	matrix.m[1][1] = scaleY;
	matrix.m[2][2] = scaleZ;
	matrix.m[3][3] = 1.0f;
	return matrix;
}

mat4x4 mat4x4::MatrixMakeViewPort(float width, float height)
{
	mat4x4 matrix;
	float xmin = 0.f;
	float ymin = 0.f;

	matrix.m[0][0] = width * 0.5;
	matrix.m[0][3] = xmin + width * 0.5;
	matrix.m[1][1] = -height * 0.5;
	matrix.m[1][3] = ymin + height * 0.5;
	matrix.m[2][2] = 1;
	matrix.m[3][3] = 1;

	return matrix;
}

mat4x4 mat4x4::MatrixPointAt(vec3d& eye, vec3d& target, vec3d& up)
{
	vec3d sub = Vector_Sub(eye, target);
	vec3d Z = Vector_Normalise(sub);

	vec3d cross = Vector_CrossProduct(up, Z);
	vec3d X = Vector_Normalise(cross);

	vec3d Y = Vector_CrossProduct(Z, X);

	mat4x4 result;
	result.m[0][0] = X.x;
	result.m[0][1] = X.y;
	result.m[0][2] = X.z;
	result.m[0][3] = -Vector_DotProduct(X, eye);

	result.m[1][0] = Y.x;
	result.m[1][1] = Y.y;
	result.m[1][2] = Y.z;
	result.m[1][3] = -Vector_DotProduct(Y, eye);

	result.m[2][0] = Z.x;
	result.m[2][1] = Z.y;
	result.m[2][2] = Z.z;
	result.m[2][3] = -Vector_DotProduct(Z, eye);


	result.m[3][0] = 0.f;
	result.m[3][1] = 0.f;
	result.m[3][2] = 0.f;
	result.m[3][3] = 1.f;
	return result;
}


vec3d mat4x4::Vector_Add(vec3d& v1, vec3d& v2)
{
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

vec3d mat4x4::Vector_Sub(vec3d& v1, vec3d& v2)
{
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

vec3d mat4x4::Vector_Mul(vec3d& v1, float k)
{
	return { v1.x * k, v1.y * k, v1.z * k };
}

vec3d mat4x4::Vector_Div(vec3d& v1, float k)
{
	return { v1.x / k, v1.y / k, v1.z / k };
}

float mat4x4::Vector_DotProduct(vec3d& v1, vec3d& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float mat4x4::Vector_Length(vec3d& v)
{
	return sqrtf(Vector_DotProduct(v, v));
}

vec3d mat4x4::Vector_Normalise(vec3d& v)
{
	
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

vec3d mat4x4::Vector_CrossProduct(vec3d& v1, vec3d& v2)
{
	vec3d v;
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}

mat4x4 mat4x4::Transposition(mat4x4& mat)
{
	mat4x4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result.m[i][j] = mat.m[j][i];
		}
	}

	return result;
}
