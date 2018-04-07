#ifndef _READFILE_H
#define _READFILE_H

#include "Buffer.h"

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