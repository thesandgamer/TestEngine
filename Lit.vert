#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec2 aTexCoord;

//On passe les matrices pour la projection(faire de la 3D)
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//out vec2 TexCoord;

out vec3 FragPos; 
out vec3 Normal;


void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));	//On multiplie par la model pour transformer en coodinées world
	Normal = mat3(transpose(inverse(model))) * aNormal;  
	gl_Position = projection * view * vec4(FragPos, 1.0);

	//TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}