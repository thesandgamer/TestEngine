#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 position);

	glm::mat4 GetViewMatrix();



private:

	void updateCameraVectors();

	//Up vector du world space
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);	//ToDo: pour le world up peut être avoir un global variable ou un singleton qui va chercher cette valeur dans ce qui gère le monde

	//Camera
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);		//Où est la camera
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);	//Où pointe la camera

	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);					//Forward vector de la camera
	glm::vec3 cameraRight = glm::normalize(glm::cross(worldUp, cameraFront));		//Right vector de la camera
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);						//Up vector pour la camera

	//Pour gérer le yaw et le pitch de la camera
	float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float pitch = 0.0f;

	float fov = 45.0f;




};

