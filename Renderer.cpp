#include "Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "stb_image.h"

#include "glm/glm.hpp"

void Renderer::init()
{
	shader_ = std::make_unique<Shader>("Lit.vert", "Lit.frag");
	//Points
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
	   0, 1, 3, // first triangle
	   1, 2, 3  // second triangle
	};


	//----------------------------------------------------------------------------------
	glGenVertexArrays(1, &VAO);	//Génère le vertex array object
	glGenBuffers(1, &VBO);	//Génère vertex buffer object
	glGenBuffers(1, &EBO);	//Génère l'Element buffer object

	glBindVertexArray(VAO);	//On bind le VAO	
	// 2. copy our vertices array in a vertex buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Pareil pour l'EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Set la façon dont opengl va lire les données stoquées

	//Ici pour les données stoque dans le premier index(les positions)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Et ici pour les couleurs
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Et ici pour la texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//-----------------------------Texture------------------
//Génère la texture
	glGenTextures(1, &texture);//Combien de texture on veut créer et où on les stoques
	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, texture);//On bind la texture

//Set paramètres
	//Set les options pour les coordonées de texture quand sort de la range
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//en U
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//en V
	//On va set la méthode d'interpolation des pixels(quand upscale ou downscale): nearest pixelise, linear blur
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//Création des options pour les mipmaps 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	/* //Si on veut utiliser Clamp to border
	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	*/

	//Load image grâce à stb image
	int width, height, nrChannels;
	unsigned char* data = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		//On génère la texture à partir de l'image loadé
		//			Target de la texture/niveau de mipmap/Format de l'image/wid...   / doit être 0/format    / data type           / data de l'image
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);	//Génère les mip maps pour la texture
	}
	else
	{
		std::cout << "ERROR::TEXTURE of type: " << "Failed to load texture" << " Error can come from an incorrect path"<< "\n -- --------------------------------------------------- -- " << std::endl;
	}
	//On libère l'image de la mémoire
	stbi_image_free(data);

//------------------texture 2
	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load("resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	//On libère l'image de la mémoire
	stbi_image_free(data);

	// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
	// -------------------------------------------------------------------------------------------
	shader_->use(); // don't forget to activate/use the shader before setting uniforms!
	// either set it manually like so:
	glUniform1i(glGetUniformLocation(shader_->ID, "texture1"), 0);
	// or set it via the texture class
	shader_->setInt("texture2", 1);

}

void Renderer::update(float dt)
{


}

void Renderer::draw()
{

	// bind Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// render container
	shader_->use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

void Renderer::end()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
