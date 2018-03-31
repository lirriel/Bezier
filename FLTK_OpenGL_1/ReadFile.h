#ifndef _READFILE_H
#define _READFILE_H
//#include <FL\gl.h>
//#include <glm\glm.hpp>
#include "MyWindow.h"
//#include <vector>
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

class ReadObj {
public:
	ReadObj();
	std::vector<Vertex> Read_File(const char* filename);
	void Write_File(const char* filename, std::vector<Vertex> model);
private:
	std::vector<Vertex> load_obj(const char* filename);
	Buffer buffer;
};


#endif