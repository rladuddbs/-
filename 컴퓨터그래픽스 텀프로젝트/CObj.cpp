#include "CHeader.h"
#include "CObj.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

std::vector<float> CObj::my_strtok_f(char* str, char* delimeter)
{
	std::vector <float> v;
	char* context;
	char* tok = strtok_s(str, delimeter, &context);

	while (tok != NULL) {
		v.emplace_back(atof(tok));
		tok = strtok_s(context, delimeter, &context);
	}
	return v;
}

std::vector<std::string> CObj::my_strtok_s(char* str, char* delimeter)
{
	std::vector <std::string> v;
	char* context;
	char* tok = strtok_s(str, delimeter, &context);

	while (tok != NULL) {
		v.emplace_back(tok);
		tok = strtok_s(context, delimeter, &context);
	}
	return v;
}

std::vector<int> CObj::my_strtok_i(char* str, char* delimeter)
{
	std::vector <int> v;
	char* context;
	char* tok = strtok_s(str, delimeter, &context);

	while (tok != NULL) {
		v.emplace_back(atoi(tok));
		tok = strtok_s(context, delimeter, &context);
	}
	return v;
}

void CObj::LoadObj(std::string name)
{
	std::ifstream fin(name);
	std::ifstream& filename = fin;

	std::string line;
	std::vector<glm::vec3> temp_v;
	std::vector<glm::vec2> temp_t;
	std::vector<glm::vec3> temp_n;

	std::vector<unsigned int> vIdx;
	std::vector<unsigned int> tIdx;
	std::vector<unsigned int> nIdx;
	int numCnt = 0;

	while (getline(filename, line)) {
		int len = line.length();
		std::vector<float> vf;
		std::vector<std::string> s;
		std::vector<int> vi;

		if (line[0] == 'o' && line[1] == ' ') {
			name = line.substr(2, len - 2);
		}

		if (line[0] == 'v' && line[1] == ' ') {
			vf = my_strtok_f((char*)line.substr(2, len - 2).c_str(), (char*)" ");
			temp_v.push_back(glm::vec3(vf[0], vf[1], vf[2]));
		}

		else if (line[0] == 'v' && line[1] == 't') {
			vf = my_strtok_f((char*)line.substr(3, len - 3).c_str(), (char*)" ");
			temp_t.push_back(glm::vec2(vf[0], vf[1]));
		}

		else if (line[0] == 'v' && line[1] == 'n') {
			vf = my_strtok_f((char*)line.substr(3, len - 3).c_str(), (char*)" ");
			temp_n.push_back(glm::vec3(vf[0], vf[1], vf[2]));
		}

		else if (line[0] == 'f' && line[1] == ' ') {
			std::stringstream a;
			std::string tline = line;
			for (int i = 0; i < tline.size(); ++i) {
				if (tline[i] == '/')
					tline[i] = ' ';
			}

			a.str(tline);
			std::string tstr;
			int fCnt = 0;
			numCnt = 0;
			while (a >> tstr) {
				if (tstr == "f") ++fCnt;
				if (fCnt == 2)break;
				++numCnt;
			}

			s = my_strtok_s((char*)line.substr(2, len - 2).c_str(), (char*)" ");
			int nVertexes = s.size();

			for (int i = 0; i < nVertexes; ++i) {
				vi = my_strtok_i((char*)s[i].c_str(), (char*)"/");
				vIdx.push_back(vi[0]);
				tIdx.push_back(vi[1]);
				nIdx.push_back(vi[2]);
			}
		}
	}
	std::cout << numCnt << std::endl;
	if (numCnt == 10) {
		for (int i = 0; i < vIdx.size(); ++i) {
			int idx = vIdx[i] - 1;
			v.push_back(temp_v[idx]);
		}
		for (int i = 0; i < tIdx.size(); ++i) {
			int idx = tIdx[i] - 1;
			vt.push_back(temp_t[idx]);
		}
		for (int i = 0; i < nIdx.size(); ++i) {
			int idx = nIdx[i] - 1;
			vn.push_back(temp_n[idx]);
			f.push_back(i);
		}
	}
	else if (numCnt == 13) {
		for (int i = 0; i < vIdx.size(); ++i) {
			int idx = vIdx[i] - 1;
			v.push_back(temp_v[idx]);
			v.push_back(temp_v[(idx + 2) % 4]);

		}
		for (int i = 0; i < tIdx.size(); ++i) {
			int idx = tIdx[i] - 1;
			vt.push_back(temp_t[idx]);
			vt.push_back(temp_t[(idx + 2) % 4]);

		}
		for (int i = 0; i < nIdx.size(); ++i) {
			int idx = nIdx[i] - 1;
			vn.push_back(temp_n[idx]);
			vn.push_back(temp_n[(idx + 2) % 4]);
			f.push_back(i);
		}
	}
}

void CObj::initBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao);	 //--- VAO를 바인드하기

	glGenBuffers(2, vbo);		 //--- 2개의 VBO를 지정하고 할당하기
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * v.size(), v.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vn.size(), vn.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); //--- GL_ELEMENT_ARRAY_BUFFER 버퍼 유형으로 바인딩
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, f.size() * sizeof(unsigned int), f.data(), GL_STATIC_DRAW);
}

void CObj::render()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, f.size(), GL_UNSIGNED_INT, 0);
}