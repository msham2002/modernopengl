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
#include "Enemy.h"
#include "Player.h"

SDL_Window* window = NULL;
SDL_GLContext context;

const GLint WIDTH = 1680, HEIGHT = 780;

//1680 x 780
//1040 x 780

void close() {
	SDL_GL_DeleteContext(context);
	window = NULL;
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int mapX = 20, mapY = 20, mapS = 400;

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
		0.0f, 608.0f, 0.0f, 0.0f, //last 2 are texture coordinates
		32.0f, 608.0f, 1.0f, 0.0f,
		32.0f, 640.0f, 1.0f, 1.0f,
		0.0f, 640.0f, 0.0f, 1.0f
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
	
	glm::mat4 proj = glm::ortho(0.0f, 1680.0f, 0.0f, 780.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	glm::mat4 mvp = proj * view; 
	
	Shader shader("res/shaders/Basic.shader");
	shader.Bind();
	shader.SetUniformMat4f("u_MVP", mvp);

	Shader test("res/shaders/Ray.shader");
	test.Bind();
	test.SetUniformMat4f("u_MVP", mvp);

	vao.UnBind();
	vb.UnBind();
	ib.UnBind();
	shader.UnBind();
	
  	Player p(shader, layout, llayout, map);
	p.AddBuffer();
	p.Bind();

	//Object2d o(test, llayout);
	//o.AddBuffer();
	
	//Enemy enemy(test, llayout, 384, 320);
	//enemy.AddBuffer();

	Enemy e1(test, llayout, 320, 160, map);
	Enemy e2(test, llayout, 420, 220, map);
	Enemy e3(test, llayout, 576, 450, map);
	Enemy e4(test, llayout, 330, 550, map);
	Enemy e5(test, llayout, 105, 370, map);
	Enemy del(test, llayout, -100, -100, map);
	Enemy enemies[5];

	enemies[0] = e1;
	enemies[1] = e2;
	enemies[2] = e3;
	enemies[3] = e4;
	enemies[4] = e5;

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
			p.HandleEvent(e);
			
		}
		//update logic


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.Bind();
		//map.DrawMap(vao, ib, shader, proj, renderer);
		const Uint8* keystate = SDL_GetKeyboardState(NULL);


		if (p.isShooting()) {
			for (int i = 0; i < 5; ++i)
			{
				enemies[i].CheckForHit();
				if (enemies[i].IsHit() == true) {
					enemies[i] = del;
				}
			}
		}

		for (int i = 0; i < 5; ++i) {
			if(enemies[i] != del)
				enemies[i].Move();
		}

		p.Keyboard(keystate);
		p.Move();
		p.Bind();
		p.Draw(renderer, proj);
		//p.DrawSprites(o, renderer, proj);
		p.DrawEnemies(enemies, renderer, proj);
		p.DrawGun();

		SDL_GL_SwapWindow(window);
	}
	close();
	return 0;
} 
