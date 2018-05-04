
	#version 420 core

	in vec3 vPos;
	in vec3 vNorm;
	out vec4 color;
	uniform mat4 PV;
 	out vec3 Normal;
  	 out vec3 FragPos;
   	uniform mat4 modelMatrix;


	void main(void)
	{
	    color = abs(vec4(vNorm, 1.0));
	    gl_Position = PV * vec4(vPos, 1.0f);
 	 	FragPos = vec3(modelMatrix * vec4(vPos, 1.0f));
 		"Normal = vNorm; \n"
	}




	// Simple vertex shader treating vertex normals as RGB colors
    //static const char * vs_n2c_src[] = {
    //#ifdef __linux__
    //	"#version 130                                                      \n"
    //#else
    //	"#version 420 core                                                 \n"
    //#endif
    //	"                                                                  \n"
    //	"in vec3 vPos;                                                     \n"
    //	"in vec3 vNorm;                                                    \n"
    //	"out vec4 color;                                                   \n"
    //	"uniform mat4 PV;                                                  \n"
    // 	"out vec3 Normal; \n"
    //  	" out vec3 FragPos; \n"
    //   	"uniform mat4 modelMatrix; \n"
    //
    //	"                                                                  \n"
    //	"void main(void)                                                   \n"
    //	"{                                                                 \n"
    //	"    color = abs(vec4(vNorm, 1.0));                                \n"
    //	"    gl_Position = PV * vec4(vPos, 1.0f);                          \n"
    // 	" 	FragPos = vec3(modelMatrix * vec4(vPos, 1.0f));\n"
    // 		"Normal = vNorm; \n"
    //	"}                                                                 \n"
    //};
    //
    //// Simple fragment shader for color interpolation
    //static const char * fs_ci_src[] = {
    //#ifdef __linux__
    //	"#version 130                                                      \n"
    //#else
    //	"#version 420 core                                                 \n"
    //#endif
    //	"                                                                  \n"
    //	"uniform vec4 light;                                                    \n"
    //	"in vec4 color;                                                    \n"
    //	"uniform vec4 Objcolor;                                                    \n"
    // 	"out vec4 fColor;                                                  \n"
    //	"                                                                  \n"
    //	"in vec3 Normal; \n"
    //	"in vec3 FragPos; \n"
    //	"uniform vec3 LightPos; \n"
    //
    //  	"uniform vec3 viewPos; \n"
    //
    // 	"void main(void)                                                   \n"
    //	"{                                                                 \n"
    //
    //	"	float ambientStrength = 0.1;										\n"
    //	" 	vec4 ambient = ambientStrength * light;					\n"
    //
    //		"vec3 norm = normalize(Normal);\n"
    //		"vec3 lightDir = normalize(LightPos - FragPos);  \n"
    //		"float diff = max(dot(norm, lightDir), 0.0);\n"
    //		"vec4 diffuse = diff * light; \n"
    //
    //
    //  		"float specularStrength = 0.5; \n"
    //		"vec3 viewDir = normalize(viewPos - FragPos); \n"
    //		"vec3 reflectDir = reflect(-lightDir, norm); \n"
    //		"float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);  \n"
    //		"vec4 specular = specularStrength * spec * light; \n"
    //
    //	" 	fColor = (ambient+ diffuse + specular) * color;						\n"
    //	"}                                                                 \n"
    //};

    // The source code for additional shaders can be added here
    // ...

