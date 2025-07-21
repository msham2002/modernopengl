#include "Player.h"

//lookup table for distances when calculating floor and ceiling pixel positions
const float distances[390] = {
	1, 1.00257, 1.00515, 1.00775, 1.01036, 1.01299, 1.01562, 1.01828, 1.02094, 1.02362, 1.02632, 1.02902, 1.03175, 1.03448, 1.03723, 1.04, 1.04278, 1.04558, 1.04839, 1.05121, 1.05405, 1.05691, 1.05978, 1.06267, 1.06557, 1.06849, 1.07143, 1.07438, 1.07735, 1.08033, 1.08333, 1.08635, 1.08939, 1.09244, 1.09551, 1.09859, 1.10169, 1.10482, 1.10795, 1.11111, 1.11429, 1.11748, 1.12069, 1.12392, 1.12717, 1.13043, 1.13372, 1.13703, 1.14035, 1.14369, 1.14706, 1.15044, 1.15385, 1.15727, 1.16071, 1.16418, 1.16766, 1.17117, 1.1747, 1.17825, 1.18182, 1.18541, 1.18902, 1.19266, 1.19632, 1.2, 1.2037, 1.20743, 1.21118, 1.21495, 1.21875, 1.22257, 1.22642, 1.23028, 1.23418, 1.2381, 1.24204, 1.24601, 1.25, 1.25402, 1.25806, 1.26214, 1.26623, 1.27036, 1.27451, 1.27869, 1.28289, 1.28713, 1.29139, 1.29568, 1.3, 1.30435, 1.30872, 1.31313, 1.31757, 1.32203, 1.32653, 1.33106, 1.33562, 1.34021, 1.34483, 1.34948, 1.35417, 1.35889, 1.36364, 1.36842, 1.37324, 1.37809, 1.38298, 1.3879, 1.39286, 1.39785, 1.40288, 1.40794, 1.41304, 1.41818, 1.42336, 1.42857, 1.43382, 1.43911, 1.44444, 1.44981, 1.45522, 1.46067, 1.46617, 1.4717, 1.47727, 1.48289, 1.48855, 1.49425, 1.5, 1.50579, 1.51163, 1.51751, 1.52344, 1.52941, 1.53543, 1.5415, 1.54762, 1.55378, 1.56, 1.56627, 1.57258, 1.57895, 1.58537, 1.59184, 1.59836, 1.60494, 1.61157, 1.61826, 1.625, 1.6318, 1.63866, 1.64557, 1.65254, 1.65957, 1.66667, 1.67382, 1.68103, 1.68831, 1.69565, 1.70306, 1.71053, 1.71806, 1.72566, 1.73333, 1.74107, 1.74888, 1.75676, 1.76471, 1.77273, 1.78082, 1.78899, 1.79724, 1.80556, 1.81395, 1.82243, 1.83099, 1.83962, 1.84834, 1.85714, 1.86603, 1.875, 1.88406, 1.8932, 1.90244, 1.91176, 1.92118, 1.93069, 1.9403, 1.95, 1.9598, 1.9697, 1.9797, 1.9898, 2, 2.01031, 2.02073, 2.03125, 2.04188, 2.05263, 2.06349, 2.07447, 2.08556, 2.09677, 2.10811, 2.11957, 2.13115, 2.14286, 2.1547, 2.16667, 2.17877, 2.19101, 2.20339, 2.21591, 2.22857, 2.24138, 2.25434, 2.26744, 2.2807, 2.29412, 2.30769, 2.32143, 2.33533, 2.3494, 2.36364, 2.37805, 2.39264, 2.40741, 2.42236, 2.4375, 2.45283, 2.46835, 2.48408, 2.5, 2.51613, 2.53247, 2.54902, 2.56579, 2.58278, 2.6, 2.61745, 2.63514, 2.65306, 2.67123, 2.68966, 2.70833, 2.72727, 2.74648, 2.76596, 2.78571, 2.80576, 2.82609, 2.84672, 2.86765, 2.88889, 2.91045, 2.93233, 2.95455, 2.9771, 3, 3.02326, 3.04688, 3.07087, 3.09524, 3.12, 3.14516, 3.17073, 3.19672, 3.22314, 3.25, 3.27731, 3.30508, 3.33333, 3.36207, 3.3913, 3.42105, 3.45133, 3.48214, 3.51351, 3.54545, 3.57798, 3.61111, 3.64486, 3.67925, 3.71429, 3.75, 3.78641, 3.82353, 3.86139, 3.9, 3.93939, 3.97959, 4.02062, 4.0625, 4.10526, 4.14894, 4.19355, 4.23913, 4.28571, 4.33333, 4.38202, 4.43182, 4.48276, 4.53488, 4.58824, 4.64286, 4.6988, 4.7561, 4.81482, 4.875, 4.93671, 5, 5.06494, 5.13158, 5.2, 5.27027, 5.34247, 5.41667, 5.49296, 5.57143, 5.65217, 5.73529, 5.8209, 5.90909, 6, 6.09375, 6.19048, 6.29032, 6.39344, 6.5, 6.61017, 6.72414, 6.84211, 6.96429, 7.09091, 7.22222, 7.35849, 7.5, 7.64706, 7.8, 7.95918, 8.125, 8.29787, 8.47826, 8.66667, 8.86364, 9.06977, 9.28571, 9.5122, 9.75, 10, 10.2632, 10.5405, 10.8333, 11.1429, 11.4706, 11.8182, 12.1875, 12.5806, 13, 13.4483, 13.9286, 14.4444, 15, 15.6, 16.25, 16.9565, 17.7273, 18.5714, 19.5, 20.5263, 21.6667, 22.9412, 24.375, 26, 27.8571, 30, 32.5, 35.4545, 39, 43.3333, 48.75, 55.7143, 65, 78, 97.5, 130, 195, 390
};

Player::Player(Shader& m_rayShader, Shader& m_texShader, Map map) :
	wallvb(nullptr, 16640 * sizeof(float)), // 1040 columns x 16 floats per wall slice
	fcvb(nullptr, 520 * 390 * 8 * 2 * 2 * sizeof(float)), // vertex buffer for floor and ceiling
	rays(nullptr, 2080 * 2 * sizeof(float)), // vertex buffer for debug rays
	brick("res/Textures/walls.png"), // texture atlas for walls
	floorceiling("res/Textures/floorceiling.png"), // texture atlast for floor and celing
	rayShader(m_rayShader), // shader for drawing debug rays
	texShader(m_texShader) // shader for drawing textured elements
{
	lines = new float[8320];
	texWalls = new float[16640];
	texFloorCeil = new float[6489600];

	llayout.Push<float>(2);
	lvao.AddBuffer(rays, llayout);

	this->map = map;

	dx = 0.0f, dy = 0.0f; // screen-space pan x and y
	angle = 90.0f * pi / 180; // facing north at start
	rot = 0.075f; // rotation speed per frame

	lines = new float[8320];
	texWalls = new float[16640];
	texFloorCeil = new float[6489600];
}

Player::~Player()
{
	delete[] lines;
	delete[] texWalls;
	delete[] texFloorCeil;
}

void Player::Draw(glm::mat4& proj) {
	glm::mat4 mvp = proj * view;
	texLine2(proj);
}

void Player::HandleEvent(SDL_Event& e) {
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
		case SDLK_SPACE:
			break;
		}
	}
}

void Player::Move() {
	//WIP COLLISION CODE
	//std::cout << trunc(20 - playerY / 32) << " " << trunc(playerX / 32) << std::endl;

	//int iInd = trunc(20 - playerY / 32);
	//int jInd = trunc(playerX / 32);
	/*
	if (map.map[iInd][jInd] == 1) {
		std::cout << "in a wall" << std::endl;
	}
	else {
		std::cout << "not in a wall" << std::endl;
	}*/
	/*
	std::cout << map.map[iInd][jInd] << std::endl;
	if (map.map[iInd][jInd] == 1) {
		view = glm::translate(view, glm::vec3(-1.1 * dx, -1.1 * dy, 0));
		playerX = oldx;
		playerY = oldy;
	}
	else {
		view = glm::translate(view, glm::vec3(dx, dy, 0));
		playerX += dx2;
		playerY += dy2;
	}
	*/
	//std::cout << trunc((playerX + dx2) / 32) << " " <<  jInd << std::endl;

	// the 'view' matrix is what OpenGL uses as its camera. Translating it gives the
	// illusion of movement. 'dx' and 'dy' are the screen-relative directions.
	view = glm::translate(view, glm::vec3(dx, dy, 0));

	// update world position
	playerX += dx2;
	playerY += dy2;

	// normalize angle to [0, 2pi]
	if (angle < 0)
		angle += 2 * pi;

	if (angle > 2 * pi)
		angle -= 2 * pi;
}

void Player::Keyboard(const Uint8* keystate) {
	const Uint8 up = keystate[SDL_SCANCODE_W];
	const Uint8 down = keystate[SDL_SCANCODE_S];
	const Uint8 left = keystate[SDL_SCANCODE_A];
	const Uint8 right = keystate[SDL_SCANCODE_D];
	const Uint8 lookL = keystate[SDL_SCANCODE_LEFT];
	const Uint8 lookR = keystate[SDL_SCANCODE_RIGHT];

	//reset movement vectors at start of frame
	dx = 0.0f; dy = 0.0f;
	dx2 = 0.0f; dy2 = 0.0f;

	// handle forward/backward movement
	if (up) {
		dy += 3.0f;
		dx2 += 3.0f * cos(angle);
		dy2 += 3.0f * sin(angle);
	}
	if (down) {
		dy -= 3.0f;
		dx2 -= 3.0f * cos(angle);
		dy2 -= 3.0f * sin(angle);
	}

	// handle strafing movement
	if (right) {
		dx += 3.0f;
		dx2 += 3.0f * sin(angle);
		dy2 -= 3.0f * cos(angle);
	}
	if (left) {
		dx -= 3.0f;
		dx2 -= 3.0f * sin(angle);
		dy2 += 3.0f * cos(angle);
	}

	// normalize diagonal movement
	// this prevents the player from moving faster on diagonals.
	if (dx2 != 0.0f && dy2 != 0.0f) {
		float length = sqrt(dx2 * dx2 + dy2 * dy2);
		dx2 = (dx2 / length) * 3.0f;
		dy2 = (dy2 / length) * 3.0f;
	}

	//no keys
	if (!up && !down && !left && !right) {
		dy = 0;
		dx = 0;
		dx2 = 0.0f;
		dy2 = 0.0f;
	}

	// look left/right around screen center point
	if (lookL) {
		view = glm::rotate(view, 0.075f, glm::vec3(0, 0, 1));
		rot = 0.075f;
	}
	if (lookR) {
		view = glm::rotate(view, -0.075f, glm::vec3(0, 0, 1));
		rot = -0.075f;
	}
	if (lookL && lookR) {
		rot = 0;
	}
	if (!lookL && !lookR) {
		rot = 0;
	}
}

/**
 * This is the core of the raycasting engine.
 * It casts rays for each vertical column of the screen to render walls, floor, and ceiling.
 * The function is structured into two large, nearly identical loops: one for the right half
 * of the screen and one for the left half.
 * @param proj The camera's projection matrix.
 */
void Player::texLine2(glm::mat4& proj) {
	glm::mat4 mvp2 = proj * line;

	// bind shader for debug rays
	rayShader.Bind();
	rayShader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
	rayShader.SetUniformMat4f("u_MVP", mvp2);

	float a = 0 * pi / 180; // cumlitive angle offset from the player's center view
	float add = 0.0f; // angle to add for the next ray

	int index = 0; // index for lines debug buffer
	int horindex = 0; // index for texWalls buffer
	int floorIndex = 0; // index for texFloorCeil buffer
	int horsize = 0;

	// instead of deleting and allocating 6.5MB every frame,
	// we now just clear the buffer that was allocated in the constructor. This is much faster.
	std::fill_n(texFloorCeil, 6489600, 0);
	//texFloorCeil = new float[6489600];

	//do this process twice, once for each half of screen
	for (float i = 0; i < HALF_GAME_WIDTH; i++) {

		// atan used to get a non linear angle increment to correct fish-eye lens distortion effect
		add = (atan(i / HALF_GAME_WIDTH));

		if (i > 0)
			add = add - atan((i - 1) / HALF_GAME_WIDTH);
		a += add; // add this angle to our offset

		float ang = angle + a; // angle of this ray in the world

		// normalization again
		if (ang < 0)
			ang += 2 * pi;
		if (ang > 2 * pi)
			ang -= 2 * pi;

		// This algorithm finds the closest wall by checking intersections with the map grid.
		// it does this twice, once for horizontal grid lines and once for vertical ones.

		//check for vertical grid line hits
		float r2;
		if (ang < pi / 2 || ang > 3 * pi / 2) { // ray is pointing right
			r2 = (TILE_SIZE - int(playerX) % TILE_SIZE + int(playerX));
		}
		else if (ang > pi / 2 || ang < 3 * pi / 2) { // ray is pointing left
			r2 = int(playerX) - int(playerX) % TILE_SIZE;
		}
		else { // ray is perfectly vertical
			r2 = 0;
		}

		// check for horizontal grid line hits
		float r;
		if (ang < pi) { // ray is pointing up
			r = (TILE_SIZE - int(playerY) % TILE_SIZE) + int(playerY); // nearest 32nd value
		}
		else if (ang > pi) { // ray is pointing down
			r = int(playerY) - int(playerY) % TILE_SIZE;
		}
		else { // ray is perfectly horizontal
			r = 0;
		}

		//correspoinding x and y coords
		float w = r - playerY; // distance between r and player center
		float xdist = w / tan(ang);

		float w2 = r2 - playerX;
		float ydist = w2 * tan(ang);

		int dof = 0;

		float texOffsetX;
		float texOffsetY;

		//check horizontal walls
		float texOffsetXhor;
		float texOffsetYhor;
		while (dof < MAP_TILE_COUNT) {
			int mapj = (TILE_SIZE - int(playerX + xdist) % TILE_SIZE) + int(playerX + xdist);
			int mapi = r;

			mapj /= TILE_SIZE;
			mapi /= TILE_SIZE;

			mapj -= 1;
			if (ang < pi)
				mapi += 1;
			mapi = MAP_TILE_COUNT - mapi;

			if (mapj < MAP_TILE_COUNT && mapi < MAP_TILE_COUNT && mapi > -1 && mapj > -1 && map.map[mapi][mapj] != 0) {
				dof = MAP_TILE_COUNT;
				switch (map.map[mapi][mapj]) {
				case 1: {
					texOffsetXhor = 0;
					texOffsetYhor = 0.5;
					break;
				}
				case 2: {
					texOffsetXhor = 32;
					texOffsetYhor = 0.5;
					break;
				}
				case 3: {
					texOffsetXhor = 64;
					texOffsetYhor = 0.5;
					break;
				}
				case 4: {
					texOffsetXhor = 96;
					texOffsetYhor = 0.5;
					break;
				}
				case 5: {
					texOffsetXhor = 128;
					texOffsetYhor = 0.5;
					break;
				}
				case 6: {
					texOffsetXhor = 160;
					texOffsetYhor = 0.5;
					break;
				}
				case 7: {
					texOffsetXhor = 0;
					texOffsetYhor = 0;
					break;
				}
				case 8: {
					texOffsetXhor = 32;
					texOffsetYhor = 0;
					break;
				}
				case 9: {
					texOffsetXhor = 64;
					texOffsetYhor = 0;
					break;
				}
				case 10: {
					texOffsetXhor = 96;
					texOffsetYhor = 0;
					break;
				}
				case 11: {
					texOffsetXhor = 128;
					texOffsetYhor = 0;
					break;
				}
				case 12: {
					texOffsetXhor = 160;
					texOffsetYhor = 0;
					break;
				}
				}
			}
			else {
				if (ang < pi)
					r += TILE_SIZE;
				else
					r -= TILE_SIZE;
				xdist = (r - playerY) / tan(ang);
				dof += 1;
			}
		}

		//check vertical walls
		dof = 0;
		float texOffsetXvert;
		float texOffsetYvert;
		while (dof < MAP_TILE_COUNT) {
			int mapy = (TILE_SIZE - int(playerY + ydist) % TILE_SIZE) + int(playerY + ydist);
			int mapx = r2;

			mapx /= TILE_SIZE;
			mapy /= TILE_SIZE;

			if (ang > pi / 2 && ang < 3 * pi / 2)
				mapx -= 1;
			mapy = MAP_TILE_COUNT - mapy;

			if (mapx < MAP_TILE_COUNT && mapy < MAP_TILE_COUNT && mapx > -1 && mapy > -1 && map.map[mapy][mapx] != 0) {
				dof = MAP_TILE_COUNT;
				switch (map.map[mapy][mapx]) {
				case 1: {
					texOffsetXvert = 0;
					texOffsetYvert = 0.5;
					break;
				}
				case 2: {
					texOffsetXvert = 32;
					texOffsetYvert = 0.5;
					break;
				}
				case 3: {
					texOffsetXvert = 64;
					texOffsetYvert = 0.5;
					break;
				}
				case 4: {
					texOffsetXvert = 96;
					texOffsetYvert = 0.5;
					break;
				}
				case 5: {
					texOffsetXvert = 128;
					texOffsetYvert = 0.5;
					break;
				}
				case 6: {
					texOffsetXvert = 160;
					texOffsetYvert = 0.5;
					break;
				}
				case 7: {
					texOffsetXvert = 0;
					texOffsetYvert = 0;
					break;
				}
				case 8: {
					texOffsetXvert = 32;
					texOffsetYvert = 0;
					break;
				}
				case 9: {
					texOffsetXvert = 64;
					texOffsetYvert = 0;
					break;
				}
				case 10: {
					texOffsetXvert = 96;
					texOffsetYvert = 0;
					break;
				}
				case 11: {
					texOffsetXvert = 128;
					texOffsetYvert = 0;
					break;
				}
				case 12: {
					texOffsetXvert = 160;
					texOffsetYvert = 0;
					break;
				}
				}
			}
			else {
				if (ang < pi / 2 || ang > 3 * pi / 2)
					r2 += TILE_SIZE;
				else if (ang > pi / 2 || ang < 3 * pi / 2)
					r2 -= TILE_SIZE;
				ydist = (r2 - playerX) * tan(ang);
				dof += 1;
			}
		}

		float vertLen;
		float horLen;

		horLen = sqrt(pow(playerX - (playerX + xdist), 2) + pow(playerY - r, 2));
		vertLen = sqrt(pow(playerX - r2, 2) + pow(playerY - (playerY + ydist), 2));

		float newX, newY;

		float ray[4] = {
				playerX, playerY,
				playerX + xdist, r
		};

		float ray2[4] = {
			playerX, playerY,
			r2, playerY + ydist
		};

		float lineDist; // this will store the final, corrected distance to the wall.

		if (horLen < vertLen) {
			lineDist = horLen;
			lines[index] = playerX;
			lines[index + 1] = playerY;
			lines[index + 2] = (playerX + xdist);
			lines[index + 3] = r;
			texOffsetX = texOffsetXhor;
			texOffsetY = texOffsetYhor;
		}

		else {
			lineDist = vertLen;
			lines[index] = playerX;
			lines[index + 1] = playerY;
			lines[index + 2] = r2;
			lines[index + 3] = (playerY + ydist);
			texOffsetX = texOffsetXvert;
			texOffsetY = texOffsetYvert;
		}

		// the raw distance 'lineDist' is longer for rays at the edge of the screen.
		// multiplying by cos(a) corrects this, making the walls appear straight.
		float ca = a;
		if (ca < 0)
			ca += 2 * pi;
		if (ca > 2 * pi)
			ca -= 2 * pi;

		// store this final distance in the Z-Buffer. This is used later to correctly draw sprites
		// in front of or behind walls. This column is on the right side of the screen.
		lineDist *= cos(ca);
		zBuffer[519 - int(i)] = lineDist;

		// the height of the wall on screen is inversely proportional to its distance.
		float lineHeight = (64 / lineDist) * HALF_GAME_WIDTH;
		if (lineHeight > 5000)
			lineHeight = 5000; // cap the height to prevent huge walls up close.

		// calculate the top Y-coordinate of the wall slice on screen.
		float lineOffset = HALF_GAME_HEIGHT - lineHeight / 2;
		float texOffset;

		if (horLen < vertLen) {
			float texcoordX = int(playerX + xdist) % TILE_SIZE;
			float texcoordY = texOffsetY;
			texcoordX += texOffsetX;
			texcoordX *= (0.0052083);

			// wall coordinates
			texWalls[horindex] = (GAME_MIDPOINT - i);
			texWalls[horindex + 1] = lineOffset;

			//texture coordinates
			texWalls[horindex + 2] = texcoordX;
			texWalls[horindex + 3] = texcoordY;

			texWalls[horindex + 4] = (GAME_MIDPOINT - i + 1);
			texWalls[horindex + 5] = lineOffset;

			texWalls[horindex + 6] = texcoordX;
			texWalls[horindex + 7] = texcoordY;

			texWalls[horindex + 8] = (GAME_MIDPOINT - i + 1);
			texWalls[horindex + 9] = (lineHeight + lineOffset);

			texWalls[horindex + 10] = texcoordX;
			texWalls[horindex + 11] = texcoordY + 0.5;

			texWalls[horindex + 12] = (GAME_MIDPOINT - i);
			texWalls[horindex + 13] = (lineHeight + lineOffset);

			texWalls[horindex + 14] = texcoordX;
			texWalls[horindex + 15] = texcoordY + 0.5;

			horindex += 16;
			horsize += 16;
		}
		else {
			float texcoordX = int(playerY + ydist) % TILE_SIZE;
			float texcoordY = texOffsetY;
			texcoordX += texOffsetX;
			texcoordX *= (0.0052083);

			texWalls[horindex] = (GAME_MIDPOINT - i);
			texWalls[horindex + 1] = lineOffset;

			texWalls[horindex + 2] = texcoordX;
			texWalls[horindex + 3] = texcoordY;

			texWalls[horindex + 4] = (GAME_MIDPOINT - i + 1); //possibly change to -1 instead of +1 probably not though, but i'll leave it here just in case im actually stupid
			texWalls[horindex + 5] = lineOffset;

			texWalls[horindex + 6] = texcoordX;
			texWalls[horindex + 7] = texcoordY;

			texWalls[horindex + 8] = (GAME_MIDPOINT - i + 1);
			texWalls[horindex + 9] = (lineHeight + lineOffset);

			texWalls[horindex + 10] = texcoordX;
			texWalls[horindex + 11] = texcoordY + 0.5;

			texWalls[horindex + 12] = (GAME_MIDPOINT - i);
			texWalls[horindex + 13] = (lineHeight + lineOffset);

			texWalls[horindex + 14] = texcoordX;
			texWalls[horindex + 15] = texcoordY + 0.5;

			horindex += 16;
			horsize += 16;
		}
		index += 4;

		float dirCos = cos(ang);
		float dirSin = sin(ang);

		float correction = 1 / cos(a);
		for (int j = 0; j < lineOffset; j++) {

			float pixelDist = distances[j];
			pixelDist *= correction;
			pixelDist *= 45;

			float pixelX = pixelDist * dirCos;
			float pixelY = pixelDist * dirSin;

			pixelX += playerX;
			pixelY += playerY;

			pixelX = int(pixelX);
			pixelY = int(pixelY);

			float texX = pixelX * 0.00078125;
			float texY = pixelY * 0.00078125;

			texFloorCeil[floorIndex] = float(GAME_MIDPOINT) - i;
			texFloorCeil[floorIndex + 1] = float(j);

			texFloorCeil[floorIndex + 2] = texX;
			texFloorCeil[floorIndex + 3] = texY;

			texFloorCeil[floorIndex + 4] = float(GAME_MIDPOINT) - i;
			texFloorCeil[floorIndex + 5] = float(j) + 1;

			texFloorCeil[floorIndex + 6] = texX;
			texFloorCeil[floorIndex + 7] = texY;


			texFloorCeil[floorIndex + 8] = float(GAME_MIDPOINT) - i;
			texFloorCeil[floorIndex + 9] = 780 - float(j);

			texFloorCeil[floorIndex + 10] = texX + 0.5;
			texFloorCeil[floorIndex + 11] = texY;

			texFloorCeil[floorIndex + 12] = float(GAME_MIDPOINT) - i;
			texFloorCeil[floorIndex + 13] = 780 - float(j) - 1;

			texFloorCeil[floorIndex + 14] = texX + 0.5;
			texFloorCeil[floorIndex + 15] = texY;

			//change floorindex to foorceilindex
			floorIndex += 16;
		}
	}

	a = 0.0f;
	add = 0.0f;
	for (float i = 0; i < HALF_GAME_WIDTH; i++) {
		add = (atan(i / HALF_GAME_WIDTH));

		if (i > 0)
			add = add - atan((i - 1) / HALF_GAME_WIDTH);

		a -= add;

		float ang = angle + a;

		if (ang < 0)
			ang += 2 * pi;
		if (ang > 2 * pi)
			ang -= 2 * pi;

		float r2;
		if (ang < pi / 2 || ang > 3 * pi / 2) {
			r2 = (TILE_SIZE - int(playerX) % TILE_SIZE + int(playerX));
		}
		else if (ang > pi / 2 || ang < 3 * pi / 2) {
			r2 = int(playerX) - int(playerX) % TILE_SIZE;
		}
		else {
			r2 = 0;
		}

		float r;
		if (ang < pi) {
			r = (TILE_SIZE - int(playerY) % TILE_SIZE) + int(playerY);
		}
		else if (ang > pi) {
			r = int(playerY) - int(playerY) % TILE_SIZE;
		}
		else {
			r = 0;
		}

		float w = r - playerY;
		float xdist = w / tan(ang);

		float w2 = r2 - playerX;
		float ydist = w2 * tan(ang);

		int dof = 0;

		float texOffsetX;
		float texOffsetY;

		//check horizontal walls
		float texOffsetXhor;
		float texOffsetYhor;
		while (dof < MAP_TILE_COUNT) {
			int mapj = (TILE_SIZE - int(playerX + xdist) % TILE_SIZE) + int(playerX + xdist);
			int mapi = r;

			mapj /= TILE_SIZE;
			mapi /= TILE_SIZE;

			mapj -= 1;
			if (ang < pi)
				mapi += 1;
			mapi = MAP_TILE_COUNT - mapi;

			if (mapj < MAP_TILE_COUNT && mapi < MAP_TILE_COUNT && mapi > -1 && mapj > -1 && map.map[mapi][mapj] != 0) {
				dof = MAP_TILE_COUNT;

				switch (map.map[mapi][mapj]) {
				case 1: {
					texOffsetXhor = 0;
					texOffsetYhor = 0.5;
					break;
				}
				case 2: {
					texOffsetXhor = 32;
					texOffsetYhor = 0.5;
					break;
				}
				case 3: {
					texOffsetXhor = 64;
					texOffsetYhor = 0.5;
					break;
				}
				case 4: {
					texOffsetXhor = 96;
					texOffsetYhor = 0.5;
					break;
				}
				case 5: {
					texOffsetXhor = 128;
					texOffsetYhor = 0.5;
					break;
				}
				case 6: {
					texOffsetXhor = 160;
					texOffsetYhor = 0.5;
					break;
				}
				case 7: {
					texOffsetXhor = 0;
					texOffsetYhor = 0;
					break;
				}
				case 8: {
					texOffsetXhor = 32;
					texOffsetYhor = 0;
					break;
				}
				case 9: {
					texOffsetXhor = 64;
					texOffsetYhor = 0;
					break;
				}
				case 10: {
					texOffsetXhor = 96;
					texOffsetYhor = 0;
					break;
				}
				case 11: {
					texOffsetXhor = 128;
					texOffsetYhor = 0;
					break;
				}
				case 12: {
					texOffsetXhor = 160;
					texOffsetYhor = 0;
					break;
				}
				}
			}
			else {
				if (ang < pi)
					r += TILE_SIZE;
				else
					r -= TILE_SIZE;
				xdist = (r - playerY) / tan(ang);
				dof += 1;
			}
		}

		//check vertical walls
		dof = 0;
		float texOffsetXvert;
		float texOffsetYvert;
		while (dof < MAP_TILE_COUNT) {
			int mapy = (TILE_SIZE - int(playerY + ydist) % TILE_SIZE) + int(playerY + ydist);
			int mapx = r2;

			mapx /= TILE_SIZE;
			mapy /= TILE_SIZE;

			if (ang > pi / 2 && ang < 3 * pi / 2)
				mapx -= 1;
			mapy = MAP_TILE_COUNT - mapy;

			if (mapx < MAP_TILE_COUNT && mapy < MAP_TILE_COUNT && mapx > -1 && mapy > -1 && map.map[mapy][mapx] != 0) {
				dof = MAP_TILE_COUNT;

				switch (map.map[mapy][mapx]) {
				case 1: {
					texOffsetXvert = 0;
					texOffsetYvert = 0.5;
					break;
				}
				case 2: {
					texOffsetXvert = 32;
					texOffsetYvert = 0.5;
					break;
				}
				case 3: {
					texOffsetXvert = 64;
					texOffsetYvert = 0.5;
					break;
				}
				case 4: {
					texOffsetXvert = 96;
					texOffsetYvert = 0.5;
					break;
				}
				case 5: {
					texOffsetXvert = 128;
					texOffsetYvert = 0.5;
					break;
				}
				case 6: {
					texOffsetXvert = 160;
					texOffsetYvert = 0.5;
					break;
				}
				case 7: {
					texOffsetXvert = 0;
					texOffsetYvert = 0;
					break;
				}
				case 8: {
					texOffsetXvert = 32;
					texOffsetYvert = 0;
					break;
				}
				case 9: {
					texOffsetXvert = 64;
					texOffsetYvert = 0;
					break;
				}
				case 10: {
					texOffsetXvert = 96;
					texOffsetYvert = 0;
					break;
				}
				case 11: {
					texOffsetXvert = 128;
					texOffsetYvert = 0;
					break;
				}
				case 12: {
					texOffsetXvert = 160;
					texOffsetYvert = 0;
					break;
				}
				}
			}
			else {
				if (ang < pi / 2 || ang > 3 * pi / 2)
					r2 += TILE_SIZE;
				else if (ang > pi / 2 || ang < 3 * pi / 2)
					r2 -= TILE_SIZE;
				ydist = (r2 - playerX) * tan(ang);
				dof += 1;
			}
		}

		float vertLen;
		float horLen;

		horLen = sqrt(pow(playerX - (playerX + xdist), 2) + pow(playerY - r, 2));
		vertLen = sqrt(pow(playerX - r2, 2) + pow(playerY - (playerY + ydist), 2));

		float newX, newY;

		float ray[4] = {
				playerX, playerY,
				playerX + xdist, r
		};

		float ray2[4] = {
			playerX, playerY,
			r2, playerY + ydist
		};

		float lineDist;

		if (horLen < vertLen) {
			lineDist = horLen;
			lines[index] = playerX;
			lines[index + 1] = playerY;
			lines[index + 2] = (playerX + xdist);
			lines[index + 3] = r;
			texOffsetX = texOffsetXhor;
			texOffsetY = texOffsetYhor;
		}

		else {
			lineDist = vertLen;
			lines[index] = playerX;
			lines[index + 1] = playerY;
			lines[index + 2] = r2;
			lines[index + 3] = (playerY + ydist);
			texOffsetX = texOffsetXvert;
			texOffsetY = texOffsetYvert;
		}

		float ca = a;
		if (ca < 0)
			ca += 2 * pi;
		if (ca > 2 * pi)
			ca -= 2 * pi;

		lineDist *= cos(ca);
		zBuffer[int(i) + HALF_GAME_WIDTH] = lineDist;

		float lineHeight = (64 / lineDist) * HALF_GAME_WIDTH;

		if (lineHeight > 5000)
			lineHeight = 5000;

		float lineOffset = HALF_GAME_HEIGHT - lineHeight / 2;

		if (horLen < vertLen) {
			float texcoordX = int(playerX + xdist) % TILE_SIZE;
			float texcoordY = texOffsetY;
			texcoordX += texOffsetX;
			texcoordX *= (0.0052083);

			texWalls[horindex] = (GAME_MIDPOINT + i);
			texWalls[horindex + 1] = lineOffset;

			texWalls[horindex + 2] = texcoordX;
			texWalls[horindex + 3] = texcoordY;

			texWalls[horindex + 4] = (GAME_MIDPOINT + i + 1);
			texWalls[horindex + 5] = lineOffset;

			texWalls[horindex + 6] = texcoordX;
			texWalls[horindex + 7] = texcoordY;

			texWalls[horindex + 8] = (GAME_MIDPOINT + i + 1);
			texWalls[horindex + 9] = (lineHeight + lineOffset);

			texWalls[horindex + 10] = texcoordX;
			texWalls[horindex + 11] = texcoordY + 0.5;

			texWalls[horindex + 12] = (GAME_MIDPOINT + i);
			texWalls[horindex + 13] = (lineHeight + lineOffset);

			texWalls[horindex + 14] = texcoordX;
			texWalls[horindex + 15] = texcoordY + 0.5;

			horindex += 16;
			horsize += 16;
		}
		else {
			float texcoordX = int(playerY + ydist) % TILE_SIZE;
			float texcoordY = texOffsetY;
			texcoordX += texOffsetX;
			texcoordX *= (0.0052083);

			texWalls[horindex] = (GAME_MIDPOINT + i);
			texWalls[horindex + 1] = lineOffset;

			texWalls[horindex + 2] = texcoordX;
			texWalls[horindex + 3] = texcoordY;

			texWalls[horindex + 4] = (GAME_MIDPOINT + i + 1);
			texWalls[horindex + 5] = lineOffset;

			texWalls[horindex + 6] = texcoordX;
			texWalls[horindex + 7] = texcoordY;

			texWalls[horindex + 8] = (GAME_MIDPOINT + i + 1);
			texWalls[horindex + 9] = (lineHeight + lineOffset);

			texWalls[horindex + 10] = texcoordX;
			texWalls[horindex + 11] = texcoordY + 0.5;

			texWalls[horindex + 12] = (GAME_MIDPOINT + i);
			texWalls[horindex + 13] = (lineHeight + lineOffset);

			texWalls[horindex + 14] = texcoordX;
			texWalls[horindex + 15] = texcoordY + 0.5;

			horindex += 16;
			horsize += 16;
		}
		index += 4;

		float dirCos = cos(ang);
		float dirSin = sin(ang);

		float correction = 1 / cos(a);
		for (int j = 0; j < lineOffset; j++) {
			float pixelDist = distances[j];

			pixelDist *= 45;
			pixelDist *= correction;

			float pixelX = pixelDist * dirCos;
			float pixelY = pixelDist * dirSin;

			pixelX += playerX;
			pixelY += playerY;

			pixelX = int(pixelX);
			pixelY = int(pixelY);

			float texX = pixelX * 0.00078125;
			float texY = pixelY * 0.00078125;

			texFloorCeil[floorIndex] = float(GAME_MIDPOINT) + i;
			texFloorCeil[floorIndex + 1] = float(j);

			texFloorCeil[floorIndex + 2] = texX;
			texFloorCeil[floorIndex + 3] = texY;

			texFloorCeil[floorIndex + 4] = float(GAME_MIDPOINT) + i;
			texFloorCeil[floorIndex + 5] = float(j) + 1;

			texFloorCeil[floorIndex + 6] = texX;
			texFloorCeil[floorIndex + 7] = texY;


			texFloorCeil[floorIndex + 8] = float(GAME_MIDPOINT) + i;
			texFloorCeil[floorIndex + 9] = 780 - float(j);

			texFloorCeil[floorIndex + 10] = texX + 0.5;
			texFloorCeil[floorIndex + 11] = texY;

			texFloorCeil[floorIndex + 12] = float(GAME_MIDPOINT) + i;
			texFloorCeil[floorIndex + 13] = 780 - float(j) - 1;

			texFloorCeil[floorIndex + 14] = texX + 0.5;
			texFloorCeil[floorIndex + 15] = texY;

			floorIndex += 16;
		}
	}
	angle += rot;

	//draw rays
	rays.Update(lines, 2080 * 2 * sizeof(float));
	lvao.Bind();
	glDrawArrays(GL_LINES, 0, 2080);

	//Ceiling and floor
	texShader.Bind();
	texShader.SetUniform1i("u_Texture", 0);
	texShader.SetUniformMat4f("u_MVP", mvp2);

	floorceiling.Bind();

	VertexArray fcvao;
	VertexBufferLayout fclayout;
	fclayout.Push<float>(2); // two floats for vertex position
	fclayout.Push<float>(2); // two floats for texture coordinates

	fcvb.Update(texFloorCeil, 6489600 * sizeof(float));
	fcvao.AddBuffer(fcvb, fclayout);
	fcvao.Bind();
	glDrawArrays(GL_LINES, 0, 3244800);

	//draw walls
	brick.Bind();

	VertexArray wallvao;
	wallvb.Update(texWalls, horsize * sizeof(float));

	VertexBufferLayout walllayout;
	walllayout.Push<float>(2);
	walllayout.Push<float>(2);
	wallvao.AddBuffer(wallvb, walllayout);

	int offset = 0;
	for (int i = 0; i < 6240; i += 6) {
		ind[i + 0] = 0 + offset;
		ind[i + 1] = 1 + offset;
		ind[i + 2] = 2 + offset;

		ind[i + 3] = 2 + offset;
		ind[i + 4] = 3 + offset;
		ind[i + 5] = 0 + offset;

		offset += 4;
	}

	IndexBuffer wallib(ind, 6240);
	wallib.Bind();
	wallvao.Bind();

	glDrawElements(GL_TRIANGLES, wallib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Player::DrawSprites(Object2d& o, Renderer renderer, glm::mat4 proj) {

	/*
	float dirCos = cos(angle);
	float dirSin = sin(angle);

	//float dist = sqrt(pow((o.screenCenterX - playerX), 2) + pow((o.screenCenterY - playerY), 2));

	float sx = o.screenCenterX - playerX;
	float sy = o.screenCenterY - playerY;
	float sz = 390;

	float nx = sx * dirCos - sy * dirSin;
	float ny = sx * dirSin + sy * dirCos;

	sx = nx;
	sy = ny;

	std::cout << sx << " " << sy << std::endl;
	//sx = (sx * 468 / sy) + (520 / 2) + 640;
	//sy = (sz * 468 / sy) + (390 / 2);
	sx += 320;
	sy += 320;

	float dist = sy;
	*/

	//float oangle = (atan2f(playerY - o.screenCenterY, playerX - o.screenCenterX));

	std::fill_n(o.bufferData, 8320, 0);
	int sIndex = 0;

	float ox = o.centerX - playerX;
	float oy = o.centerY - playerY;

	float px = sin(angle);
	float py = cos(angle);

	float oangle = atan2f(o.centerY - playerY, o.centerX - playerX) + (pi / 2 - angle);
	float odist = sqrt(pow(o.centerX - playerX, 2) + pow(o.centerY - playerY, 2));

	if (oangle < 0)
		oangle += 2 * pi;
	if (oangle > 2 * pi)
		oangle -= 2 * pi;

	float sx = tan(oangle - pi / 2);

	sx *= HALF_GAME_WIDTH;

	float ca = oangle - (pi / 2);

	if (ca < 0)
		ca += 2 * pi;
	if (ca > 2 * pi)
		ca -= 2 * pi;

	odist *= cos(ca);

	float sh = (64 / odist) * HALF_GAME_WIDTH / 2;
	float sw = (64 / odist) * HALF_GAME_WIDTH / 2;

	if (sh >= GAME_WIDTH)
		sh = GAME_WIDTH;

	if (sw >= GAME_WIDTH)
		sw = GAME_WIDTH;

	float sy = HALF_GAME_HEIGHT - sh;

	float screen[] = {
		GAME_MIDPOINT - sx, sy, 0.0f, 0.0f,
		GAME_MIDPOINT - sx + 32, sy, 1.0f, 0.0f,
		GAME_MIDPOINT - sx + 32, sy + sh, 1.0f, 1.0f,
		GAME_MIDPOINT - sx, sy + sh, 0.0f, 1.0f,
	};

	float screen3[8320];
	int start = -sw / 2;
	int end = -(start);
	float texStep = 64 / sw / 2;
	float texCoord = 0.015625;
	float step = 0;

	for (int i = start; i < end; ++i) {
		int bufferIndex = HALF_GAME_WIDTH - sx + i;

		if (bufferIndex >= 0 && bufferIndex < GAME_WIDTH)
		{
			if (zBuffer[bufferIndex] > odist)
			{
				o.bufferData[sIndex] = GAME_MIDPOINT - sx + i;
				o.bufferData[sIndex + 1] = sy;

				o.bufferData[sIndex + 4] = GAME_MIDPOINT - sx + i;
				o.bufferData[sIndex + 5] = sy + sh;

				if (o.IsHit()) {
					o.bufferData[sIndex + 2] = step * texCoord + 0.5;
					o.bufferData[sIndex + 3] = 0.0f;

					o.bufferData[sIndex + 6] = step * texCoord + 0.5;
					o.bufferData[sIndex + 7] = 1.0f;
				}
				else {
					o.bufferData[sIndex + 2] = step * texCoord;
					o.bufferData[sIndex + 3] = 0.0f;

					o.bufferData[sIndex + 6] = step * texCoord;
					o.bufferData[sIndex + 7] = 1.0f;
				}
				sIndex += 8;
			}
		}
		step += texStep;
	}

	/*
	*billboard sprite code that needs to be refactored

	GLuint oind[] = {
		0, 1, 2,
		2, 3, 0
	};
	Texture sprite("res/Textures/barrel.png");
	sprite.Bind();

	VertexBufferLayout slayout;
	slayout.Push<float>(2);
	slayout.Push<float>(2);

	VertexArray ovao;
	VertexBuffer ovb(o.bufferData, 8320 * sizeof(float));
	ovb.Bind();
	ovao.AddBuffer(ovb, slayout);

	//IndexBuffer oib(oind, 6);
	//oib.Bind();
	//ovao.Bind();
	*/

	glDrawArrays(GL_LINES, 0, 2080);

}