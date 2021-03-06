#include "IndexBuffer.h"
IndexBuffer::IndexBuffer() {

}

IndexBuffer::IndexBuffer(const GLuint* data, GLuint count) : m_Count(count){
	glGenBuffers(1, &m_RendererID); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_DYNAMIC_DRAW);
}

IndexBuffer::~IndexBuffer(){
	glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::UnBind() const{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
