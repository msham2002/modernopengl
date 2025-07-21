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
#include "Object2d.h"	
#include "Player.h"

SDL_Window* window = NULL;
SDL_GLContext context;

const GLint WIDTH = 1680, HEIGHT = 780;

void close() {
	SDL_GL_DeleteContext(context);
	window = NULL;
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	// sdl and opengl initialization
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	window = SDL_CreateWindow("C++ Raycaster", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	context = SDL_GL_CreateContext(window);

	if (!context) {
		std::cerr << "Error: Failed to create OpenGL context! | " << SDL_GetError() << std::endl;
		close();
		return EXIT_FAILURE;
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Error: GLEW failed to initialize!" << std::endl;
		close();
		return EXIT_FAILURE;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	glViewport(0, 0, WIDTH, HEIGHT);
	//blending for transparent pixels
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	Renderer renderer;
	Shader texShader("res/shaders/Basic.shader");
	Shader rayShader("res/shaders/Ray.shader");
	Map map;
	Player p(rayShader, texShader, map);

	//2d map setup
	float positions[] = { // 656
		0.0f, 608.0f, 0.0f, 0.0f, //last 2 columns are texture coordinates
		32.0f, 608.0f, 1.0f, 0.0f,
		32.0f, 640.0f, 1.0f, 1.0f,
		0.0f, 640.0f, 0.0f, 1.0f
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	VertexArray mapVao;
	VertexBuffer mapVb(positions, 4 * 4 * sizeof(float));

	VertexBufferLayout mapLayout;
	mapLayout.Push<float>(2);
	mapLayout.Push<float>(2); // for texture coordinates
	mapVao.AddBuffer(mapVb, mapLayout);
	IndexBuffer ib(indices, 6);

	// ray layout
	VertexBufferLayout llayout;
	llayout.Push<float>(2);

	// projection matrix
	glm::mat4 proj = glm::ortho(0.0f, 1680.0f, 0.0f, 780.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	glm::mat4 mvp = proj * view;

	//billboard sprite stuff that needs to be fixed
	//Shader raySahder("res/shaders/Ray.shader");
	//Object2d o(rayShader, llayout);
	//o.AddBuffer();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//game loop
	bool quit = false;
	SDL_Event e;

	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
			//events
			p.HandleEvent(e);

		}
		//update logic
		const Uint8* keystate = SDL_GetKeyboardState(NULL);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//texShader.Bind();
		map.DrawMap(mapVao, ib, texShader, proj, renderer);
		p.Keyboard(keystate);
		p.Move();
		p.Draw(proj); // draw the 2.5D raycasted view
		//p.DrawSprites(o, renderer, proj);

		SDL_GL_SwapWindow(window);
	}
	close();
	return 0;
}