#include "Player.h"

Player::Player() /*: vb(positions, 4 * 4 * sizeof(float))*/ {
}


Player::Player(Shader& shader, VertexBufferLayout& layout, VertexBufferLayout& llayout, Map map) : ib(indices, 6), vb(positions, 4 * 4 * sizeof(float)), texture("res/Textures/placeholder2.png"), lib(rayind, 2)
{
	this->shader = shader;
	this->layout = layout;
	this->llayout = llayout;
	dx = 0.0f, dy = 0.0f;
	angle = 90.0f * pi / 180;
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

void Player::HandleEvent(SDL_Event& e){
}

void Player::Move(){
	view = glm::translate(view, glm::vec3(dx, dy, 0));

	if (angle < 0)
		angle += 2 * pi;
	if (angle > 2 * pi)
		angle -= 2 * pi;
}

void Player::Keyboard(const Uint8* keystate){
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
	}
	if (down) {
		dx = 0;
		dy = -3.0;
	}
	if (left) {
		dx = -3.0;
		dy = 0;
	}
	if (right) {
		dx = 3.0;
		dy = 0;
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

	//no keys
	if (!up && !down && !left && !right) {
		dy = 0;
		dx = 0;
	}

	//look
	if (lookL) {
		view = glm::translate(view, glm::vec3(centerX, centerY, 0));
		view = glm::rotate(view, 0.075f, glm::vec3(0, 0, 1));
		view = glm::translate(view, glm::vec3(-centerX, -centerY, 0));
	}
	if (lookR) {
		view = glm::translate(view, glm::vec3(centerX, centerY, 0));
		view = glm::rotate(view, -0.075f, glm::vec3(0, 0, 1));
		view = glm::translate(view, glm::vec3(-centerX, -centerY, 0));
	}
}

void Player::Line(glm::mat4& proj) {
	
	glm::mat4 mvp = proj * view;
	Shader rayshader("res/shaders/Ray.shader");
	rayshader.Bind();
	rayshader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
	rayshader.SetUniformMat4f("u_MVP", mvp);
	
	float x = 0.0f;
	float y = 100.0f;
	angle = float(1 * pi / 180);

	for (int i = 0; i < 80; ++i) {
		float ray[4] = {
			centerX, centerY,
			x + centerX, y + centerY
		};

		if (i == 40){
			angle = -angle;
			x = 0.0f;
			y = 100.0f;
		}

		float fx = cos(angle) * x - sin(angle) * y;
		float fy = sin(angle) * x + cos(angle) * y;
		x = fx;
		y = fy;
		VertexBuffer lvb(ray, 2 * 2 * sizeof(float));
		lvao.AddBuffer(lvb, llayout);
		lvao.Bind();
		lib.Bind();

		glDrawArrays(GL_LINES, 0, 2);
	}
}
