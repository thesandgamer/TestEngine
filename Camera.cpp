#include "Camera.h"

#include <glm/ext/matrix_transform.hpp>

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp);
}

void Camera::updateCameraVectors()
{
	//On va calculer le nouveau forward de la camera en fonction du pich et du yaw
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cameraFront = glm::normalize(direction);

	//Et on va aussi calculer le forward de la camera(faire une fonction qui get le right en retrournant la fonction sera plus simple)
	cameraRight = glm::normalize(glm::cross(cameraFront,worldUp ));

	//Pareil pour le camera Up
	cameraUp = glm::normalize(glm::cross(cameraRight,cameraFront));
}
