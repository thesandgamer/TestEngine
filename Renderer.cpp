#include "Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Renderer::init()
{
	shader_ = std::make_unique<Shader>("Lit.vert", "Lit.frag");
	//Points
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


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
	shader_->setInt("texture", 0);
	shader_->setInt("texture2", 1);


	//-----------------------------Transforms------------------
//-----------Model matrix/transform
	//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

//-----------View
	//Bouger la caméra c'est comme bouger le monde entier, du coup il faut bouger avec des valeurs inverses le monde
	// note that we're translating the scene in the reverse direction of where we want to move
	//Où on est, où on regarde et le up du monde: créer une matrice pour changer la vue(c'est à dire le monde)
	view = glm::lookAt(cameraPos,cameraTarget,up);	//Donne les informations à la matrice de vue de où on veut que la camera regarde 
	//Bind dans le shader
	shader_->setMat4("view", view);	


//-----------Projection
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	//Bind dans le shader
	shader_->setMat4("projection", projection);


}

void Renderer::update(float dt)
{
	deltaTime = dt;

}

void Renderer::draw()
{

	// bind Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);



	//Use shader
	shader_->use();

	//Reset la matrice du model car on va en changer les valeurs
	//model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

//-------------Transform-------
	//		L'idée est de modifier la matrice du model(local transfrom) sur laquel on va appliquer des transformations(rotation, translation, scale) et qu'on va ensuite injecter au shader pour faire bouger nos vertexes
	//Les opérations de matrices se font dans l'ordre inverse, on met en dernier ce qui se fait en premier(faire la scale en premier sinon ça va scale les autre modifications)

	//Pour la rotation tourner autour d'un axe normalisé
	/*
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));*/
	//On passe les matrices de transformation  au shader
	shader_->setMat4("model", model);

	//Pour faire tourner la camera autour de la scene
	/*
	const float radius = 5.0f;
	float camX = sin(glfwGetTime()) * radius;
	float camZ = cos(glfwGetTime()) * radius;
	view = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	shader_->setMat4("view", view);//Bind la matrice dans le shader*/
	//Pour contrôler la caméra qui se déplace


	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	shader_->setMat4("view", view);//Bind la matrice dans le shader
	


	glBindVertexArray(VAO);

	//L'idée c'est qu'on a un array qui va stoquer des positions auquelles sont censé être nos cubes(tous les mêmes)
	//Pour chaque cube on va set le shader avec les informations de transform du cube puis on va dessiner
	//Ca va donc dessiner le cube en fonction de ce qu'on à mis dans le shader
	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f*i, 0.0f));
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5)); 
		shader_->setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}


}

void Renderer::processInputs(GLFWwindow* window)
{
	float cameraSpeed = 2.5f * deltaTime; 

//-------Camera control
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraRight;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraRight;
}

void Renderer::processMouse(float mouseX, float mouseY)
{
	yaw += mouseX;
	pitch += mouseY;

	//Camera constraints avec haut et bas
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	//On va calculer le nouveau forward de la camera en fonction du pich et du yaw
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cameraFront = glm::normalize(direction);

	//Et on va aussi calculer le forward de la camera(faire une fonction qui get le right en retrournant la fonction sera plus simple)
	cameraRight = glm::normalize(glm::cross(up, cameraFront));

	//Pareil pour le camera Up
	cameraUp = glm::cross(cameraFront, cameraRight);
}


void Renderer::end()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

