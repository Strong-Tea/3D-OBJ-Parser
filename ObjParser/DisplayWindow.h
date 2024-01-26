#pragma once
#include <SFML/Graphics.hpp>
#include "3DGeometry.h"
#include "Object3D.h"
#include "Camera.h"
#include <Windows.h>


#define PIXEL_SIZE_BYTES 4

class DisplayWindow {
private:
	// Window
	sf::RenderWindow window;
	int width;
	int height;
	unsigned int diffuseMapWidth = 0;
	unsigned int diffuseMapHeight = 0;

	unsigned int normalMapWidth = 0;
	unsigned int normalMapHeight = 0;

	unsigned int specularMapWidth = 0;
	unsigned int specularMapHeight = 0;

	// Camera
	Camera camera;

	// Buffers
	sf::Uint8* pixelBuffer;
	float* zBuffer;
	std::vector<std::vector<sf::Color>> diffuseMap;
	std::vector<std::vector<sf::Color>> normalMap;
	std::vector<std::vector<sf::Color>> specularMap;

	// Lighting and Shading
	float ambient;
	float shininess;
	float ambientIntensity;
	float diffuseReflectanceCoefficient;
	float specularReflectionCoefficient;
	vec3d modelColor;
	vec3d diffuseColor;
	vec3d specularColor;
	vec3d lightPosition;

	// Private functions
	void resetZBuffer();
	void drawDislayInfo();
public:

	

	DisplayWindow(int width, int height, const std::string& title = "", int framerateLimit = 60);
	~DisplayWindow();
	vec3d calculatePhongShading(vec3d& point, vec3d& interpolatedNormal, vec3d& lightPosition, vec3d& cameraPosition, vec3d& modelColor, vec3d& diffuseColor, vec3d& specularColor, Object3D& obj, mat4x4& matproj);
	float calculateLambertLighting(vec3d& normal, vec3d& lightDirection, float ambient);
	void threadPoolWork(int ix, int iy, float iz, int index, vec3d point, triangle& tri, triangle& tri1, mat4x4& matRotate, Object3D& obj3D, mat4x4& matproj);
	void drawDDALine(int x1, int y1, int x2, int y2, float z1, float z2, float intensity, triangle& tri, mat4x4& invViewPort, triangle& tri1, mat4x4& matRotate, Object3D& obj3D, mat4x4& matproj);
	std::vector<std::vector<sf::Color>> readMap(const std::string& filepath, unsigned int& width, unsigned int& height);


	bool isPointInShadow(const vec3d& point, const vec3d& lightPosition, Object3D& obj, mat4x4& matproj);
	bool isRayIntersectingTriangle(vec3d& lightPosition, vec3d& point, Object3D& obj3D);

	void drawObjModel(Object3D& obj3D);
	void render(Object3D& obj3D);
};
