#pragma once
#include <vector>
#include "Vertex.h"
#include <glm\glm.hpp>

using namespace std;

struct Buffer {
	vector<Vertex> vertices;
	vector<glm::vec3> bPosition;
	vector<glm::vec3> bNormal;
	vector<glm::vec2> bTexcoord;
	vector<unsigned int> indices;
	int    verticesI;
	int    indicesI;
};
