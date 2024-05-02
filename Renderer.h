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
	void init();
	void update(float dt);
	void draw();

	void set_render_mode(RenderMode n_mode) { render_mode_ = n_mode; }

private:
	unsigned int VBO;	//Créer un ID pour le vertex buffer
	unsigned int VAO;
	unsigned int EBO;

	unsigned int shader_program_;

	Shader shader_{"Lit.vert", "Lit.frag"};

	RenderMode render_mode_ = Flat;


};

