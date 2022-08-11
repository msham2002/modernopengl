#include "Player.h"

Player::Player() /*: vb(positions, 4 * 4 * sizeof(float))*/ {
}


Player::Player(Shader& shader, VertexBufferLayout& layout, VertexBufferLayout& llayout, Map map) :rifle("res/Textures/rifle.png"), gunvb(gunPos, 16 * sizeof(float)), wallvb(texWalls, 16640 * sizeof(float)), 
																								fcvb(texFloorCeil, 540 * 390 * 8 * 2 * 2 * sizeof(float)), rays(lines, 2080 * 2 * sizeof(float)), ib(indices, 6), 
																								vb(positions, 4 * 4 * sizeof(float)), texture("res/Textures/placeholder2.png"), brick("res/Textures/walls.png"), 
																								floorceiling("res/Textures/floorceiling.png"),  lib(rayind, 2), rayShader("res/shaders/Ray.shader"), texShader("res/shaders/Basic.shader")/*, lvb(ray, 2 * 2 * sizeof(float))*/
{
	this->shader = shader;
	this->layout = layout;
	this->llayout = llayout;
	dx = 0.0f, dy = 0.0f;
	angle = 90.0f * pi / 180, rot = 0.075f;
	this->map = map;
	lines = new float[8320];
	texWalls = new float[16640];
	texFloors = new float[3369600];
	texCeil = new float[3369600];
	texFloorCeil = new float[6489600];
	tempsprite = new float[8320];
}

Player::~Player()
{
	delete[] lines;
	delete[] texWalls;
	delete[] texFloors;
	delete[] texCeil;
	delete[] texFloorCeil;
	delete[] tempsprite;
}


void Player::AddBuffer() {
	vao.AddBuffer(vb, layout);
}


void Player::Bind() {
	texture.Bind();
}


void Player::Draw(Renderer renderer, glm::mat4& proj) {
	glm::mat4 mvp = proj * view;
	shader.SetUniformMat4f("u_MVP", mvp);
	//renderer.Draw(vao, ib, shader);
	
	texLine2(proj);
	//Line(proj);
}

void Player::HandleEvent(SDL_Event& e) {
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
		case SDLK_SPACE:
			gunPos[2] = 0.5f;
			gunPos[6] = 1.0f;
			gunPos[10] = 1.0f;
			gunPos[14] = 0.5f;
			press = true;
			time = SDL_GetTicks();
			break;
		}
	}
}

void Player::Move() {

	//std::cout << trunc(20 - cy / 32) << " " << trunc(cx / 32) << std::endl;

	//int iInd = trunc(20 - cy / 32);
	//int jInd = trunc(cx / 32);
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
		cx = oldx;
		cy = oldy;
	}
	else {
		view = glm::translate(view, glm::vec3(dx, dy, 0));
		cx += dx2;
		cy += dy2;
	}
	*/
	//std::cout << trunc((cx + dx2) / 32) << " " <<  jInd << std::endl;

	

	view = glm::translate(view, glm::vec3(dx, dy, 0));
	cx += dx2;
	cy += dy2;

	if (angle < 0)
		angle += 2 * pi;

	if (angle > 2 * pi)
		angle -= 2 * pi;

	oldx = cx - 1.1 * dx;
	oldy = cy - 1.1 * dy;
	
}

void Player::Keyboard(const Uint8* keystate) {
	const Uint8 up = keystate[SDL_SCANCODE_W];
	const Uint8 down = keystate[SDL_SCANCODE_S];
	const Uint8 left = keystate[SDL_SCANCODE_A];
	const Uint8 right = keystate[SDL_SCANCODE_D];
	const Uint8 lookL = keystate[SDL_SCANCODE_LEFT];
	const Uint8 lookR = keystate[SDL_SCANCODE_RIGHT];
	//const Uint8 shoot = keystate[SDL_SCANCODE_SPACE];

	//standard
	if (up) {
		dx = 0;
		dy = 3.0;
		dx2 = 3.0 * cos(angle);
		dy2 = 3.0 * sin(angle);
	}
	if (down) {
		dx = 0;
		dy = -3.0;
		dx2 = -3.0 * cos(angle);
		dy2 = -3.0 * sin(angle);
	}
	if (left) {
		dx = -3.0;
		dy = 0;
		dx2 = 3.0 * cos(angle + (90 * pi / 180));
		dy2 = 3.0 * sin(angle + (90 * pi / 180));

	}
	if (right) {
		dx = 3.0;
		dy = 0;
		dx2 = -3.0 * cos(angle + (90 * pi / 180));
		dy2 = -3.0 * sin(angle + (90 * pi / 180));
	}

	//diagonal
	if (up && right) {
		dx = 3.0 * sin(45 * pi / 180);
		dy = 3.0 * cos(45 * pi / 180);
	}
	if (up && left) {
		dx = -3.0 * sin(45 * pi / 180);
		dy = 3.0 * cos(45 * pi / 180);
	}
	if (down && right) {
		dx = 3.0 * sin(45 * pi / 180);
		dy = -3.0 * cos(45 * pi / 180);
	}
	if (down && left) {
		dx = -3.0 * sin(45 * pi / 180);
		dy = -3.0 * cos(45 * pi / 180);
	}

	/*
	//3 key
	if (left && right && up) {
		dy = 3.0 * sin((angle)*pi / 180);
		dx = 3.0 * cos((angle)*pi / 180);
	}
	else if (left && right && down) {
		dy = -3.0 * sin((angle)*pi / 180);
		dx = -3.0 * cos((angle)*pi / 180);
	}
	else if (left && right) {
		dy = 0;
		dx = 0;
	}
	*/

	/*
	//cancel
	if (up && down) {
		dy = 0;
		dx = 0;
	}
	*/

	//no keys
	if (!up && !down && !left && !right) {
		dy = 0;
		dx = 0;
		dx2 = 0.0f;
		dy2 = 0.0f;
	}

	//look
	if (lookL) {
		view = glm::translate(view, glm::vec3(centerX, centerY, 0));
		view = glm::rotate(view, 0.075f, glm::vec3(0, 0, 1));
		view = glm::translate(view, glm::vec3(-centerX, -centerY, 0));
		rot = 0.075f;
	}
	if (lookR) {
		view = glm::translate(view, glm::vec3(centerX, centerY, 0));
		view = glm::rotate(view, -0.075f, glm::vec3(0, 0, 1));
		view = glm::translate(view, glm::vec3(-centerX, -centerY, 0));
		rot = -0.075f;
	}
	if (lookL && lookR) {
		rot = 0;
	}
	if (!lookL && !lookR) {
		rot = 0;
	}
}

//delete this
void Player::Line(glm::mat4& proj) {
	glm::mat4 mvp2 = proj * line;

	rayShader.Bind();
	rayShader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
	rayShader.SetUniformMat4f("u_MVP", mvp2);

	float a = -30 * pi / 180;

	int index = 0;
	int horindex = 0;
	int vertindex = 0;

	int horsize = 0;
	int vertsize = 0;

	for (int i = 0; i < 1040; ++i) {

		a += 0.05769 * pi / 180;
		if (angle + a < 0)
			angle += 2 * pi;
		if (angle + a > 2 * pi)
			angle -= 2 * pi;

		float r2;
		if (angle + a < pi / 2 || angle + a > 3 * pi / 2) {
			r2 = (64 - int(cx) % 64 + int(cx));
		}
		else if (angle + a > pi / 2 || angle + a < 3 * pi / 2) {
			r2 = int(cx) - int(cx) % 64;
		}
		else {
			r2 = 0;
		}

		float r;
		if (angle + a < pi) {
			r = (64 - int(cy) % 64) + int(cy); // nearest 64th value
		}
		else if (angle + a > pi) {
			r = int(cy) - int(cy) % 64;
		}
		else {
			r = 0;
		}

		float w = r - cy; // distance between r and player center
		float xdist = w / tan(angle + a);

		float w2 = r2 - cx;
		float ydist = w2 * tan(angle + a);

		int dof = 0;

		//check horizontal walls
		while (dof < 10) {
			int mapj = (64 - int(cx + xdist) % 64) + int(cx + xdist);
			int mapi = r;

			mapj /= 64;
			mapi /= 64;

			mapj -= 1;
			if (angle + a < pi)
				mapi += 1;
			mapi = 10 - mapi;

			if (mapj < 10 && mapi < 10 && mapi > -1 && mapj > -1 && map.map[mapi][mapj] == 1) {
				dof = 10;
			}
			else {
				if (angle + a < pi)
					r += 64;
				else
					r -= 64;
				xdist = (r - cy) / tan(angle + a);
				dof += 1;
			}
		}


		//check vertical walls
		dof = 0;

		while (dof < 10) {
			int mapy = (64 - int(cy + ydist) % 64) + int(cy + ydist);
			int mapx = r2;

			mapx /= 64;
			mapy /= 64;

			if (angle + a > pi / 2 && angle + a < 3 * pi / 2)
				mapx -= 1;
			mapy = 10 - mapy;

			if (mapx < 10 && mapy < 10 && mapx > -1 && mapy > -1 && map.map[mapy][mapx] == 1) {
				dof = 10;
			}
			else {
				if (angle + a < pi / 2 || angle + a > 3 * pi / 2)
					r2 += 64;
				else if (angle + a > pi / 2 || angle + a < 3 * pi / 2)
					r2 -= 64;
				ydist = (r2 - cx) * tan(angle + a);
				dof += 1;
			}
		}

		float vertLen;
		float horLen;

		horLen = sqrt(pow(cx - (cx + xdist), 2) + pow(cy - r, 2));
		vertLen = sqrt(pow(cx - r2, 2) + pow(cy - (cy + ydist), 2));

		float newX, newY;

		float ray[4] = {
				cx, cy,
				cx + xdist, r
		};

		float ray2[4] = {
			cx, cy,
			r2, cy + ydist
		};

		float lineDist;

		if (horLen < vertLen) {
			lineDist = horLen;
			lines[index] = cx;
			lines[index + 1] = cy;
			lines[index + 2] = (cx + xdist);
			lines[index + 3] = r;
		}

		else {
			lineDist = vertLen;
			lines[index] = cx;
			lines[index + 1] = cy;
			lines[index + 2] = r2;
			lines[index + 3] = (cy + ydist);
		}


		//calculate wall heights
		float ca = angle - angle + a;
		if (ca < 0)
			ca += 2 * pi;
		if (ca > 2 * pi)
			ca -= 2 * pi;

		lineDist *= cos(ca);

		//float lineHeight = (60 * 1040) / lineDist;
		float lineHeight = (64 / lineDist) * 900;
		if (lineHeight > 5000)
			lineHeight = 1040;

		float lineOffset = 390 - lineHeight / 2;

		float wallarr[4] = {
			i + 640, lineOffset,
			i + 640, lineHeight + lineOffset
		};

		if (horLen < vertLen) {
			horwalls[horindex] = (1680 - i);
			horwalls[horindex + 1] = lineOffset;
			horwalls[horindex + 2] = (1680 - i);
			horwalls[horindex + 3] = (lineHeight + lineOffset);
			horindex += 4;
			horsize++;
		}
		else {
			vertwalls[vertindex] = (1680 - i);
			vertwalls[vertindex + 1] = lineOffset;
			vertwalls[vertindex + 2] = (1680 - i);
			vertwalls[vertindex + 3] = (lineHeight + lineOffset);
			vertindex += 4;
			vertsize++;
		}
		index += 4;
	}

	angle += rot;

	//draw rays
	VertexBuffer rays(lines, 2080 * 2 * sizeof(float));
	lvao.AddBuffer(rays, llayout);
	lvao.Bind();
	glDrawArrays(GL_LINES, 0, 2080);

	//draw walls
	rayShader.SetUniform4f("u_Color", 0.5f, 0.5f, 0.5f, 1.0f);
	rayShader.SetUniformMat4f("u_MVP", mvp2);

	VertexBuffer horwall(horwalls, 2080 * 2 * sizeof(float));
	lvao.AddBuffer(horwall, llayout);
	lvao.Bind();
	glDrawArrays(GL_LINES, 0, horsize * 2);

	rayShader.SetUniform4f("u_Color", 0.75f, 0.75f, 0.75f, 1.0f);
	rayShader.SetUniformMat4f("u_MVP", mvp2);

	VertexBuffer vertwall(vertwalls, 2080 * 2 * sizeof(float));
	lvao.AddBuffer(vertwall, llayout);
	lvao.Bind();
	glDrawArrays(GL_LINES, 0, vertsize * 2);
}

void Player::texLine2(glm::mat4& proj) {
	glm::mat4 mvp2 = proj * line;

	rayShader.Bind();
	rayShader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
	rayShader.SetUniformMat4f("u_MVP", mvp2);

	float a = 0 * pi / 180;
	float add = 0.0f;

	int index = 0;
	int horindex = 0;
	int vertindex = 0;
	int floorIndex = 0;
	int floorCeilIndex = 0;

	int horsize = 0;
	int vertsize = 0;

	//do this process twice, once for each half of screen

	//std::fill_n(texFloorCeil, 6489600, 0);
	texFloorCeil = new float[6489600];

	for (float i = 0; i < 520; i++) {
		//a += (1 * 0.057692307) * pi / 180;
		add = (atan(i/520));
		//std::cout << add << " ";
		if (i > 0)
			add = add - atan((i - 1) / 520);
		//std::cout << add << " ";
		//atan(i - 1 / 900))
		
		/*
		if (i > 519)
			a -= add;
		else
			a += add;
			*/

		a += add;

		//std::cout << a << " " << i << std::endl;

		float ang = angle + a;

		if (ang < 0)
			ang += 2 * pi;
		if (ang > 2 * pi)
			ang -= 2 * pi;


		float r2;
		if (ang < pi / 2 || ang > 3 * pi / 2) {
			r2 = (32 - int(cx) % 32 + int(cx));
		}
		else if (ang > pi / 2 || ang < 3 * pi / 2) {
			r2 = int(cx) - int(cx) % 32;
		}
		else {
			r2 = 0;
		}

		float r;
		if (ang < pi) {
			r = (32 - int(cy) % 32) + int(cy); // nearest 32nd value
		}
		else if (ang > pi) {
			r = int(cy) - int(cy) % 32;
		}
		else {
			r = 0;
		}

		float w = r - cy; // distance between r and player center
		float xdist = w / tan(ang);

		float w2 = r2 - cx;
		float ydist = w2 * tan(ang);

		int dof = 0;

		float texOffsetX;
		float texOffsetY;
		//check horizontal walls
		float texOffsetXhor;
		float texOffsetYhor;
		while (dof < 20) {
			int mapj = (32 - int(cx + xdist) % 32) + int(cx + xdist);
			int mapi = r;

			mapj /= 32;
			mapi /= 32;

			mapj -= 1;
			if (ang < pi)
				mapi += 1;
			mapi = 20 - mapi;

			if (mapj < 20 && mapi < 20 && mapi > -1 && mapj > -1 && map.map[mapi][mapj] != 0) {
				dof = 20;
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
					r += 32;
				else
					r -= 32;
				xdist = (r - cy) / tan(ang);
				dof += 1;
			}
		}


		//check vertical walls
		dof = 0;
		float texOffsetXvert;
		float texOffsetYvert;
		while (dof < 20) {
			int mapy = (32 - int(cy + ydist) % 32) + int(cy + ydist);
			int mapx = r2;

			mapx /= 32;
			mapy /= 32;

			if (ang > pi / 2 && ang < 3 * pi / 2)
				mapx -= 1;
			mapy = 20 - mapy;

			if (mapx < 20 && mapy < 20 && mapx > -1 && mapy > -1 && map.map[mapy][mapx] != 0) {
				dof = 20;
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
					r2 += 32;
				else if (ang > pi / 2 || ang < 3 * pi / 2)
					r2 -= 32;
				ydist = (r2 - cx) * tan(ang);
				dof += 1;
			}
		}

		float vertLen;
		float horLen;

		horLen = sqrt(pow(cx - (cx + xdist), 2) + pow(cy - r, 2));
		vertLen = sqrt(pow(cx - r2, 2) + pow(cy - (cy + ydist), 2));

		float newX, newY;

		float ray[4] = {
				cx, cy,
				cx + xdist, r
		};

		float ray2[4] = {
			cx, cy,
			r2, cy + ydist
		};

		float lineDist;

		if (horLen < vertLen) {
			lineDist = horLen;
			lines[index] = cx;
			lines[index + 1] = cy;
			lines[index + 2] = (cx + xdist);
			lines[index + 3] = r;
			texOffsetX = texOffsetXhor;
			texOffsetY = texOffsetYhor;
		}

		else {
			lineDist = vertLen;
			lines[index] = cx;
			lines[index + 1] = cy;
			lines[index + 2] = r2;
			lines[index + 3] = (cy + ydist);
			texOffsetX = texOffsetXvert;
			texOffsetY = texOffsetYvert;
		}


		//calculate wall heights
		float ca = a;
		if (ca < 0)
			ca += 2 * pi;
		if (ca > 2 * pi)
			ca -= 2 * pi;

			
		lineDist *= cos(ca);
		zBuffer[519 - int(i)] = lineDist;

		float lineHeight = (64 / lineDist) * 520;
		//float lineHeight = (60 * 1040) / lineDist;
		if (lineHeight > 5000)
			lineHeight = 5000;

		float lineOffset = 390 - lineHeight / 2;
		float texOffset;

		
		if (horLen < vertLen) {
			float texcoordX = int(cx + xdist) % 32;
			float texcoordY = texOffsetY;
			texcoordX += texOffsetX;
			texcoordX *= (0.0052083);
			
			// wall coordinates
			texWalls[horindex] = (1160 - i);
			texWalls[horindex + 1] = lineOffset;

			//texture coordinates
			texWalls[horindex + 2] = texcoordX;
			texWalls[horindex + 3] = texcoordY;

			texWalls[horindex + 4] = (1160 - i + 1);
			texWalls[horindex + 5] = lineOffset;

			texWalls[horindex + 6] = texcoordX;
			texWalls[horindex + 7] = texcoordY;

			texWalls[horindex + 8] = (1160 - i + 1);
			texWalls[horindex + 9] = (lineHeight + lineOffset);

			texWalls[horindex + 10] = texcoordX;
			texWalls[horindex + 11] = texcoordY + 0.5;

			texWalls[horindex + 12] = (1160 - i);
			texWalls[horindex + 13] = (lineHeight + lineOffset);

			texWalls[horindex + 14] = texcoordX;
			texWalls[horindex + 15] = texcoordY + 0.5;

			
			horindex += 16;
			horsize += 16;
		}
		else {
			float texcoordX = int(cy + ydist) % 32;
			float texcoordY = texOffsetY;
			texcoordX += texOffsetX;
			texcoordX *= (0.0052083);

			texWalls[horindex] = (1160 - i);
			texWalls[horindex + 1] = lineOffset;

			texWalls[horindex + 2] = texcoordX;
			texWalls[horindex + 3] = texcoordY;

			texWalls[horindex + 4] = (1160 - i + 1); //possibly change to -1 instead of +1 probably not though because think about it but i'll leave it here just in case im actually stupid
			texWalls[horindex + 5] = lineOffset;

			texWalls[horindex + 6] = texcoordX;
			texWalls[horindex + 7] = texcoordY;

			texWalls[horindex + 8] = (1160 - i + 1);
			texWalls[horindex + 9] = (lineHeight + lineOffset);

			texWalls[horindex + 10] = texcoordX;
			texWalls[horindex + 11] = texcoordY + 0.5;

			texWalls[horindex + 12] = (1160 - i);
			texWalls[horindex + 13] = (lineHeight + lineOffset);

			texWalls[horindex + 14] = texcoordX;
			texWalls[horindex + 15] = texcoordY + 0.5;


			horindex += 16;
			horsize += 16;
		}
		index += 4;
		
		float dirCos = cos(ang);
		float dirSin = sin(ang);


		//--
		/*
		float first = 1;
		float last = 390;
		
		//first *= 45;
		//last *= 45;

		first /= cos(a);
		last /= cos(a);

		float firstX = first * dirCos;
		float firstY = first * dirSin;

		float lastX = last * dirCos;
		float lastY = last * dirSin;

		firstX += cx;
		firstY += cy;

		lastX += cx;
		lastY += cy;

		firstX = int(firstX);
		firstY = int(firstY);

		lastX = int(lastX);
		lastY = int(lastY);

		float firstTexX = firstX * 0.00078125;
		float firstTexY = firstY * 0.00078125;

		float lastTexX = lastX * 0.00078125;
		float lastTexY = lastY * 0.00078125;

		texFloorCeil[floorIndex] = float(1160) - i;
		texFloorCeil[floorIndex + 1] = 1;
		
		texFloorCeil[floorIndex + 2] = firstTexX;
		texFloorCeil[floorIndex + 3] = firstTexY;
		
		texFloorCeil[floorIndex + 4] = float(1160) - i;
		texFloorCeil[floorIndex + 5] = 390;

		texFloorCeil[floorIndex + 6] = lastTexX;
		texFloorCeil[floorIndex + 7] = lastTexY;
		//--
		*/
		
		//int off = lineOffset / 2;

		float correction = 1 / cos(a);
		for (int j = 0; j < lineOffset; j++) {
			
			float pixelDist = distances[j];
			//float pixelDist = 780  / (2 * (780 - float(j)) - 780);
			//float pixelDist = 1;
			//pixelDist *= 45;
			pixelDist *= correction;
			pixelDist *= 45;

			float pixelX = pixelDist * dirCos;
			float pixelY = pixelDist * dirSin;

			pixelX += cx;
			pixelY += cy;

			pixelX = int(pixelX);
			pixelY = int(pixelY);

			float texX = pixelX * 0.00078125;
			float texY = pixelY * 0.00078125;


			texFloorCeil[floorIndex] = float(1160) - i;
			texFloorCeil[floorIndex + 1] = float(j);

			texFloorCeil[floorIndex + 2] = texX;
			texFloorCeil[floorIndex + 3] = texY;

			texFloorCeil[floorIndex + 4] = float(1160) - i;
			texFloorCeil[floorIndex + 5] = float(j) + 1;

			texFloorCeil[floorIndex + 6] = texX;
			texFloorCeil[floorIndex + 7] = texY;

			
			texFloorCeil[floorIndex + 8] = float(1160) - i;
			texFloorCeil[floorIndex + 9] = 780 - float(j);
			
			texFloorCeil[floorIndex + 10] = texX + 0.5;
			texFloorCeil[floorIndex + 11] = texY;
			
			texFloorCeil[floorIndex + 12] = float(1160) - i;
			texFloorCeil[floorIndex + 13] = 780 - float(j) - 1;
			
			texFloorCeil[floorIndex + 14] = texX + 0.5;
			texFloorCeil[floorIndex + 15] = texY;
			

			//change floorindex to foorceilindex
			floorIndex += 16;
			floorCeilIndex += 16;
		}
		
		
	}
	
	a = 0.0f;
	add = 0.0f;
	for (float i = 0; i < 520; i++) {
		add = (atan(i / 520));
		
		if (i > 0)
			add = add - atan((i - 1) / 520);
	
		a -= add;

		float ang = angle + a;

		if (ang < 0)
			ang += 2 * pi;
		if (ang > 2 * pi)
			ang -= 2 * pi;

		float r2;
		if (ang < pi / 2 || ang > 3 * pi / 2) {
			r2 = (32 - int(cx) % 32 + int(cx));
		}
		else if (ang > pi / 2 || ang < 3 * pi / 2) {
			r2 = int(cx) - int(cx) % 32;
		}
		else {
			r2 = 0;
		}

		float r;
		if (ang < pi) {
			r = (32 - int(cy) % 32) + int(cy);
		}
		else if (ang > pi) {
			r = int(cy) - int(cy) % 32;
		}
		else {
			r = 0;
		}

		float w = r - cy;
		float xdist = w / tan(ang);

		float w2 = r2 - cx;
		float ydist = w2 * tan(ang);

		int dof = 0;

		float texOffsetX;
		float texOffsetY;

		//check horizontal walls
		float texOffsetXhor;
		float texOffsetYhor;
		while (dof < 20) {
			int mapj = (32 - int(cx + xdist) % 32) + int(cx + xdist);
			int mapi = r;

			mapj /= 32;
			mapi /= 32;

			mapj -= 1;
			if (ang < pi)
				mapi += 1;
			mapi = 20 - mapi;

			if (mapj < 20 && mapi < 20 && mapi > -1 && mapj > -1 && map.map[mapi][mapj] != 0) {
				dof = 20;

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
					r += 32;
				else
					r -= 32;
				xdist = (r - cy) / tan(ang);
				dof += 1;
			}
		}


		//check vertical walls
		dof = 0;
		float texOffsetXvert;
		float texOffsetYvert;
		while (dof < 20) {
			int mapy = (32 - int(cy + ydist) % 32) + int(cy + ydist);
			int mapx = r2;

			mapx /= 32;
			mapy /= 32;

			if (ang > pi / 2 && ang < 3 * pi / 2)
				mapx -= 1;
			mapy = 20 - mapy;

			if (mapx < 20 && mapy < 20 && mapx > -1 && mapy > -1 && map.map[mapy][mapx] != 0) {
				dof = 20;

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
					r2 += 32;
				else if (ang > pi / 2 || ang < 3 * pi / 2)
					r2 -= 32;
				ydist = (r2 - cx) * tan(ang);
				dof += 1;
			}
		}

		float vertLen;
		float horLen;

		horLen = sqrt(pow(cx - (cx + xdist), 2) + pow(cy - r, 2));
		vertLen = sqrt(pow(cx - r2, 2) + pow(cy - (cy + ydist), 2));

		float newX, newY;

		float ray[4] = {
				cx, cy,
				cx + xdist, r
		};

		float ray2[4] = {
			cx, cy,
			r2, cy + ydist
		};

		float lineDist;

		if (horLen < vertLen) {
			lineDist = horLen;
			lines[index] = cx;
			lines[index + 1] = cy;
			lines[index + 2] = (cx + xdist);
			lines[index + 3] = r;
			texOffsetX = texOffsetXhor;
			texOffsetY = texOffsetYhor;
		}

		else {
			lineDist = vertLen;
			lines[index] = cx;
			lines[index + 1] = cy;
			lines[index + 2] = r2;
			lines[index + 3] = (cy + ydist);
			texOffsetX = texOffsetXvert;
			texOffsetY = texOffsetYvert;
		}

		float ca = a;
		if (ca < 0)
			ca += 2 * pi;
		if (ca > 2 * pi)
			ca -= 2 * pi;

		lineDist *= cos(ca);
		zBuffer[int(i) + 520] = lineDist;

		float lineHeight = (64 / lineDist) * 520;
		//float lineHeight = (60 * 1040) / lineDist;
		if (lineHeight > 5000)
			lineHeight = 5000;

		float lineOffset = 390 - lineHeight / 2;

		if (horLen < vertLen) {
			float texcoordX = int(cx + xdist) % 32;
			float texcoordY = texOffsetY;
			texcoordX += texOffsetX;
			texcoordX *= (0.0052083);
		
			texWalls[horindex] = (1160 + i);
			texWalls[horindex + 1] = lineOffset;

			texWalls[horindex + 2] = texcoordX;
			texWalls[horindex + 3] = texcoordY;

			texWalls[horindex + 4] = (1160 + i + 1);
			texWalls[horindex + 5] = lineOffset;

			texWalls[horindex + 6] = texcoordX;
			texWalls[horindex + 7] = texcoordY;

			texWalls[horindex + 8] = (1160 + i + 1);
			texWalls[horindex + 9] = (lineHeight + lineOffset);

			texWalls[horindex + 10] = texcoordX;
			texWalls[horindex + 11] = texcoordY + 0.5;

			texWalls[horindex + 12] = (1160 + i);
			texWalls[horindex + 13] = (lineHeight + lineOffset);

			texWalls[horindex + 14] = texcoordX;
			texWalls[horindex + 15] = texcoordY + 0.5;


			horindex += 16;
			horsize += 16;
		}
		else {
			float texcoordX = int(cy + ydist) % 32;
			float texcoordY = texOffsetY;
			texcoordX += texOffsetX;
			texcoordX *= (0.0052083);
		
			texWalls[horindex] = (1160 + i);
			texWalls[horindex + 1] = lineOffset;

			texWalls[horindex + 2] = texcoordX;
			texWalls[horindex + 3] = texcoordY;

			texWalls[horindex + 4] = (1160 + i + 1);
			texWalls[horindex + 5] = lineOffset;

			texWalls[horindex + 6] = texcoordX;
			texWalls[horindex + 7] = texcoordY;

			texWalls[horindex + 8] = (1160 + i + 1);
			texWalls[horindex + 9] = (lineHeight + lineOffset);

			texWalls[horindex + 10] = texcoordX;
			texWalls[horindex + 11] = texcoordY + 0.5;

			texWalls[horindex + 12] = (1160 + i);
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
			//float pixelDist = 780  /  (2 * (780 - float(j)) - 780);
			float pixelDist = distances[j];
			//float pixelDist = 1;

			pixelDist *= 45;
			pixelDist *= correction;

			float pixelX = pixelDist * dirCos;
			float pixelY = pixelDist * dirSin;

			pixelX += cx;
			pixelY += cy;

			pixelX = int(pixelX);
			pixelY = int(pixelY);

			float texX = pixelX * 0.00078125;
			float texY = pixelY * 0.00078125;

			
			texFloorCeil[floorIndex] = float(1160) + i;
			texFloorCeil[floorIndex + 1] = float(j);

			texFloorCeil[floorIndex + 2] = texX;
			texFloorCeil[floorIndex + 3] = texY;

			texFloorCeil[floorIndex + 4] = float(1160) + i;
			texFloorCeil[floorIndex + 5] = float(j) + 1;

			texFloorCeil[floorIndex + 6] = texX;
			texFloorCeil[floorIndex + 7] = texY;
			

			texFloorCeil[floorIndex + 8] = float(1160) + i;
			texFloorCeil[floorIndex + 9] = 780 - float(j);

			texFloorCeil[floorIndex + 10] = texX + 0.5;
			texFloorCeil[floorIndex + 11] = texY;

			texFloorCeil[floorIndex + 12] = float(1160) + i;
			texFloorCeil[floorIndex + 13] = 780 - float(j) - 1;

			texFloorCeil[floorIndex + 14] = texX + 0.5;
			texFloorCeil[floorIndex + 15] = texY;
			

			floorIndex += 16;
			floorCeilIndex += 16;	
		}
	}
	angle += rot;
	
	//draw rays

	rays.Update(lines, 2080 * 2 * sizeof(float));
	lvao.AddBuffer(rays, llayout);
	lvao.Bind();
	glDrawArrays(GL_LINES, 0, 2080);
	
	//Ceiling and floor
	

	texShader.Bind();
	texShader.SetUniform1i("u_Texture", 0);
	texShader.SetUniformMat4f("u_MVP", mvp2);

	floorceiling.Bind();

	VertexArray fcvao;

	VertexBufferLayout fcl;
	fcl.Push<float>(2);
	fcl.Push<float>(2);

	fcvb.Update(texFloorCeil, 6489600 * sizeof(float));
	fcvao.AddBuffer(fcvb, fcl);
	fcvao.Bind();
	glDrawArrays(GL_LINES, 0, 3244800);
	
	delete[] texFloorCeil;
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

	//test sprite
	/*
	float sprite[] = {
		200.0f, 200.0f,
		300.0f, 300.0f
	};

	GLuint testind[] = {
		0, 1, 2,
		2, 3, 0
	};
	*/
}

void Player::DrawSprites(Object2d &o, Renderer renderer, glm::mat4 proj) {
	
	/*
	float dirCos = cos(angle);
	float dirSin = sin(angle);

	//float dist = sqrt(pow((o.centerX - cx), 2) + pow((o.centerY - cy), 2));
	
	float sx = o.centerX - cx;
	float sy = o.centerY - cy;
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

	//float oangle = (atan2f(cy - o.centerY, cx - o.centerX));


	std::fill_n(o.bufferData, 8320, 0);
	int sIndex = 0;

	float ox = o.centerX - cx;
	float oy = o.centerY - cy;

	float px = sin(angle);
	float py = cos(angle);

	//float oangle = atan2f(py, px) - atan2f(oy, ox) - pi;
	float oangle = atan2f(o.centerY - cy, o.centerX - cx) + (pi/2 -angle);
	float odist = sqrt(pow(o.centerX - cx, 2) + pow(o.centerY - cy, 2));
	//std::cout << odist << " ";

	if (oangle < 0)
		oangle += 2 * pi;
	if (oangle > 2 * pi)
		oangle -= 2 * pi;

	float sx = tan(oangle - pi/2);
	//std::cout << sx << " ";

	sx *= 520;
	//sx += 640;

	float ca = oangle - (pi/2);
	if (ca < 0)
		ca += 2 * pi;
	if (ca > 2 * pi)
		ca -= 2 * pi;

	odist *= cos(ca);

	float sh = (64 / odist) * 520 / 2;
	float sw = (64 / odist) * 520 / 2;
	
	if (sh >= 1040)
		sh = 1040;

	if (sw >= 1040)
		sw = 1040;

	float sy = 390 - sh;
	
	//std::cout << angle * 180 / pi << " " << oangle * 180 / pi << " " << ca * 180 / pi << std::endl;
	//std::cout << sw << std::endl;
	
	
	float screen[] = {
		1160 - sx, sy, 0.0f, 0.0f,
		1160 - sx + 32, sy, 1.0f, 0.0f,
		1160 - sx + 32, sy + sh, 1.0f, 1.0f,
		1160 - sx, sy + sh, 0.0f, 1.0f,
	};
	

	float screen3[8320];

	int start = -sw / 2;
	int end = -(start);

	float texStep = 64/sw/2;
	//std::cout << texStep << std::endl;
	//std::cout << 520 - sx << std::endl;
	float texCoord = 0.015625;
	float step = 0;
	for (int i = start; i < end; ++i) {
		int bufferIndex = 520 - sx + i;

		if (bufferIndex >= 0 && bufferIndex < 1040)
		{
			//std::cout << zBuffer[bufferIndex] << " " << odist << std::endl;
			if (zBuffer[bufferIndex] > odist)
			{
				o.bufferData[sIndex] = 1160 - sx + i;
				o.bufferData[sIndex + 1] = sy;

				o.bufferData[sIndex + 4] = 1160 - sx + i;
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

	GLuint oind[] = {
		0, 1, 2,
		2, 3, 0
	};
	/*
	glm::mat4 mvp2 = proj * line;

	texShader.Bind();
	texShader.SetUniform1i("u_Texture", 0);
	texShader.SetUniformMat4f("u_MVP", mvp2);
	*/

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
	ovao.Bind();

	//std::cout << angle << std::endl;
	//glDrawElements(GL_TRIANGLES, oib.GetCount(), GL_UNSIGNED_INT, nullptr);
	
	glDrawArrays(GL_LINES, 0, 2080);

}

void Player::DrawEnemies(Enemy *enemies, Renderer renderer, glm::mat4 proj) {
	//sort enemies by distance to player

	bool swapped;
	//Enemy enemies[3];

	for (int m = 0; m < 5 - 1; m++) {
		for (int n = 0; n < 5 - m - 1; n++) {
			float d1 = sqrt(pow(enemies[n].centerX - cx, 2) + pow(enemies[n].centerY - cy, 2));
			float d2 = sqrt(pow(enemies[n + 1].centerX - cx, 2) + pow(enemies[n + 1].centerY - cy, 2));

			if (d1 < d2) {
				//swap
				Enemy temp = enemies[n];
				enemies[n] = enemies[n + 1];
				enemies[n + 1] = temp;
			}
		}
	}

	for (int i = 0; i < 5; i++){
		std::fill_n(enemies[i].bufferData, 8320, 0);
		int sIndex = 0;

		float ox = enemies[i].centerX - cx;
		float oy = enemies[i].centerY - cy;

		float px = sin(angle);
		float py = cos(angle);

		float oangle = atan2f(enemies[i].centerY - cy, enemies[i].centerX - cx) + (pi / 2 - angle);
		float oangle2 = atan2f(enemies[i].centerY - cy, enemies[i].centerX - cx);
		float odist = sqrt(pow(enemies[i].centerX - cx, 2) + pow(enemies[i].centerY - cy, 2));


		if (oangle < 0)
			oangle += 2 * pi;
		if (oangle > 2 * pi)
			oangle -= 2 * pi;

		if (oangle2 < 0)
			oangle2 += 2 * pi;
		if (oangle2 > 2 * pi)
			oangle2 -= 2 * pi;

		enemies[i].SetAngle(oangle2);
		enemies[i].SetPlayerPos(cx, cy);

		float sx = tan(oangle - pi / 2);

		sx *= 520;

		float ca = oangle - (pi / 2);
		if (ca < 0)
			ca += 2 * pi;
		if (ca > 2 * pi)
			ca -= 2 * pi;

		odist *= cos(ca);

		float sh = (64 / odist) * 520 / 1.5;
		float sw = (64 / odist) * 520 / 1.5;

		if (sh >= 1040)
			sh = 1040;

		if (sw >= 1040)
			sw = 1040;

		float sy = 390 - sh / 1.5;

		float screen[] = {
			1160 - sx, sy, 0.0f, 0.0f,
			1160 - sx + 32, sy, 1.0f, 0.0f,
			1160 - sx + 32, sy + sh, 1.0f, 1.0f,
			1160 - sx, sy + sh, 0.0f, 1.0f,
		};


		float screen3[8320];

		int start = -sw / 2;
		int end = -(start);

		float texStep = 64 / sw / 2;
		//std::cout << texStep << std::endl;
		//std::cout << 520 - sx << std::endl;
		float texCoord = 0.015625;
		float step = 0;
		unsigned time2 = SDL_GetTicks();
		for (int j = start; j < end; ++j) {
			int bufferIndex = 520 - sx + j;

			if (bufferIndex >= 0 && bufferIndex < 1040)
			{
				//std::cout << zBuffer[bufferIndex] << " " << odist << std::endl;
				if (zBuffer[bufferIndex] > odist)
				{
					enemies[i].bufferData[sIndex] = 1160 - sx + j;
					enemies[i].bufferData[sIndex + 1] = sy;

					enemies[i].bufferData[sIndex + 4] = 1160 - sx + j;
					enemies[i].bufferData[sIndex + 5] = sy + sh;

					if (time2 % 1000 > 0 && time2 % 1000 < 500) {
						enemies[i].bufferData[sIndex + 2] = step * texCoord + 0.5;
						enemies[i].bufferData[sIndex + 3] = 0.0f;

						enemies[i].bufferData[sIndex + 6] = step * texCoord + 0.5;
						enemies[i].bufferData[sIndex + 7] = 1.0f;
					}
					else {
						enemies[i].bufferData[sIndex + 2] = step * texCoord;
						enemies[i].bufferData[sIndex + 3] = 0.0f;

						enemies[i].bufferData[sIndex + 6] = step * texCoord;
						enemies[i].bufferData[sIndex + 7] = 1.0f;

					}

					/*
					if (enemies[i].IsHit()) {
						enemies[i].bufferData[sIndex + 2] = step * texCoord + 0.5;
						enemies[i].bufferData[sIndex + 3] = 0.0f;

						enemies[i].bufferData[sIndex + 6] = step * texCoord + 0.5;
						enemies[i].bufferData[sIndex + 7] = 1.0f;
					}
					else {
						enemies[i].bufferData[sIndex + 2] = step * texCoord;
						enemies[i].bufferData[sIndex + 3] = 0.0f;

						enemies[i].bufferData[sIndex + 6] = step * texCoord;
						enemies[i].bufferData[sIndex + 7] = 1.0f;
					}
					*/

					sIndex += 8;
				}
			}
			step += texStep;
		}

		GLuint oind[] = {
			0, 1, 2,
			2, 3, 0
		};

		Texture sprite("res/Textures/demon.png");
		sprite.Bind();

		VertexBufferLayout slayout;
		slayout.Push<float>(2);
		slayout.Push<float>(2);

		VertexArray ovao;
		VertexBuffer ovb(enemies[i].bufferData, 8320 * sizeof(float));
		ovb.Bind();
		ovao.AddBuffer(ovb, slayout);
		ovao.Bind();
		glDrawArrays(GL_LINES, 0, 2080);
	}
}
void Player::DrawGun() {

	float origin = 1680 - 496;
	
	if (dx > 0 || dx < 0 || dy > 0 || dy < 0) {
		//std::cout << gunStep <<  " " << right << std::endl;
		float x, y;
		if (end == false) {
			if (right == true)
				x = 2;
				//x = 3 * sqrt(abs(gunStep));
				//x = 1 / gunStep * gunStep;
			else
				x = -2;
				//x = 3 * -sqrt(abs(gunStep));
				//x - -1 / gunStep * gunStep;
			y = 1.5 * -gunStep * gunStep;
			 
			gunStep += 0.15;
		}
		else {
			if (right == true)
				x = -2;
				//x = 3 * -sqrt(abs(gunStep));
			else
				x = 2;
				//x = 3 * sqrt(abs(gunStep));
			y = 1.5 * gunStep * gunStep;
			gunStep -= 0.15;
		}

		gunPos[0] += x;
		gunPos[4] += x;
		gunPos[8] += x;
		gunPos[12] += x;

		gunPos[1] += y;
		gunPos[5] += y;
		gunPos[9] += y;
		gunPos[13] += y;

		if (gunStep > 1.5) {
			gunStep = 1.5;
			end = true;
		}
		else if (gunStep < 0) {
			gunStep = 0;
			end = false;
			if (right == true) {
				right = false;
			}
			else if (right == false) {
				right = true;
			}
		}

	}

	if (dx == 0) {

		/*
		if (gunPos[0] != origin) {
			if (gunPos[0] > origin) {
				gunPos[0] -= 3;
				gunPos[4] -= 3;
				gunPos[8] -= 3;
				gunPos[12] -= 3;
			}
			else {
				gunPos[0] += 3;
				gunPos[4] += 3;
				gunPos[8] += 3;
				gunPos[12] += 3;
			}
		}
		if (gunStep > 0)
			gunStep -= 0.25;
			*/
	}

	if (press) {
		unsigned newtime = SDL_GetTicks();
		if (newtime - time > 100) {
			gunPos[2] = 0.0f;
			gunPos[6] = 0.5f;
			gunPos[10] = 0.5f;
			gunPos[14] = 0.0f;
			press = false;
		}
	}

	rifle.Bind();
	gunvb.Update(gunPos, 16 * sizeof(float));
	gunvao.AddBuffer(gunvb, layout);
	ib.Bind();
	gunvb.Bind();
	gunvao.Bind();
	

	/*
	float gPos[] = {
		0.0f, 0.0f, 0.0f, 0.0f,
		100.0f, 0.0f, 1.0f, 0.0f,
		100.0f, 100.0f, 1.0f, 1.0f,
		0.0f, 100.0f, 0.0f, 1.0f,
	};

	GLuint gind[] = {
		0, 1, 2,
		2, 3, 0
	};

	VertexArray gvao;
	VertexBuffer gvb(gunPos, 16 * sizeof(float));

	VertexBufferLayout glayout;
	glayout.Push<float>(2);
	glayout.Push<float>(2);
	gvao.AddBuffer(gvb, glayout);

	IndexBuffer gib(gind, 6);

	
	gvb.Bind();
	gib.Bind();
	
	gvao.Bind();
	*/
	
	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
	
}

bool Player::isShooting() {
	if (press)
		return true;
	else
		return false;
}
