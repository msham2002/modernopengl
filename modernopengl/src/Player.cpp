#include "Player.h"

Player::Player() /*: vb(positions, 4 * 4 * sizeof(float))*/ {
}


Player::Player(Shader& shader, VertexBufferLayout& layout, VertexBufferLayout& llayout, Map map) :wallvb(texWalls, 16640 * sizeof(float)), fcvb(texFloorCeil, 540 * 390 * 8 * 2 * 2 * sizeof(float)), rays(lines, 2080 * 2 * sizeof(float)), ib(indices, 6), vb(positions, 4 * 4 * sizeof(float)), texture("res/Textures/placeholder2.png"), floorceiling("res/Textures/floorceiling.png"),  lib(rayind, 2), rayShader("res/shaders/Ray.shader"), texShader("res/shaders/Basic.shader")/*, lvb(ray, 2 * 2 * sizeof(float))*/
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
	texFloorCeil = new float[6739200];
}

Player::~Player()
{
	delete[] lines;
	delete[] texWalls;
	delete[] texFloors;
	delete[] texCeil;
	delete[] texFloorCeil;
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
	renderer.Draw(vao, ib, shader);
	
	texLine2(proj);
	//Line(proj);
}

void Player::HandleEvent(SDL_Event& e) {
}

void Player::Move() {
	view = glm::translate(view, glm::vec3(dx, dy, 0));
	cx += dx2;
	cy += dy2;

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

		if (angle + a < 0)
			angle += 2 * pi;
		if (angle + a > 2 * pi)
			angle -= 2 * pi;


		float r2;
		if (angle + a < pi / 2 || angle + a > 3 * pi / 2) {
			r2 = (32 - int(cx) % 32 + int(cx));
		}
		else if (angle + a > pi / 2 || angle + a < 3 * pi / 2) {
			r2 = int(cx) - int(cx) % 32;
		}
		else {
			r2 = 0;
		}

		float r;
		if (angle + a < pi) {
			r = (32 - int(cy) % 32) + int(cy); // nearest 32nd value
		}
		else if (angle + a > pi) {
			r = int(cy) - int(cy) % 32;
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
		while (dof < 20) {
			int mapj = (32 - int(cx + xdist) % 32) + int(cx + xdist);
			int mapi = r;

			mapj /= 32;
			mapi /= 32;

			mapj -= 1;
			if (angle + a < pi)
				mapi += 1;
			mapi = 20 - mapi;

			if (mapj < 20 && mapi < 20 && mapi > -1 && mapj > -1 && map.map[mapi][mapj] == 1) {
				dof = 20;
			}
			else {
				if (angle + a < pi)
					r += 32;
				else
					r -= 32;
				xdist = (r - cy) / tan(angle + a);
				dof += 1;
			}
		}


		//check vertical walls
		dof = 0;

		while (dof < 20) {
			int mapy = (32 - int(cy + ydist) % 32) + int(cy + ydist);
			int mapx = r2;

			mapx /= 32;
			mapy /= 32;

			if (angle + a > pi / 2 && angle + a < 3 * pi / 2)
				mapx -= 1;
			mapy = 20 - mapy;

			if (mapx < 20 && mapy < 20 && mapx > -1 && mapy > -1 && map.map[mapy][mapx] == 1) {
				dof = 20;
			}
			else {
				if (angle + a < pi / 2 || angle + a > 3 * pi / 2)
					r2 += 32;
				else if (angle + a > pi / 2 || angle + a < 3 * pi / 2)
					r2 -= 32;
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

		float lineHeight = (64 / lineDist) * 520;
		//float lineHeight = (60 * 1040) / lineDist;
		if (lineHeight > 5000)
			lineHeight = 5000;

		float lineOffset = 390 - lineHeight / 2;

		if (horLen < vertLen) {
			float texcoord = int(cx + xdist) % 32;
			texcoord += 32;
			texcoord *= (0.015625);
			
			// wall coordinates
			texWalls[horindex] = (1160 - i);
			texWalls[horindex + 1] = lineOffset;

			//texture coordinates
			texWalls[horindex + 2] = texcoord;
			texWalls[horindex + 3] = 0.0f;

			texWalls[horindex + 4] = (1160 - i + 1);
			texWalls[horindex + 5] = lineOffset;

			texWalls[horindex + 6] = texcoord;
			texWalls[horindex + 7] = 0.0f;

			texWalls[horindex + 8] = (1160 - i + 1);
			texWalls[horindex + 9] = (lineHeight + lineOffset);

			texWalls[horindex + 10] = texcoord;
			texWalls[horindex + 11] = 1.0f;

			texWalls[horindex + 12] = (1160 - i);
			texWalls[horindex + 13] = (lineHeight + lineOffset);

			texWalls[horindex + 14] = texcoord;
			texWalls[horindex + 15] = 1.0f;

			
			horindex += 16;
			horsize += 16;
		}
		else {
			float texcoord = int(cy + ydist) % 32;
			texcoord *= (0.015625);

			texWalls[horindex] = (1160 - i);
			texWalls[horindex + 1] = lineOffset;

			texWalls[horindex + 2] = texcoord;
			texWalls[horindex + 3] = 0.0f;

			texWalls[horindex + 4] = (1160 - i + 1);
			texWalls[horindex + 5] = lineOffset;

			texWalls[horindex + 6] = texcoord;
			texWalls[horindex + 7] = 0.0f;

			texWalls[horindex + 8] = (1160 - i + 1);
			texWalls[horindex + 9] = (lineHeight + lineOffset);

			texWalls[horindex + 10] = texcoord;
			texWalls[horindex + 11] = 1.0f;

			texWalls[horindex + 12] = (1160 - i);
			texWalls[horindex + 13] = (lineHeight + lineOffset);

			texWalls[horindex + 14] = texcoord;
			texWalls[horindex + 15] = 1.0f;


			horindex += 16;
			horsize += 16;
		}
		index += 4;
		
		float dirCos = cos(angle + a);
		float dirSin = sin(angle + a);
		for (int j = 0; j < lineOffset; j+=2) {
			float pixelDist = 780 * (1 / (2 * (780 - float(j)) - 780));

			pixelDist *= 45;
			pixelDist *=  1 / cos(a);

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

		if (angle + a < 0)
			angle += 2 * pi;
		if (angle + a > 2 * pi)
			angle -= 2 * pi;

		float r2;
		if (angle + a < pi / 2 || angle + a > 3 * pi / 2) {
			r2 = (32 - int(cx) % 32 + int(cx));
		}
		else if (angle + a > pi / 2 || angle + a < 3 * pi / 2) {
			r2 = int(cx) - int(cx) % 32;
		}
		else {
			r2 = 0;
		}

		float r;
		if (angle + a < pi) {
			r = (32 - int(cy) % 32) + int(cy);
		}
		else if (angle + a > pi) {
			r = int(cy) - int(cy) % 32;
		}
		else {
			r = 0;
		}

		float w = r - cy;
		float xdist = w / tan(angle + a);

		float w2 = r2 - cx;
		float ydist = w2 * tan(angle + a);

		int dof = 0;

		//check horizontal walls
		while (dof < 20) {
			int mapj = (32 - int(cx + xdist) % 32) + int(cx + xdist);
			int mapi = r;

			mapj /= 32;
			mapi /= 32;

			mapj -= 1;
			if (angle + a < pi)
				mapi += 1;
			mapi = 20 - mapi;

			if (mapj < 20 && mapi < 20 && mapi > -1 && mapj > -1 && map.map[mapi][mapj] == 1) {
				dof = 20;
			}
			else {
				if (angle + a < pi)
					r += 32;
				else
					r -= 32;
				xdist = (r - cy) / tan(angle + a);
				dof += 1;
			}
		}


		//check vertical walls
		dof = 0;

		while (dof < 20) {
			int mapy = (32 - int(cy + ydist) % 32) + int(cy + ydist);
			int mapx = r2;

			mapx /= 32;
			mapy /= 32;

			if (angle + a > pi / 2 && angle + a < 3 * pi / 2)
				mapx -= 1;
			mapy = 20 - mapy;

			if (mapx < 20 && mapy < 20 && mapx > -1 && mapy > -1 && map.map[mapy][mapx] == 1) {
				dof = 20;
			}
			else {
				if (angle + a < pi / 2 || angle + a > 3 * pi / 2)
					r2 += 32;
				else if (angle + a > pi / 2 || angle + a < 3 * pi / 2)
					r2 -= 32;
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

		float ca = angle - angle + a;
		if (ca < 0)
			ca += 2 * pi;
		if (ca > 2 * pi)
			ca -= 2 * pi;

		lineDist *= cos(ca);

		float lineHeight = (64 / lineDist) * 520;
		//float lineHeight = (60 * 1040) / lineDist;
		if (lineHeight > 5000)
			lineHeight = 5000;

		float lineOffset = 390 - lineHeight / 2;

		if (horLen < vertLen) {
			float texcoord = int(cx + xdist) % 32;
			texcoord += 32;
			texcoord *= (0.015625);
		
			texWalls[horindex] = (1160 + i);
			texWalls[horindex + 1] = lineOffset;

			texWalls[horindex + 2] = texcoord;
			texWalls[horindex + 3] = 0.0f;

			texWalls[horindex + 4] = (1160 + i + 1);
			texWalls[horindex + 5] = lineOffset;

			texWalls[horindex + 6] = texcoord;
			texWalls[horindex + 7] = 0.0f;

			texWalls[horindex + 8] = (1160 + i + 1);
			texWalls[horindex + 9] = (lineHeight + lineOffset);

			texWalls[horindex + 10] = texcoord;
			texWalls[horindex + 11] = 1.0f;

			texWalls[horindex + 12] = (1160 + i);
			texWalls[horindex + 13] = (lineHeight + lineOffset);

			texWalls[horindex + 14] = texcoord;
			texWalls[horindex + 15] = 1.0f;


			horindex += 16;
			horsize += 16;
		}
		else {
			float texcoord = int(cy + ydist) % 32;
	
			texcoord *= (0.015625);
		
			texWalls[horindex] = (1160 + i);
			texWalls[horindex + 1] = lineOffset;

			texWalls[horindex + 2] = texcoord;
			texWalls[horindex + 3] = 0.0f;

			texWalls[horindex + 4] = (1160 + i + 1);
			texWalls[horindex + 5] = lineOffset;

			texWalls[horindex + 6] = texcoord;
			texWalls[horindex + 7] = 0.0f;

			texWalls[horindex + 8] = (1160 + i + 1);
			texWalls[horindex + 9] = (lineHeight + lineOffset);

			texWalls[horindex + 10] = texcoord;
			texWalls[horindex + 11] = 1.0f;

			texWalls[horindex + 12] = (1160 + i);
			texWalls[horindex + 13] = (lineHeight + lineOffset);

			texWalls[horindex + 14] = texcoord;
			texWalls[horindex + 15] = 1.0f;

			horindex += 16;
			horsize += 16;
		}
		index += 4;

		float dirCos = cos(angle + a);
		float dirSin = sin(angle + a);
		
		for (int j = 0; j < lineOffset; j+=2) {
			float pixelDist = 780 * (1  /  (2 * (780 - float(j)) - 780));

			pixelDist *= 45;
			pixelDist *= 1/ cos(a);

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

	fcvb.Update(texFloorCeil, 540 * 390 * 8 * 2 * 2 * sizeof(float));
	fcvao.AddBuffer(fcvb, fcl);
	fcvao.Bind();
	glDrawArrays(GL_LINES, 0, 1684800 * 2);
	
	//draw walls
	Texture brick("res/Textures/walls2.png");
	brick.Bind();

	VertexArray wallvao;
	wallvb.Update(texWalls, horsize * sizeof(float));

	VertexBufferLayout walllayout;
	walllayout.Push<float>(2);
	walllayout.Push<float>(2);
	wallvao.AddBuffer(wallvb, walllayout);

	GLuint ind[6240];

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

