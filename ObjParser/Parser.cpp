#include "Parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "3DGeometry.h"


void Parser::parseObjFile(std::string& path, Object3D& object3D) {
	std::ifstream file(path);
	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			line = trim(line);
			if (line.empty()) {
				continue;
			}

			if (line[0] == '#') {
				continue;
			}
			else if (line[0] == 'v' && line[1] == 'n') {		// texture
				processNormal(line, object3D);
			}
			else if (line[0] == 'v' && line[1] == 't') {		// normal
				processTexture(line, object3D);
			}
			else if (line[0] == 'v') {							// vertex
				processVertex(line, object3D);
			}
			else if (line[0] == 'f') {							// face
				processFace(line, object3D);
			}
		}
		file.close();
	}
	else {
		std::cerr << "Unable to open the file." << std::endl;
	}
}


int Parser::skipWhiteSpace(std::string& str, int startIndex) {
	while (startIndex < str.length()) {
		if (str[startIndex] == ' ' || str[startIndex] == '\n' || str[startIndex] == '\t') {
			startIndex++;
		}
		else {
			break;
		}
	}
	return startIndex;
}


int Parser::findFirstWhiteSpace(std::string& str, int startIndex) {
	while (startIndex < str.length()) {
		if (str[startIndex] != ' ' && str[startIndex] != '\n' && str[startIndex] != '\t') {
			startIndex++;
		}
		else {
			break;
		}
	}
	return startIndex;
}


void Parser::parseStringList(std::string& str, int startIndex, std::vector<std::string>& buffer) {
	int strLength = str.length();
	while (startIndex < strLength) {
		startIndex = skipWhiteSpace(str, startIndex);
		if (startIndex >= strLength) {
			break;
		}
		int endIndex = findFirstWhiteSpace(str, startIndex);
		buffer.push_back(str.substr(startIndex, endIndex - startIndex));
		startIndex = endIndex;
	}
}


std::string& Parser::ltrim(std::string& s) {
	//s.erase(s.begin(), std::find_if(s.begin(), s.end(),
	//	std::not1(std::ptr_fun<int, int>(std::isspace))));
	//return s;



	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int c) {return !std::isspace(c); }));
	return s;
}


std::string& Parser::rtrim(std::string& s) {
	//s.erase(std::find_if(s.rbegin(), s.rend(),
	//	std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	//return s;

	s.erase(std::find_if(s.rbegin(), s.rend(), [](int c) { return !std::isspace(c); }).base(), s.end());
	return s;
}


std::string& Parser::trim(std::string& s) {
	return ltrim(rtrim(s));
}


void Parser::parseFloatList(int numFloats, std::string str, int startIndex, float* fbuffer) {
	int parseIndex = 0;
	int strLength = str.length();

	while (startIndex < strLength) {
		startIndex = skipWhiteSpace(str, startIndex);
		if (startIndex >= strLength || parseIndex == numFloats) {
			break;
		}
		int endIndex = findFirstWhiteSpace(str, startIndex);
		fbuffer[parseIndex++] = std::stof(str.substr(startIndex, endIndex));
		startIndex = endIndex;
	}
}


void Parser::processVertex(std::string& line, Object3D& object3DBuffer) {
	float values[3];
	parseFloatList(3, line, 1, values);
	vec3d vertex = { values[0], values[1], values[2] };
	object3DBuffer.AddVertexGeometric(vertex);
}


void Parser::processNormal(std::string& line, Object3D& object3DBuffer) {
	float values[3];
	parseFloatList(3, line, 2, values);
	vec3d normal = { values[0], values[1], values[2] };
	object3DBuffer.AddVertexNormal(normal);
}


void Parser::processTexture(std::string& line, Object3D& object3DBuffer) {
	float values[2];
	parseFloatList(2, line, 2, values);
	vec3d texture = { values[0], values[1] };
	object3DBuffer.AddVertexTexture(texture);
}

std::vector<std::vector<int>> Parser::splitPolygonToTriangles(const std::vector<int>& polygonIndices) {
	std::vector<std::vector<int>> triangles;

	/*WRITE ALGORITHM*/

	return triangles;
}


void Parser::processFace(std::string& line, Object3D& object3DBuffer) {
	std::vector<std::string> strIndexFaceVertex;
	std::vector<int> vertexIndices;
	parseStringList(line, 1, strIndexFaceVertex);

	for (std::string fv : strIndexFaceVertex) {
		
		std::istringstream iss(fv);
		std::string token;
		std::vector<std::string> tokens;

		while (std::getline(iss, token, ' ')) {
			std::istringstream tokenStream(token);
			while (std::getline(tokenStream, token, '/')) {
				tokens.push_back(token);
			}
		}

		for (const std::string& number : tokens) {
			if (number.empty()) vertexIndices.push_back(EMPTY_VALUE);
			else vertexIndices.push_back(stoi(number));
		}
	}


	if (vertexIndices.size() / 3 == 3) {
		object3DBuffer.AddTriangle(vertexIndices);
	}
	else {
		/*EXAMPLE
		For a quadrilateral with indexes "240/1/1 180/2/1 243/3/1 239/4/1", it will be:
		The first triangle (ABC):
			Vertex A: 240
			Vertex B: 180
			Vertex C: 243
		The second triangle (ACD):
			Vertex A: 240
			Vertex C: 243
			Vertex D: 239*/

		std::vector<int> tri1;
		tri1.push_back(vertexIndices.at(0));
		tri1.push_back(vertexIndices.at(1));
		tri1.push_back(vertexIndices.at(2));
		tri1.push_back(vertexIndices.at(3));
		tri1.push_back(vertexIndices.at(4));
		tri1.push_back(vertexIndices.at(5));
		tri1.push_back(vertexIndices.at(6));
		tri1.push_back(vertexIndices.at(7));
		tri1.push_back(vertexIndices.at(8));
		object3DBuffer.AddTriangle(tri1);

		std::vector<int> tri2;
		tri2.push_back(vertexIndices.at(0));
		tri2.push_back(vertexIndices.at(1));
		tri2.push_back(vertexIndices.at(2));
		tri2.push_back(vertexIndices.at(6));
		tri2.push_back(vertexIndices.at(7));
		tri2.push_back(vertexIndices.at(8));
		tri2.push_back(vertexIndices.at(9));
		tri2.push_back(vertexIndices.at(10));
		tri2.push_back(vertexIndices.at(11));
		object3DBuffer.AddTriangle(tri2);
	}
}
