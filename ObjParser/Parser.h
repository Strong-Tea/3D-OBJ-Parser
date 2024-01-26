#pragma once
#include "Object3D.h"
#include <string>
#include <vector>
#include <functional> 


class Parser {
public:
	static void parseObjFile(std::string& path, Object3D& object3D);
	static int  skipWhiteSpace(std::string& str, int startIndex);
	static int  findFirstWhiteSpace(std::string& str, int startIndex);
	static void parseStringList(std::string& str, int startIndex, std::vector<std::string>& buffer);
	static std::vector<std::vector<int>> splitPolygonToTriangles(const std::vector<int>& polygonIndices);
	static std::string& ltrim(std::string& s);
	static std::string& rtrim(std::string& s);
	static std::string& trim(std::string& s);
private:
	static void parseFloatList(int numFloats, std::string str, int startIndex, float *fbuffer);
	static void processVertex(std::string& line, Object3D& object3DBuffer);
	static void processNormal(std::string& line, Object3D& object3DBuffer);
	static void processTexture(std::string& line, Object3D& object3DBuffer);
	static void processFace(std::string& line, Object3D& object3DBuffer);
};
