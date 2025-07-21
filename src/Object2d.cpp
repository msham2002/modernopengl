#include "Object2d.h"

Object2d::Object2d() /*: vb(positions, 4 * 4 * sizeof(float))*/ {
	//create positions array
}


Object2d::Object2d(Shader& shader, VertexBufferLayout& layout) : ib(indices, 6), vb(positions, 8 * sizeof(float)), texture("res/Textures/placeholder.png")
{	
	this->shader = shader;
	this->layout = layout;
	bufferData = new float[8320];
}

Object2d::~Object2d() {
	delete[] bufferData;
}

void Object2d::AddBuffer(){
	vao.AddBuffer(vb, layout);
}

void Object2d::Bind() {
	texture.Bind();
}

void Object2d::Draw(Renderer renderer, glm::mat4& proj){
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

void Object2d::CheckForHit()
{
	for (int i = 0; i < 8320; i += 8) {
		if (trunc(bufferData[i]) == 1160) {
			hit = true;
			break;
		}
		hit = false;
	}
}

bool Object2d::IsHit() {
	return hit;
}