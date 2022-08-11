#include "Map.h"
#include "Texture.h"

void Map::DrawMap(const VertexArray& vao, const IndexBuffer& ib, Shader& shader, glm::mat4 proj, Renderer renderer)
{
	Texture wall("res/Textures/tile64.png");
	Texture blank("res/Textures/tile64black.png");
	for (int i = 0; i < mapY; ++i) {
		for (int j = 0; j < mapX; ++j) {
			if (map[i][j] != 0) {
				wall.Bind(0);
				shader.SetUniform1i("u_Texture", 0);
				model = glm::translate(glm::mat4(1.0f), glm::vec3(j * 32, -i * 32, 0));
				glm::mat4 mvp = proj * model;
				shader.SetUniformMat4f("u_MVP", mvp);
				renderer.Draw(vao, ib, shader);
			}
			else {
				blank.Bind(1);
				shader.SetUniform1i("u_Texture", 1);
				model = glm::translate(glm::mat4(1.0f), glm::vec3(j * 32, -i * 32, 0));
				glm::mat4 mvp = proj * model;
				shader.SetUniformMat4f("u_MVP", mvp);
				renderer.Draw(vao, ib, shader);

			}
		}
	}
}
