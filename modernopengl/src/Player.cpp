#include "Player.h"

Player::Player() /*: vb(positions, 4 * 4 * sizeof(float))*/ {
}


Player::Player(Shader& shader, VertexBufferLayout& layout, VertexBufferLayout& llayout, Map map) : ib(indices, 6), vb(positions, 4 * 4 * sizeof(float)), texture("res/Textures/placeholder2.png"), lib(rayind, 2), rayShader("res/shaders/Ray.shader")/*, lvb(ray, 2 * 2 * sizeof(float))*/
{
	this->shader = shader;
	this->layout = layout;
	this->llayout = llayout;
	dx = 0.0f, dy = 0.0f;
	angle = 90.0f * pi / 180, rot = 0.075f;
	this->map = map;
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
	Line(proj);
}

void Player::HandleEvent(SDL_Event& e) {
}

void Player::Move() {
	//float deg = (angle * pi) / 180;

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

		float lineHeight = (60 * 1040) / lineDist;
		if (lineHeight > 1040)
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
