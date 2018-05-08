#version 130

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;


   float constant;
   float linear;
   float quadratic;
};

in vec4 color;
out vec4 fColor;

in vec3 vNormFr;
in vec3 vPosFr;
in mat4 modelMatrixFr;
in vec3 viewPosFr;

flat in int typeOfShadingFr;

uniform Material material;

uniform Light light;
#define NUM_LIGHTS 4
uniform Light lights[NUM_LIGHTS];

void main(void)
{

    if(typeOfShadingFr == 1)
   {
          vec3 ambient = material.ambient * light.ambient;

          vec3 norm = normalize(vNormFr);
          vec3 vertPos = vec3(modelMatrixFr * vec4(vPosFr, 1.0));
          vec3 lightDir = normalize(light.position - vertPos);
          float diff = max(dot(norm, lightDir), 0.0);
          vec3 diffuse = (diff * material.diffuse) * light.diffuse;

          vec3 viewDir = normalize(viewPosFr - vertPos);
          vec3 reflectDir = reflect(-lightDir, norm);
          float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
          vec3 specular = (material.specular * spec) * light.specular;

          // attenuation
          float distance = length(light.position - vertPos);
          float attenuation = max(1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)), 1.0);

          ambient *= attenuation;
          diffuse *= attenuation;
          specular *= attenuation;

          vec3 res = (ambient + diffuse + specular);
          fColor = vec4(res,1.0f);
   }
   else
   {
        fColor = color;
   }
}
