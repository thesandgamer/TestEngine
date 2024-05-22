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
	lightShader_ = std::make_unique<Shader>("Light.vert", "Light.frag");


//////]-----------------------------[Données du mesh]-----------------------------[
	//Coorodonées des points en local de l'objet
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
	/*
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	*/

	//----------------------------------------------------------------------------------
	glGenVertexArrays(1, &VAO);	//Génère le vertex array object 
	glGenBuffers(1, &VBO);		//Génère vertex buffer object //Le VBO va contenir les datas des vertices
	//glGenBuffers(1, &EBO);		//Génère l'Element buffer object


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/*
	//Pareil pour l'EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	*/

	// Set la façon dont opengl va lire les données stoquées
	glBindVertexArray(VAO);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	

//--------------------Textures du mesh
	char const* pathName = "resources/textures/container2.png";
	diffuseMap = loadTexture(pathName);		//Load la texture


//////]-----------------------------[Transforms]-----------------------------[
	https://learnopengl.com/Getting-started/Coordinate-Systems

//-----------Model matrix/transform
	//La matrice de model permet de gérer les points relatif à l'objet dans la scene du monde(donc déplacement, rotation et taille)
	//L'idée est de modifier la matrice du model(local transfrom) sur laquel on va appliquer des transformations(rotation, translation, scale) et qu'on va ensuite injecter au shader pour faire bouger nos vertexes
	//Les opérations de matrices se font dans l'ordre inverse, on met en dernier ce qui se fait en premier(faire la scale en premier sinon ça va scale les autre modifications)
	//Pour la rotation tourner autour d'un axe normalisé

//-----------View
	//La matrice de vue permet de rendre les points par rapport à la camera 
	//Bouger la caméra c'est comme bouger le monde entier, du coup il faut bouger avec des valeurs inverses le monde
	// note that we're translating the scene in the reverse direction of where we want to move
	//Où on est, où on regarde et le up du monde: créer une matrice pour changer la vue(c'est à dire le monde)
	shader_->use();
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);	//Donne les informations à la matrice de vue de où on veut que la camera regarde 
	shader_->setMat4("view", view);	//Bind dans le shader

//-----------Projection
	//La projection sert à gérer si la vue de la camera se fait en Orthographique ou en perspective
	projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
	shader_->setMat4("projection", projection);//Bind dans le shader

	//Ensuite les points sont rendu par rapport à l'écran




//////]-----------------------------[Lighting]-----------------------------[
//--------------------Lighting Source Mesh

	//Créer un VAO pour la lumière
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// set the vertex attribute 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Set shaders pour le cube de lumière
	glm::vec3 lightColor = { .5f, .5f, .5f };
	//Set shader for light source
	lightShader_->use();
	lightShader_->setMat4("projection", projection);
	lightShader_->setMat4("view", view);
	lightShader_->setVec3("lightColor", lightColor);

	//---Set les params des shaders

	
	shader_->use();
	//ToDo: bien gérer l'ambiant et le diffuse de la lumière, car je n'ai pas trop compris
	shader_->setVec3("light.ambient", { lightColor.x/2,lightColor.y/2,lightColor.z/2 });
	shader_->setVec3("light.diffuse", lightColor); // darken diffuse light a bit
	shader_->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

	//
	shader_->setVec3("light.position", lightPos);
	shader_->setVec3("viewPos", cameraPos);

	//To set material for cubes
	glm::vec3 materialColor = { 1.0f, 1.0f, 1.0f };

	shader_->setInt("material.diffuseTexture", 0);
	shader_->setVec3("material.color", materialColor);
	shader_->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	shader_->setFloat("material.shininess", 64.0f);
	



}

void Renderer::update(float dt)
{
	deltaTime = dt;

}

void Renderer::draw()
{

	//glm::vec3 posOfLight = { lightPos.x, lightPos.y + glm::sin((float)glfwGetTime() * .5f) * 2, lightPos.z };
	glm::vec3 posOfLight = lightPos;

	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);	//Gère où la camera regarde(quelle partie du monde elle va voir)

	
	shader_->use(); //Use shader

	//Set la position de la lumière dans le shader
	shader_->setVec3("light.position", posOfLight);
	shader_->setVec3("viewPos", cameraPos);

	//Pour faire tourner la camera autour de la scene
	/*
	const float radius = 5.0f;
	float camX = sin(glfwGetTime()) * radius;
	float camZ = cos(glfwGetTime()) * radius;
	view = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	shader_->setMat4("view", view);//Bind la matrice dans le shader*/

	//Gestion de la matrice pour la camera
	shader_->setMat4("view", view);//Bind la matrice dans le shader


	
	// bind Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);

	glBindVertexArray(VAO); //Bind Vertex

	//L'idée c'est qu'on a un array qui va stoquer des positions auquelles sont censé être nos cubes(tous les mêmes)
	//Pour chaque cube on va set le shader avec les informations de transform du cube puis on va dessiner
	//Ca va donc dessiner le cube en fonction de ce qu'on à mis dans le shader
	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);	//Reset car on change les valeurs
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f*i, 0.0f));
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		shader_->setMat4("model", model);	//On passe la matrice de transformation  au shader

		glDrawArrays(GL_TRIANGLES, 0, 36);	//Dessine les triangles
	}

//-------For Light cube

	lightShader_->use();

	//Déplace la lumière
	model = glm::mat4(1.0f);
	model = glm::translate(model, posOfLight);
	model = glm::scale(model, glm::vec3(.2f));
	lightShader_->setMat4("model", model);
	lightShader_->setMat4("view", view);

	//Dessine le cube de lumière
	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

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

void Renderer::processScroll(float scrollvalue)
{
	fov -= scrollvalue;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;

	projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
	//Bind dans le shader
	shader_->setMat4("projection", projection);
}

unsigned int Renderer::loadTexture(char const* path)
{
	unsigned int textureID;

	//Génère la texture
	glGenTextures(1, &textureID);//Combien de texture on veut créer et où on les stoques

	//Load image grâce à stb image
	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		//glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
		glBindTexture(GL_TEXTURE_2D, textureID);//On bind la texture
		//On génère la texture à partir de l'image loadé
		//Target de la texture			/niveau de mipmap/Format de l'image/wid...   / doit être 0/format    / data type           / data de l'image
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);	//Génère les mip maps pour la texture

		//Set paramètres
		//Set les options pour les coordonées de texture quand sort de la range
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//en U
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//en V
		//On va set la méthode d'interpolation des pixels(quand upscale ou downscale): nearest pixelise, linear blur
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//Création des options pour les mipmaps 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//Si on veut utiliser Clamp to border
		//float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		stbi_image_free(data);//On libère l'image de la mémoire
	}
	else
	{
		std::cout << "ERROR::TEXTURE of type: " << "Failed to load texture" << " Error can come from an incorrect path" << "\n -- --------------------------------------------------- -- " << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}


void Renderer::end()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

}

