#include "Camera.h"




Camera::Camera() {
	position	   = { 0.f, 0.f, 100.0f, 1.0f };
	target		   = { 0, 0.f, 0.f, 1.f };
	up			   = { 0.f, 1.f, 0.f, 1.f };
	fNear		   = 0.1f;
	fFar		   = 100.f;
	fov			   = 45.f;
}

Camera::Camera(const vec3d& cameraPosition,
	const vec3d& cameraTarget,
	const vec3d& cameraUp,
	float near,
	float far,
	float fov
) {
	this->position       = cameraPosition;
	this->target	     = cameraTarget;
	this->up		     = cameraUp;
	this->fNear			 = near;
	this->fFar			 = far;
	this->fov			 = fov;
}

