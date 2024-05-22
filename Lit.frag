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
struct PointLight {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    //For attenuation
    float constant;
    float linear;
    float quadratic;
};
#define NR_POINT_LIGHTS 4  //Définit combien on peut avoir de point light au max
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;


in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

in mat4 ViewMat;

uniform vec3 viewPos;       //Position de la camera
uniform Material material;  //Données du material du shaders
uniform PointLight pointLight;        //Données de la lumière

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir); 


//Actuellement le shader est fait en local et pas en world,pour passer en world il faut: dans le .vert enlever le *view pour Fragpos et Normal
//Et dans les .frag remplacer normalize(viewPos - FragPos) pour le calcul de viewDir 
//et remettre viewPos dans le calcul de viewDir et remplacer "vec3(ViewMat * vec4(light.position, 1.0))" par light.position

void main()
{


//Calculate ambiant color
	vec3 ambient = pointLight.ambient * texture(material.diffuseTexture, TexCoords).rgb ;// * material.color ;

//Calculate diffuse
	vec3 norm = normalize(Normal);
    //vec3 lightDir = normalize(light.position - FragPos);
    vec3 lightDir = normalize(vec3(ViewMat * vec4(pointLight.position, 1.0)) - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = pointLight.diffuse * diff * texture(material.diffuseTexture, TexCoords).rgb;// * material.color ;  


//Calculate specular
	vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = pointLight.specular * spec * texture(material.specularTexture, TexCoords).rgb * pointLight.diffuse ;   //Faire en sorte que la specular ait la couleur de la lumière

//Calculs pour attenuation
    float distance    = length(pointLight.position - FragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + 
    pointLight.quadratic * (distance * distance));  
    
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;   
    
//Résulat final
	vec3 result = (ambient + diffuse + specular) * material.color;
    FragColor = vec4(result, 1.0);



}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuseTexture, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuseTexture, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specularTexture, TexCoords));
    return (ambient + diffuse + specular);

} 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuseTexture, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuseTexture, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specularTexture, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 