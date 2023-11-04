#pragma once


#define EMPTY_VALUE	-2147483648

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
};

struct triangle {
	vec3d vertex[3];
	vec3d normal[3];
	vec2d texture[3];
};

struct Point {
	int x, y;
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

struct DepthBuffer {
	float* data;
	int width;
	int height;
};


