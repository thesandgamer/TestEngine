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

    //For attenuation
    float constant;
    float linear;
    float quadratic;
};
//in vec3 LightPos;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

in mat4 ViewMat;

uniform vec3 viewPos;       //Position de la camera
uniform Material material;  //Données du material du shaders
uniform Light light;        //Données de la lumière


//Actuellement le shader est fait en local et pas en world,pour passer en world il faut: dans le .vert enlever le *view pour Fragpos et Normal
//Et dans les .frag remplacer normalize(viewPos - FragPos) pour le calcul de viewDir 
//et remettre viewPos dans le calcul de viewDir et remplacer "vec3(ViewMat * vec4(light.position, 1.0))" par light.position

void main()
{


//Calculate ambiant color
	vec3 ambient = light.ambient * texture(material.diffuseTexture, TexCoords).rgb ;// * material.color ;

//Calculate diffuse
	vec3 norm = normalize(Normal);
    //vec3 lightDir = normalize(light.position - FragPos);
    vec3 lightDir = normalize(vec3(ViewMat * vec4(light.position, 1.0)) - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuseTexture, TexCoords).rgb;// * material.color ;  


//Calculate specular
	vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specularTexture, TexCoords).rgb * light.diffuse ;   //Faire en sorte que la specular ait la couleur de la lumière

//Calculs pour attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
    light.quadratic * (distance * distance));  
    
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;   
    
//Résulat final
	vec3 result = (ambient + diffuse + specular) * material.color;
    FragColor = vec4(result, 1.0);



}