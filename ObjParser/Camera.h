#pragma once
#include "3DGeometry.h"

class Camera {
public:
	vec3d position;
	vec3d target;
	vec3d up;
	float fNear;
	float fFar;
	float fov;

	Camera();
	Camera(const vec3d& cameraPosition,
		   const vec3d& cameraTarget,
		   const vec3d& cameraUp,
		   float fNear,
		   float fFar,
		   float fov
	);
};
