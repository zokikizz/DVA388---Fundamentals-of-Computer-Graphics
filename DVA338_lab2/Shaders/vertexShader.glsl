#version 320 es 


precision highp float;
precision highp int;

precision highp float;
precision highp int;
precision lowp sampler2D;
precision lowp samplerCube;

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

in vec3 vPos;
in vec3 vNorm;
out vec4 color;
uniform mat4 PV;

uniform vec4 myColor;	
uniform int typeOfShading;

uniform mat4 modelMatrix;
uniform mat4 viewModelMatrix;
uniform mat4 VM;
//uniform mat4 V;
uniform vec3 viewPos;

out mat4 modelMatrixFr; 
out vec3 viewPosFr;

out vec3 vNormFr;
out vec3 vPosFr;

uniform Material material;

uniform Light light;
#define NUM_LIGHTS 2
uniform Light lights[NUM_LIGHTS];
uniform int multipleLights;

vec3 caculateColorForOneLight(int index, vec3 normal)
{
    vec3 ambient = material.ambient * lights[index].ambient;
    
    vec3 norm = normalize(normal);
    vec3 vertPos = vec3(modelMatrix * vec4(vPos, 1.0));
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

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 res = (ambient + diffuse + specular);

    return res;
}


void main(void)                                                   
{                                                                 
    gl_Position = PV * vec4(vPos, 1.0f);

    // vec3 norm = mat3(transpose(inverse(PV))) *vNorm;
    // vec3 norm = vec3(modelMatrix * vec4(vNorm, 1.0f));
    vec3 norm = mat3(transpose(inverse(VM))) * vNorm;

    if(typeOfShading == 0) 
    {

        if(multipleLights == 1) 
        {
            // vec3 ambient = material.ambient * lights[0].ambient;

            vec3 temp = vec3(0.0f);
            for(int i = 0; i < NUM_LIGHTS; i++)
            {
                temp += caculateColorForOneLight(i, norm);
            }
            // temp += ambient;
            color = vec4(temp,1.0f);
        }
        else
        {
            // vec3 ambient = material.ambient * lights[0].ambient;
            vec3 temp = vec3(0.0f);
            temp = caculateColorForOneLight(0, norm);

            // temp += ambient;
            color = vec4(temp,1.0f);
        }
        
    }
    else
    {
        color = abs(vec4(norm, 1.0));
        vNormFr = norm;
        vPosFr = vPos;
    }
}