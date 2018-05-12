#version 320 es

precision highp float;
precision highp int;

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
uniform mat4 modelMatrix;
uniform vec3 viewPos;

uniform int typeOfShading;

uniform Material material;

uniform Light light;
#define NUM_LIGHTS 2
uniform Light lights[NUM_LIGHTS];
uniform int multipleLights;
uniform int cartoonShading;

vec3 caculateColorForOneLight(int index)
{
    // vec3 ambient = material.ambient * lights[index].ambient;

    
    vec3 norm = normalize(vNormFr);
    vec3 vertPos = vec3(modelMatrix * vec4(vPosFr, 1.0));
    vec3 lightDir = normalize(lights[index].position - vertPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * material.diffuse) * lights[index].diffuse;

    vec3 viewDir = normalize(viewPos - vertPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (material.specular * spec) * lights[index].specular;

    // attenuation
    float distance = length(lights[index].position - vertPos);
    float attenuation = max(1.0 / (lights[index].constant + lights[index].linear * distance + lights[index].quadratic * (distance * distance)), 1.0);

    // ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 res = (diffuse + specular);

    return res;
}

vec4 cartoon()
{
    float intensity;
	vec4 color;

    vec3 normal = normalize(vNormFr);
    vec3 vertPos = vec3(modelMatrix * vec4(vPosFr, 1.0));
    vec3 lightDir = normalize(lights[0].position - vertPos);

	intensity = dot(lightDir,normal);

	if (intensity > 0.95)
		color = vec4(1.0,0.5,0.5,1.0);
	else if (intensity > 0.5)
		color = vec4(0.6,0.3,0.3,1.0);
	else if (intensity > 0.25)
		color = vec4(0.4,0.2,0.2,1.0);
	else
		color = vec4(0.2,0.1,0.1,1.0);
	return color;
}

void main(void)
{
    if(typeOfShading == 1)
   {
       if(cartoonShading == 1)
       {
           fColor = cartoon();
       }
       else
       {
            if(multipleLights == 1)
            {
                vec3 ambient = material.ambient * lights[0].ambient;
                vec3 temp = vec3(0.0f);
                for(int i = 0; i < NUM_LIGHTS; i++)
                {
                    temp += caculateColorForOneLight(i);
                }
                temp += ambient;
                fColor = vec4(temp,1.0f);
            }
            else 
            {
                vec3 ambient = material.ambient * lights[0].ambient;
                vec3 temp = vec3(0.0f);
                temp = caculateColorForOneLight(0);

                temp += ambient;
                fColor = vec4(temp,1.0f);
            }
       }
   }
   else
   {
        fColor = color;
   }
}
