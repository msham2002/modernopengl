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
	Shader shader;
private:
	bool hit = false;
public:

	float centerX = 384;
	float centerY = 320;

	float positions[8] = {
		centerX, centerY,
		centerX + 32, centerY,
		centerX + 32, centerY + 32,
		centerX, centerY + 32
	};
	float* bufferData;

	GLuint indices[6] = {
		0, 1, 2,
		2, 3, 0
	};
	VertexBuffer vb;
	IndexBuffer ib;
	Texture texture;
	VertexArray vao;
	VertexBufferLayout layout;

	Object2d();
	Object2d(Shader& shader, VertexBufferLayout& layout);
	~Object2d();
	
	void AddBuffer();
	void Bind();
	void Draw(Renderer renderer, glm::mat4& proj);
	//float* GetBufferData();
	void CheckForHit();
	bool IsHit();
	
};