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

out mat4 ViewMat;


//Actuellement le shader est fait en local et pas en world,pour passer en world il faut: dans le .vert enlever le *view pour Fragpos et Normal
//Et dans les .frag remplacer normalize(viewPos - FragPos) pour le calcul de viewDir 
//et remettre viewPos dans le calcul de viewDir et remplacer "vec3(ViewMat * vec4(light.position, 1.0))" par light.position

void main()
{
	FragPos = vec3(view * model * vec4(aPos, 1.0));	//On multiplie par la model pour transformer en coodinées world

	Normal = mat3(transpose(inverse(view *model))) * aNormal; 

	TexCoords = aTexCoords;	//Passe les coordonées des textures au fragement shader

	gl_Position = projection * view * model * vec4(aPos, 1.0);

	ViewMat = view;

}