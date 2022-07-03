#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "GL/glew.h"
#include "renderer.h"
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

class Object2d {
protected:
	VertexBufferLayout layout;
	Shader shader;
private:
	float positions[16] = {
		800.0f, 300.0f, 0.0f, 0.0f, //last 2 are texture coordinates
		816.0f, 300.0f, 1.0f, 0.0f,
		816.0f, 316.0f, 1.0f, 1.0f,
		800.0f, 316.0f, 0.0f, 1.0f
	};
	GLuint indices[6] = {
		0, 1, 2,
		2, 3, 0
	};
	VertexBuffer vb;
	IndexBuffer ib;
	Texture texture;
	VertexArray vao;
public:
	Object2d();
	Object2d(Shader& shader, VertexBufferLayout& layout);
	
	void AddBuffer();
	void Bind();
	void Draw(Renderer renderer, glm::mat4& proj);
	
};