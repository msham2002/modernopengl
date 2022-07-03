#include<iostream>
#include<SDL.h>
#include<string>
#include <fstream>
#include<sstream>

#define GLEW_STATIC
#include<GL/glew.h>
#include<SDL_opengl.h>
#include <cassert>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Map.h"
//#include "Object2d.h"	
#include "Player.h"

SDL_Window* window = NULL;
SDL_GLContext context;

const GLint WIDTH = 1280, HEIGHT = 640;

void close() {
	SDL_GL_DeleteContext(context);
	window = NULL;
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int mapX = 10, mapY = 10, mapS = 100;

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit()) {
		std::cout << "You fucked up" << std::endl;
		return EXIT_FAILURE;
	}

	glViewport(0, 0, WIDTH, HEIGHT);


	bool quit = false;
	SDL_Event e;
	Map map;
	
	float positions[] = { // 656
		0.0f, 576.0f, 0.0f, 0.0f, //last 2 are texture coordinates
		64.0f, 576.0f, 1.0f, 0.0f,
		64.0f, 640.0f, 1.0f, 1.0f,
		00.0f, 640.0f, 0.0f, 1.0f
	};
	
	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	
	//blending for transparent pixels
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	VertexArray vao;
	VertexBuffer vb(positions, 4 * 4 * sizeof(float));
	
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2); // for texture coordinates
	vao.AddBuffer(vb, layout);

	//ray layout
	VertexBufferLayout llayout;
	llayout.Push<float>(2);
	
	IndexBuffer ib(indices, 6);
	
	glm::mat4 proj = glm::ortho(0.0f, 1280.0f, 0.0f, 640.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	glm::mat4 mvp = proj * view; 
	
	Shader shader("res/shaders/Basic.shader");
	shader.Bind();
	shader.SetUniformMat4f("u_MVP", mvp);

	vao.UnBind();
	vb.UnBind();
	ib.UnBind();
	shader.UnBind();
	
	Player p(shader, layout, llayout, map);
	p.AddBuffer();
	p.Bind();

	std::cout << glGetString(GL_VERSION) << std::endl;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	Renderer renderer;

	//game loop
	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			//events
		}
		//update logic

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.Bind();
		map.DrawMap(vao, ib, shader, proj, renderer);
		const Uint8* keystate = SDL_GetKeyboardState(NULL);
		p.Keyboard(keystate);
		p.Move();
		p.Bind();
		p.Draw(renderer, proj);
		SDL_GL_SwapWindow(window);
	}
	close();
	return 0;
} 
