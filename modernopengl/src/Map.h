#pragma once
#include "Renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Map {
private:
	int mapX = 10, mapY = 10;
	int mapS = 100;
	int xOff = 64, yOff = 64;
	glm::mat4 model;
public:
	//move to private
	int map[10][10] = {
	{1,1,1,1,1,1,1,1,1,1},
	{1,0,0,1,0,0,0,0,0,1},
	{1,0,0,1,0,0,0,0,0,1},
	{1,0,0,1,1,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,1,1,1,1,1},
	{1,0,0,0,0,0,1,0,0,1},
	{1,0,0,1,0,0,0,0,0,1},
	{1,0,0,1,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1}
	};

	void DrawMap(const VertexArray& vao, const IndexBuffer& ib, Shader& shader, glm::mat4 proj, Renderer renderer);
};