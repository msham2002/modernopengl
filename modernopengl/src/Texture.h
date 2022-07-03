#pragma once
#include "Renderer.h"

class Texture {
private:
	GLuint m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture();
	Texture(const std::string& path);
	~Texture();

	void Bind(GLuint slot = 0) const;
	void UnBind() const;

	//get width, get height possibly
};