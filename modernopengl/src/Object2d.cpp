#include "Object2d.h"

Object2d::Object2d() /*: vb(positions, 4 * 4 * sizeof(float))*/ {
	//create positions array
}


Object2d::Object2d(Shader& shader, VertexBufferLayout& layout) : ib(indices, 6), vb(positions, 4 * 4 * sizeof(float)), texture("res/Textures/placeholder.png")
{	
	this->shader = shader;
	this->layout = layout;
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
	shader.SetUniformMat4f("u_MVP", mvp);
	renderer.Draw(vao, ib, shader);
}
