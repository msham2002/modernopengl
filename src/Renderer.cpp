#include "Renderer.h"

void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ib, const Shader& shader) const {
	shader.Bind();
	ib.Bind();
	vao.Bind();

	//glDrawArrays(GL_TRIANGLES, 0, 3); TRIANGLE
	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}