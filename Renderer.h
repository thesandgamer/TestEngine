#pragma once
#include <memory>

#include "Shader.h"

enum RenderMode
{
	Flat,
	Wireframe,
};

class Renderer
{
public:
	Renderer() = default;

	void init();
	void update(float dt);
	void draw();
	void end();

	void set_render_mode(RenderMode n_mode) { render_mode_ = n_mode; }

private:
	unsigned int VBO;	//Créer un ID pour le vertex buffer
	unsigned int VAO;
	unsigned int EBO;

	unsigned int shader_program_;

	unsigned int texture;
	unsigned int texture2;

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection;

	std::unique_ptr<Shader> shader_;

	RenderMode render_mode_ = Flat;


};

