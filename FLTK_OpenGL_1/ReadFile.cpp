#include "ReadFile.h"
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <FL\gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

ReadObj::ReadObj(){}

std::vector<Vertex> ReadObj::Read_File(const char * filename) {
	return load_obj(filename);
}

void ReadObj::Write_File(const char * filename, std::vector<Vertex> model) {
	ofstream myfile(filename, ios::trunc);
	
	if (myfile.is_open()) {
		int k = 1;
		for (int i = 0; i < model.size(); i += 3) {
			glm::vec3 normal = model.at(i).normal;

			for (int j = 0; j < 3; j++) {
				myfile << "v " << model.at(i + j).position.x << " ";
				myfile << model.at(i + j).position.y << " ";
				myfile << model.at(i + j).position.z << "\n";
			}
			
			myfile << "vn " << normal.x << " " << normal.y << " " << normal.z << "\n";
			myfile << "f " << i + 1 << "/" << "0/" << k << " ";
			myfile << i + 2 << "/" << "0/" << k << " ";
			myfile << i + 3 << "/" << "0/" << k << "\n";
			k++;
		}

		myfile.close();
	}
	else {
		cout << "Unable to open file";
	}
}

/*
unsigned int ReadObj::loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
*/
std::vector<Vertex> ReadObj::load_obj(const char* filename)
{
	buffer.verticesI = 0;
	buffer.indicesI = 0;

	vector<glm::vec3> Positions;
	vector<glm::vec2> TexCoords;
	vector<glm::vec3> Normals;

	int PositionsI = 0;
	int TexCoordsI = 0;
	int NormalsI = 0;

	WCHAR strCommand[256] = { 0 };
	wifstream InFile(filename);

	if (!InFile) return std::vector<Vertex>();

	for (; ; )
	{
		InFile >> strCommand;
		if (!InFile)
			break;

		if (0 == wcscmp(strCommand, L"v"))
		{
			// Координаты
			float x, y, z;
			InFile >> x >> y >> z;
			Positions.push_back(glm::vec3(x, y, z));
			PositionsI++;
		}
		else if (0 == wcscmp(strCommand, L"vt"))
		{
			// Текстурные координаты
			float u, v;
			InFile >> u >> v;
			TexCoords.push_back(glm::vec2(u, v));
			TexCoordsI++;
		}
		else if (0 == wcscmp(strCommand, L"vn"))
		{
			// Нормали
			float x, y, z;
			InFile >> x >> y >> z;
			Normals.push_back(glm::vec3(x, y, z)); 
			NormalsI++;
		}
		else if (0 == wcscmp(strCommand, L"f"))
		{
			// Face
			UINT iPosition, iTexCoord, iNormal;
			Vertex vertex;

			for (UINT iFace = 0; iFace < 3; iFace++)
			{
				ZeroMemory(&vertex, sizeof(Vertex));

				// OBJ формат использует массивы с индексами от 1
				InFile >> iPosition;
				vertex.position = Positions[iPosition - 1];

				if ('/' == InFile.peek())
				{
					InFile.ignore();

					if ('/' != InFile.peek())
					{
						// Опционно текстурные координаты
						InFile >> iTexCoord;
						//vertex.texcoord = TexCoords[iTexCoord - 1];
					}

					if ('/' == InFile.peek())
					{
						InFile.ignore();

						// Опционно нормали
						InFile >> iNormal;
						vertex.normal = Normals[iNormal - 1];
					}
				}

				buffer.vertices.push_back(vertex);
			}
		}
	}
	InFile.close();

	//Очистка временных массивов
	Positions.clear();
	TexCoords.clear();
	Normals.clear();
	return buffer.vertices;
}