#version 330 core
out vec4 FragColor;

//For material properties
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
uniform Material material;

//For Light properties
struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;  
in vec3 LightPos;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoord;



// Texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;



void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);


	//Calculate ambiant color
	vec3 ambient = light.ambient * material.ambient;

	//Calculate diffuse
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos); 

	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);


	//Calculate specular
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  

    
	vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);



}