#include "Enemy.h"

Enemy::Enemy() /*: vb(positions, 4 * 4 * sizeof(float))*/ {
	//create positions array
}


Enemy::Enemy(Shader& shader, VertexBufferLayout& layout, float centerX, float centerY, Map map) : ib(indices, 6), vb(positions, 8 * sizeof(float)), texture("res/Textures/placeholder.png")
{
	this->shader = shader;
	this->layout = layout;
	this->map = map;
	this->centerX = centerX;
	this->centerY = centerY;
	playerX = 0;
	playerY = 0;
	angle = 0;
	bufferData = new float[8320];
}

Enemy::~Enemy() {
	//fucks shit up for some reason
	//delete[] bufferData;
}

void Enemy::AddBuffer() {
	vao.AddBuffer(vb, layout);
}

void Enemy::Bind() {
	texture.Bind();
}

void Enemy::Draw(Renderer renderer, glm::mat4& proj) {
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	glm::mat4 mvp = proj * view;
	shader.Bind();
	shader.SetUniform4f("u_Color", 1.0f, 0.0f, 1.0f, 1.0f);
	shader.SetUniformMat4f("u_MVP", mvp);
	//renderer.Draw(vao, ib, shader);

	ib.Bind();
	vao.Bind();

	//glDrawArrays(GL_TRIANGLES, 0, 3); TRIANGLE
	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Enemy::Move() {

	float distance = sqrt(pow(centerX - playerX, 2) + pow(centerY - playerY, 2));

	if (distance < 200) {
		int iInd = trunc(20 - centerY / 32);
		int jInd = trunc(centerX / 32);

		int spx = int(centerX);
		int spy = int(centerY);
		int spx_add = ((int)centerX + 32) / 32;
		int spx_sub = ((int)centerX - 32) / 32;
		int spy_add = 20 - ((int)centerY + 32) / 32;
		int spy_sub = 20 - ((int)centerY - 32) / 32;

		if (centerX > playerX && map.map[iInd][spx_sub] == 0)
			centerX -= 0.6;
		if (centerX < playerX && map.map[iInd][spx_add] == 0)
			centerX += 0.6;
		if (centerY > playerY && map.map[spy_sub][jInd] == 0)
			centerY -= 0.6;
		if (centerY < playerY && map.map[spy_add][jInd] == 0)
			centerY += 0.6;
	}
	
}

void Enemy::SetAngle(float angle){
	this->angle = angle;
}

void Enemy::SetPlayerPos(float playerX, float playerY) {
	this->playerX = playerX;
	this->playerY = playerY;
}

void Enemy::CheckForHit()
{
	for (int i = 0; i < 8320; i += 8) {
		if (trunc(bufferData[i]) == 1160) {
			hit = true;
			break;
		}
		hit = false;
	}
}

bool Enemy::IsHit() {
	return hit;
}

bool Enemy::operator!=(const Enemy& enemy){
	if (centerX == enemy.centerX)
		return false;
	return true;
}
