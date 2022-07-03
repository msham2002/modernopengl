#include "Map.h"
#include "Texture.h"

void Map::DrawMap(const VertexArray& vao, const IndexBuffer& ib, Shader& shader, glm::mat4 proj, Renderer renderer)
{
	for (int i = 0; i < mapY; ++i) {
		for (int j = 0; j < mapX; ++j) {
			if (map[i][j] == 1) {
				Texture texture("res/Textures/tile64.png");
				texture.Bind();
				shader.SetUniform1i("u_Texture", 0);
				model = glm::translate(glm::mat4(1.0f), glm::vec3(j * 64, -i * 64, 0));
				glm::mat4 mvp = proj * model;
				shader.SetUniformMat4f("u_MVP", mvp);
				renderer.Draw(vao, ib, shader);
			}
			else {
				Texture texture("res/Textures/tile64black.png");
				texture.Bind();
				shader.SetUniform1i("u_Texture", 0);
				model = glm::translate(glm::mat4(1.0f), glm::vec3(j * 64, -i * 64, 0));
				glm::mat4 mvp = proj * model;
				shader.SetUniformMat4f("u_MVP", mvp);
				renderer.Draw(vao, ib, shader);

			}
		}
	}
}
