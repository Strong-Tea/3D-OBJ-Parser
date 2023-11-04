#pragma once
#include "Object3D.h"
#include <string>
#include <vector>
#include <functional> 


class Parser {
public:
	static void ParseObjFile(std::string& path, Object3D& object3D);
	static int SkipWhiteSpace(std::string& str, int startIndex);
	static int FindFirstWhiteSpace(std::string& str, int startIndex);
	static void ParseStringList(std::string& str, int startIndex, std::vector<std::string>& buffer);
	static std::vector<std::vector<int>> SplitPolygonToTriangles(const std::vector<int>& polygonIndices);
	static std::string& ltrim(std::string& s);
	static std::string& rtrim(std::string& s);
	static std::string& trim(std::string& s);
private:
	static void ParseFloatList(int numFloats, std::string str, int startIndex, float *fbuffer);
	static void ProcessVertex(std::string& line, Object3D& object3DBuffer);
	static void ProcessNormal(std::string& line, Object3D& object3DBuffer);
	static void ProcessTexture(std::string& line, Object3D& object3DBuffer);
	static void ProcessFace(std::string& line, Object3D& object3DBuffer);
};
