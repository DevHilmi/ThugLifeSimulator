#pragma once

#include "Core.h"
using namespace std;
class Shader {
protected:
	GLuint BuildShader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	void UseShader(GLuint program);
private:
	void CheckShaderErrors(GLuint shader, string type);
};