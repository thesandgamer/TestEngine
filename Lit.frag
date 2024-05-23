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

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  


uniform DirLight dirLight;                          //Les info pour la directional light(equivalent soleil)
uniform PointLight pointLights[NR_POINT_LIGHTS];    //Les informations pour lespoints lights

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

in mat4 ViewMat;

uniform vec3 viewPos;       //Position de la camera
uniform Material material;  //Données du material du shaders


//Fonctions de calcul de lumière
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir); 


//Actuellement le shader est fait en local et pas en world,pour passer en world il faut: dans le .vert enlever le *view pour Fragpos et Normal
//Et dans les .frag remplacer normalize(viewPos - FragPos) pour le calcul de viewDir 
//et remettre viewPos dans le calcul de viewDir et remplacer "vec3(ViewMat * vec4(light.position, 1.0))" par light.position

void main()
{

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(-FragPos);

    //Calculate Dir light
    vec3 result = CalcDirLight(dirLight, norm, FragPos ,viewDir);

    //Calculate all points lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        if (pointLights[i].diffuse != vec3(0.0,0.0,0.0))
        {
            result += CalcPointLight(pointLights[i], norm, FragPos, viewDir); 
        }
    }

    FragColor = vec4(result, 1.0);

}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    //Calculate ambiant
    vec3 ambient  = light.ambient  * texture(material.diffuseTexture, TexCoords).rgb;

    //Calcualte diffuse
    vec3 lightDir = normalize(-light.direction);
    vec3 norm = normalize(normal);
	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuseTexture, TexCoords).rgb;

    // specular shading
    vec3 ViewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(ViewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specularTexture, TexCoords).rgb * light.diffuse ;

    // combine results
    return (ambient + diffuse + specular);

} 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
//Calculate ambiant color
	vec3 ambient = light.ambient * texture(material.diffuseTexture, TexCoords).rgb ;// * material.color ;

//Calculate diffuse
	vec3 norm = normalize(normal);
    vec3 lightDir = normalize(vec3(ViewMat * vec4(light.position, 1.0)) - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuseTexture, TexCoords).rgb;// * material.color ;  


//Calculate specular
	vec3 ViewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(ViewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specularTexture, TexCoords).rgb * light.diffuse ;   //Faire en sorte que la specular ait la couleur de la lumière

//Calculs pour attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));  
    
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;   
    
//Résulat final
	return (ambient + diffuse + specular) * material.color;
   
} 