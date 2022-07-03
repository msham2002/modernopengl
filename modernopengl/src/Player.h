#pragma once
#include<SDL.h>
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
#include <cmath>
#include "Map.h"


class Player{
private:

	float centerX = 208.0f;
	float centerY = 288.0f;

	//Player
	float positions[16] = { //centery 288, y1 280, y2 296 
		200.0f, 280.0f, 0.0f, 0.0f, //last 2 are texture coordinates
		216.0f, 280.0f, 1.0f, 0.0f,
		216.0f, 296.0f, 1.0f, 1.0f,
		200.0f, 296.0f, 0.0f, 1.0f
	};

	GLuint indices[6] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint rayind[2] = {
		0, 1
	};

	VertexBufferLayout layout;
	Shader shader;
	VertexBuffer vb;
	IndexBuffer ib;
	Texture texture;
	VertexArray vao;

	//ray
	VertexArray lvao;
	//VertexBuffer lvb;
	VertexBufferLayout llayout;
	IndexBuffer lib;
	Shader lshader;

	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	glm::mat4 line = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	float angle;
	float pi = 3.1415926;
	float pa = pi / 2;
	float dx, dy;

public:
	Player();
	Player(Shader& shader, VertexBufferLayout& layout, VertexBufferLayout& llayout, Map map);

	void AddBuffer();
	void Bind();
	void Draw(Renderer renderer, glm::mat4& proj);
	void HandleEvent(SDL_Event& e);
	void Move();
	void Keyboard(const Uint8* keystate);
	void Line(glm::mat4& proj);
};