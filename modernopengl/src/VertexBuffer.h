#pragma once
#include <GL/glew.h>

class VertexBuffer {
private:
	GLuint m_RendererID;

public:
	VertexBuffer();
	VertexBuffer(const void* data, GLuint size);
	~VertexBuffer();

	void Bind() const;
	void UnBind() const;
};
