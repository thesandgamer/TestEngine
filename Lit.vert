#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

//On passe les matrices pour la projection(faire de la 3D)
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec3 FragPos; 
out vec3 Normal;
out vec2 TexCoords;

//out vec3 LightPos;

//uniform vec3 lightPos;


void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));	//On multiplie par la model pour transformer en coodinées world

	Normal = mat3(transpose(inverse(model))) * aNormal; 

	//LightPos = vec3(view * vec4(lightPos, 1.0));

	TexCoords = aTexCoords;	//Passe les coordonées des textures au fragement shader

	gl_Position = projection * view * vec4(FragPos, 1.0);

}