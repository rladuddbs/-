#pragma once
#include "CHeader.h"
#include <string>
#include <vector>
#include <fstream>

class CObj
{
private:
	std::vector<glm::vec3> v;
	std::vector<glm::vec2> vt;
	std::vector<glm::vec3> vn;
	std::vector<unsigned int> f;
	std::string name;

public:
	unsigned int vao, ebo;
	unsigned int vbo[2];
public:
	std::vector<float> my_strtok_f(char* str, char* delimeter);
	std::vector<std::string> my_strtok_s(char* str, char* delimeter);
	std::vector<int> my_strtok_i(char* str, char* delimeter);

	void LoadObj(std::string name);
	void initBuffer();
	void render();

};