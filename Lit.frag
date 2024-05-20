#version 330 core
out vec4 FragColor;


in vec3 FragPos;  
in vec3 Normal;  
in vec3 LightPos;

in vec2 TexCoord;

//	Base color
uniform vec3 objectColor;

//	Light
uniform vec3 lightColor;

// Texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;



void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);


	//Calculate ambiant color
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

	//Calculate diffuse
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos); 

	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;


	//Calculate specular
	float specularStrength = 0.5;
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 

    
	vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);



}