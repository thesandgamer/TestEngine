#pragma once
#include <memory>

class Renderer
{
public:
	void init();
	void update(float dt);
	void draw();

private:
	unsigned int VBO;	//Créer un ID pour le vertex buffer
	unsigned int VAO;
	unsigned int shaderProgram;


};

