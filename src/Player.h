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
#include "Object2d.h"
#include "Enemy.h"

extern const float distances[390];

class Player {
private:
	// these constants define the layout of the game window, which is
	// a 1040x780 3D viewport on the right and a 640x780 2D map on the left.
	const int SCREEN_WIDTH = 1680;
	const int SCREEN_HEIGHT = 780;
	const int TILE_SIZE = 32;
	const int MAP_TILE_COUNT = 20;
	const int MAP_PIXEL_SIZE = MAP_TILE_COUNT * TILE_SIZE;
	const int GAME_WIDTH = SCREEN_WIDTH - MAP_PIXEL_SIZE;
	const int HALF_GAME_WIDTH = GAME_WIDTH / 2;
	const int GAME_HEIGHT = SCREEN_HEIGHT;
	const int HALF_GAME_HEIGHT = SCREEN_HEIGHT / 2;
	const int GAME_MIDPOINT = SCREEN_WIDTH - HALF_GAME_WIDTH;
	const float pi = 3.1415926;
	
	//player starting position in world coordinates
	float playerX = 320.0f;
	float playerY = 64.0f;

	float dx, dy; // screen-space movement vector
	float dx2 = 0.0f, dy2 = 0.0f; // world-space movement vector
	float rot = 0.0f;
	float angle; // current view angle in radians

	//borrowed resources
	Shader& texShader;
	Shader& rayShader;

	//resources ownd by the player
	//walls, floor, ceiling
	Texture floorceiling;
	Texture brick;
	VertexBuffer wallvb;
	VertexBuffer rays;
	VertexBuffer fcvb;

	//rays
	VertexArray lvao;
	VertexBufferLayout llayout;
	IndexBuffer lib;

	GLuint ind[6240]; // needed for creating the index buffer for the wall quads

	//data buffers
	float* lines = nullptr;
	float* texWalls = nullptr;
	float* texFloorCeil = nullptr;
	float zBuffer[1040];

	// main view matrix, representing the camera
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	glm::mat4 line = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	Map map;

	void texLine2(glm::mat4& proj);
public:
	Player();
	Player(Shader& m_rayShader, Shader& m_texShader, Map map);
	~Player();

	void Draw(glm::mat4& proj);
	void HandleEvent(SDL_Event& e);
	void Move();
	void Keyboard(const Uint8* keystate);
	void DrawSprites(Object2d& o, Renderer renderer, glm::mat4 proj);
};