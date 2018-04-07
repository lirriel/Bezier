#include "ReadFile.h"
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>

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