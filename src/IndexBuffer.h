#pragma once
#include <GL/glew.h>

class IndexBuffer {
private:
	GLuint m_RendererID;
	GLuint m_Count;
public:
	IndexBuffer();
	IndexBuffer(const GLuint* data, GLuint count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;

	inline GLuint GetCount() const {
		return m_Count;
	}
};
