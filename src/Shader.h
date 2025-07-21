#pragma once
#include<string>
#include <fstream>
#include<sstream>
#include<iostream>
#include<GL/glew.h>
#include<unordered_map>
#include"glm/glm.hpp"

struct ShaderSource {
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader {
private:
	GLuint m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	int GetUniformLocation(const std::string& name);
	GLuint CompileShader(GLuint type, const std::string& source);
	GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderSource ParseShader(const std::string& file);
public:
	Shader(const std::string& filepath);
	Shader();
	~Shader();
	 
	void Bind() const;
	void UnBind() const;

	//set uniforms
	void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniform1i(const std::string& name, int value);
};