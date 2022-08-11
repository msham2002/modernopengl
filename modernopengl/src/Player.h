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


class Player {
private:

	float centerX = 320.0f;
	float centerY = 64.0f;

	float cx = 320.0f;
	float cy = 64.0f;

	float dx2 = 0.0f;
	float dy2 = 0.0f;

	float rot = 0.0f;

	float oldx = 0;
	float oldy = 0;

	//Player
	float positions[16] = { 
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
	Shader texShader;
	VertexBuffer vb;
	IndexBuffer ib;
	Texture texture;
	VertexArray vao;

	//ray
	VertexArray lvao;
	VertexBufferLayout llayout;
	IndexBuffer lib;
	//Shader lshader;

	Shader rayShader;

	Texture floorceiling;
	Texture brick;
	VertexBuffer wallvb;
	VertexBuffer rays;
	VertexBuffer fcvb;
	GLuint ind[6240];

	//lookup table for distances when calculating floor and ceiling pixel positions
	float distances[390] = {
		1, 1.00257, 1.00515, 1.00775, 1.01036, 1.01299, 1.01562, 1.01828, 1.02094, 1.02362, 1.02632, 1.02902, 1.03175, 1.03448, 1.03723, 1.04, 1.04278, 1.04558, 1.04839, 1.05121, 1.05405, 1.05691, 1.05978, 1.06267, 1.06557, 1.06849, 1.07143, 1.07438, 1.07735, 1.08033, 1.08333, 1.08635, 1.08939, 1.09244, 1.09551, 1.09859, 1.10169, 1.10482, 1.10795, 1.11111, 1.11429, 1.11748, 1.12069, 1.12392, 1.12717, 1.13043, 1.13372, 1.13703, 1.14035, 1.14369, 1.14706, 1.15044, 1.15385, 1.15727, 1.16071, 1.16418, 1.16766, 1.17117, 1.1747, 1.17825, 1.18182, 1.18541, 1.18902, 1.19266, 1.19632, 1.2, 1.2037, 1.20743, 1.21118, 1.21495, 1.21875, 1.22257, 1.22642, 1.23028, 1.23418, 1.2381, 1.24204, 1.24601, 1.25, 1.25402, 1.25806, 1.26214, 1.26623, 1.27036, 1.27451, 1.27869, 1.28289, 1.28713, 1.29139, 1.29568, 1.3, 1.30435, 1.30872, 1.31313, 1.31757, 1.32203, 1.32653, 1.33106, 1.33562, 1.34021, 1.34483, 1.34948, 1.35417, 1.35889, 1.36364, 1.36842, 1.37324, 1.37809, 1.38298, 1.3879, 1.39286, 1.39785, 1.40288, 1.40794, 1.41304, 1.41818, 1.42336, 1.42857, 1.43382, 1.43911, 1.44444, 1.44981, 1.45522, 1.46067, 1.46617, 1.4717, 1.47727, 1.48289, 1.48855, 1.49425, 1.5, 1.50579, 1.51163, 1.51751, 1.52344, 1.52941, 1.53543, 1.5415, 1.54762, 1.55378, 1.56, 1.56627, 1.57258, 1.57895, 1.58537, 1.59184, 1.59836, 1.60494, 1.61157, 1.61826, 1.625, 1.6318, 1.63866, 1.64557, 1.65254, 1.65957, 1.66667, 1.67382, 1.68103, 1.68831, 1.69565, 1.70306, 1.71053, 1.71806, 1.72566, 1.73333, 1.74107, 1.74888, 1.75676, 1.76471, 1.77273, 1.78082, 1.78899, 1.79724, 1.80556, 1.81395, 1.82243, 1.83099, 1.83962, 1.84834, 1.85714, 1.86603, 1.875, 1.88406, 1.8932, 1.90244, 1.91176, 1.92118, 1.93069, 1.9403, 1.95, 1.9598, 1.9697, 1.9797, 1.9898, 2, 2.01031, 2.02073, 2.03125, 2.04188, 2.05263, 2.06349, 2.07447, 2.08556, 2.09677, 2.10811, 2.11957, 2.13115, 2.14286, 2.1547, 2.16667, 2.17877, 2.19101, 2.20339, 2.21591, 2.22857, 2.24138, 2.25434, 2.26744, 2.2807, 2.29412, 2.30769, 2.32143, 2.33533, 2.3494, 2.36364, 2.37805, 2.39264, 2.40741, 2.42236, 2.4375, 2.45283, 2.46835, 2.48408, 2.5, 2.51613, 2.53247, 2.54902, 2.56579, 2.58278, 2.6, 2.61745, 2.63514, 2.65306, 2.67123, 2.68966, 2.70833, 2.72727, 2.74648, 2.76596, 2.78571, 2.80576, 2.82609, 2.84672, 2.86765, 2.88889, 2.91045, 2.93233, 2.95455, 2.9771, 3, 3.02326, 3.04688, 3.07087, 3.09524, 3.12, 3.14516, 3.17073, 3.19672, 3.22314, 3.25, 3.27731, 3.30508, 3.33333, 3.36207, 3.3913, 3.42105, 3.45133, 3.48214, 3.51351, 3.54545, 3.57798, 3.61111, 3.64486, 3.67925, 3.71429, 3.75, 3.78641, 3.82353, 3.86139, 3.9, 3.93939, 3.97959, 4.02062, 4.0625, 4.10526, 4.14894, 4.19355, 4.23913, 4.28571, 4.33333, 4.38202, 4.43182, 4.48276, 4.53488, 4.58824, 4.64286, 4.6988, 4.7561, 4.81482, 4.875, 4.93671, 5, 5.06494, 5.13158, 5.2, 5.27027, 5.34247, 5.41667, 5.49296, 5.57143, 5.65217, 5.73529, 5.8209, 5.90909, 6, 6.09375, 6.19048, 6.29032, 6.39344, 6.5, 6.61017, 6.72414, 6.84211, 6.96429, 7.09091, 7.22222, 7.35849, 7.5, 7.64706, 7.8, 7.95918, 8.125, 8.29787, 8.47826, 8.66667, 8.86364, 9.06977, 9.28571, 9.5122, 9.75, 10, 10.2632, 10.5405, 10.8333, 11.1429, 11.4706, 11.8182, 12.1875, 12.5806, 13, 13.4483, 13.9286, 14.4444, 15, 15.6, 16.25, 16.9565, 17.7273, 18.5714, 19.5, 20.5263, 21.6667, 22.9412, 24.375, 26, 27.8571, 30, 32.5, 35.4545, 39, 43.3333, 48.75, 55.7143, 65, 78, 97.5, 130, 195, 390
	};


	//gun

	float gunPos[16] = {
		1680 - 496, 0.0f, 0.0f, 0.0f,
		1680, 0.0f, 0.5f, 0.0f,
		1680, 276.0f, 0.5f, 1.0f,
		1680 - 496, 276.0f, 0.0f, 1.0f,
	};
	VertexArray gunvao;
	VertexBuffer gunvb;
	VertexBufferLayout gunlayout;
	IndexBuffer gunib;

	Texture rifle;

	float gunStep = 0;
	bool end = false;
	bool right = true;
	int count = 0;
	bool press = false;
	unsigned time;
	//
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	glm::mat4 line = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

	float angle;
	float pi = 3.1415926;
	float pa = pi / 2;
	float dx, dy;

	Map map;

	float *lines;
	//float walls[4160];
	float horwalls[4160];
	float vertwalls[4160];
	//float texwalls[8320];
	//float wallEdge[8320];
	float *texWalls;
	float *texFloors;
	float *texCeil;
	float *texFloorCeil;
	float *tempsprite;
	float zBuffer[1040];
public:
	Player();
	Player(Shader& shader, VertexBufferLayout& layout, VertexBufferLayout& llayout, Map map);
	~Player();

	void AddBuffer();
	void Bind();
	void Draw(Renderer renderer, glm::mat4& proj);
	void HandleEvent(SDL_Event& e);
	void Move();
	void Keyboard(const Uint8* keystate);
	void Line(glm::mat4& proj);
	void texLine2(glm::mat4& proj);
	void DrawSprites(Object2d &o, Renderer renderer, glm::mat4 proj);
	void DrawEnemies(Enemy *arr, Renderer renderer, glm::mat4 proj);
	void DrawGun();
	bool isShooting();
};