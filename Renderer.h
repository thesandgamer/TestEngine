#pragma once
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

	void processInputs(GLFWwindow* window);
	void processMouse(float mouseX, float mouseY);
	void processScroll(float scrollvalue);

	void set_render_mode(RenderMode n_mode) { render_mode_ = n_mode; }

	unsigned int loadTexture(char const* path);

private:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;

	unsigned int lightVAO;

	unsigned int shader_program_;

	unsigned int diffuseMap;
	unsigned int specularMap;


	//Matrices pour
	glm::mat4 model = glm::mat4(1.0f);	//Matrice qui controle le model par rapport au monde
	glm::mat4 view = glm::mat4(1.0f);		//Matrice le rapport camera objets
	glm::mat4 projection;					//Comment sont rendu les objets(orthographique, perspective,...)

	std::unique_ptr<Shader> shader_;
	std::unique_ptr<Shader> lightShader_;

	RenderMode render_mode_ = Flat;


	//Pour gérer plusieurs objets
	glm::vec3 cubePositions[10] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	//Up vector du world space
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);


	//Camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);		//Où est la camera

	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);					//Forward vector de la camera
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraFront));		//Right vector de la camera
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);						//Up vector pour la camera

	//Pour gérer le yaw et le pitch de la camera
	float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float pitch = 0.0f;

	float fov = 45.0f;



	//-------Lights
	glm::vec3 pointLightPositions[2] = {
	   glm::vec3(0.7f,  0.2f,  2.0f),
	   glm::vec3(0.7f, .2f, -7.0f),
	};

	glm::vec3 basePos = pointLightPositions[0];

	glm::vec3 pointLightColors[2] = {
	   glm::vec3( 1.0f, 1.0f, 1.0f ),
	   glm::vec3(1.0f, .5f, .5f),
	};
	//----------
	float deltaTime = 0.0f;

};

