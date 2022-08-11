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
#include "Map.h"

class Enemy {
protected:
	Shader shader;
private:
	bool hit = false;
public:

	float centerX, centerY;
	float playerX, playerY;
	float angle;

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

	Map map;

	Enemy();
	Enemy(Shader& shader, VertexBufferLayout& layout, float centerX, float centerY, Map map);
	~Enemy();

	void AddBuffer();
	void Bind();
	void Draw(Renderer renderer, glm::mat4& proj);
	void SetAngle(float angle);
	void SetPlayerPos(float playerX, float playerY);
	void Move();
	//float* GetBufferData();
	void CheckForHit();
	bool IsHit();
	bool operator!= (const Enemy& enemy);

};