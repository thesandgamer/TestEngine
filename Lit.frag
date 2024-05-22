#version 330 core
out vec4 FragColor;

//For material properties
struct Material {
    sampler2D diffuseTexture;	//Texture pour la couleur de base
	vec3 color;
    sampler2D specularTexture;
    float shininess;
}; 

//For Light properties
struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
//in vec3 LightPos;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform vec3 viewPos;       //Position de la camera
uniform Material material;  //Données du material du shaders
uniform Light light;        //Données de la lumière


void main()
{
	//Calculate ambiant color
	vec3 ambient = light.ambient * texture(material.diffuseTexture, TexCoords).rgb ;// * material.color ;

	//Calculate diffuse
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos); 
	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuseTexture, TexCoords).rgb;// * material.color ;  


	//Calculate specular
	vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specularTexture, TexCoords).rgb;   //Faire en sorte que la specular ait la couleur de la lumière

    
	vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);



}