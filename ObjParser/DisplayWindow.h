#pragma once
#include <SFML/Graphics.hpp>
#include "Mat4x4.h"
#include "Object3D.h"
#include <Windows.h>

#define PIXEL_SIZE_BYTES 4

class DisplayWindow {
private:
	// Window
	sf::RenderWindow window;
	int width;
	int height;
	sf::Uint8* pixelBuffer;
	float* zBuffer;
public:
	DisplayWindow(int width, int height, const std::string& title = "", int framerateLimit = 60);
	~DisplayWindow();
	void DrawDDALine(int x1, int y1, int x2, int y2, float z1, float z2);
	void resetZBuffer();
	void drawObjModel(Object3D& obj3D, float fElapsedTime);
	void fillTriangle(const Point& p1, const Point& p2, const Point& p3);
	void DrawFilledTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
	std::vector<int> findIntersections(const Point& p1, const Point& p2, int y);
	void drawDislayInfo();
	void Render(Object3D& obj3D);
};
