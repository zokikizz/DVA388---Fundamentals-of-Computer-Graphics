// basic shading

static const char * vs_n2c_src[] = {
#ifdef __linux__
		"#version 130                                                      \n"
#else
		"#version 420 core                                                 \n"
		#endif
		""
		"struct Material { \n"
			"vec3 ambient; \n"
			"vec3 diffuse;\n"
			"vec3 specular;\n"
   			"float shininess;\n"
  		"};                                                                  \n"
		"in vec3 vPos;                                                     \n"
		"in vec3 vNorm;                                                    \n"
		"out vec4 color;                                                   \n"
		"uniform mat4 PV;                                                  \n"


  		"uniform vec4 myColor;	\n"
		"uniform int typeOfShading;	\n"
		"uniform int phase;	\n"
		"uniform vec3 matirialProp;	\n"

		"uniform vec3 lightColor;	\n"
		"uniform vec3 lightPosition;	\n"
		"uniform float ambientStrength;	\n"
  		"uniform mat4 modelMatrix; \n"
		"uniform vec3 viewPos;\n"



		"out vec3 lightColorFr;	\n"
		"out vec3 lightPositionFr;	\n"
		"out float ambientStrengthFr;	\n"
		"out mat4 modelMatrixFr; \n"
		"out vec3 viewPosFr;\n"

		"flat out int typeOfShadingFr;	\n"
		"flat out int phaseFr;	\n"
		"out vec3 matirialPropFr;	\n"
		"out vec3 vNormFr;	\n"
		"out vec3 vPosFr;	\n"

		"uniform Material material; \n"
		"out Material materialFr; \n"
		"                                                                  \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
		"    gl_Position = PV * vec4(vPos, 1.0f);   \n"
		"	if(typeOfShading == 0) \n"
		"   { \n"
			"	if(phase == 1) \n"
			"   { \n"
				"		vec3 ambient = material.ambient * lightColor; \n"

				"		vec3 res = (ambient); \n"
				"	 	color = vec4(res,1.0f); \n"
				"}\n"
				"else if(phase == 3)\n"
				"{ \n"
			"		vec3 ambient = material.ambient * lightColor; \n"

			"		vec3 norm = normalize(vNorm);\n"
			"		vec3 vertPos = vec3(modelMatrix * vec4(vPos, 1.0));\n"
			"		vec3 lightDir = normalize(lightPosition - vertPos);\n"
			"		float diff = max(dot(norm, lightDir), 0.0);\n"
			"		vec3 diffuse = (diff * material.diffuse) * lightColor;		  \n"

			"	    float specularStrength = 2.0f;\n"
			"		vec3 viewDir = normalize(viewPos - vertPos);\n"
			"    	vec3 reflectDir = reflect(-lightDir, norm);  \n"
			"    	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
			"    	vec3 specular = (material.specular * spec) * lightColor;  \n"

			"		vec3 res = (ambient + diffuse + specular); \n"
			"	 	color = vec4(res,1.0f); \n"
			"	}\n"
				"else if(phase == 2) \n"
				"{ \n"
				"		vec3 ambient = material.ambient * lightColor; \n"

				"		vec3 norm = normalize(vNorm);\n"
				"		vec3 vertPos = vec3(modelMatrix * vec4(vPos, 1.0));\n"
				"		vec3 lightDir = normalize(lightPosition - vertPos);\n"
				"		float diff = max(dot(norm, lightDir), 0.0);\n"
				"		vec3 diffuse = (diff * material.diffuse) * lightColor;		  \n"

				"		vec3 res = (ambient + diffuse); \n"
				"	 	color = vec4(res,1.0f); \n"
				"}\n"
			"}\n"
			"else \n"
   		"	{ \n"
	 	" 		color = abs(vec4(vNorm, 1.0));\n          "
   		"		typeOfShadingFr = typeOfShading; \n"
		"		phaseFr = phase; \n"
		"		lightColorFr = lightColor; \n"
		"		lightPositionFr = lightPosition; \n"
		"		matirialPropFr = matirialProp; \n"
		"		ambientStrengthFr = ambientStrength; \n"
		"		modelMatrixFr = modelMatrix; \n"
		"		viewPosFr = viewPos; \n"
		"		vNormFr = normalize(vNorm); \n"
		"		vPosFr = vPos; \n"
  		"		materialFr.ambient = material.ambient;"
		"		materialFr.diffuse = material.diffuse;"
		"		materialFr.specular = material.specular;"
		"		materialFr.shininess = material.shininess;"

		"	}	\n"
		"}                                                                 \n"
};

// Simple fragment shader for color interpolation
static const char * fs_ci_src[] = {
#ifdef __linux__
		"#version 130                                                      \n"
#else
		"#version 420 core                                                 \n"
		#endif
		"struct Material {\n"
        "vec3 ambient;\n"
		"vec3 diffuse;\n"
		"vec3 specular;\n"
		"float shininess;\n"
		"};"

  		"in Material materialFr;"
		"in vec4 color;                                                    \n"
		"out vec4 fColor;                                                  \n"
  		""
  		"in vec3 vNormFr; 												   \n"

		"in vec3 vPosFr; 												   \n"

		"in vec3 lightColorFr;	\n"
		"in vec3 lightPositionFr;	\n"
		"in float ambientStrengthFr;	\n"
		"in mat4 modelMatrixFr; \n"
		"in vec3 viewPosFr;\n"

		"flat in int typeOfShadingFr;	\n"
		"flat in int phaseFr;	\n"
		"in vec3 matirialPropFr;	\n"
		"                                                                  \n"
		"void main(void)                                                   \n"
		"{                                                                 \n"
			  "	if(typeOfShadingFr == 1) \n"
	 		"   { \n"
				"	if(phase == 1) \n"
				"   { \n"
					"		vec3 ambient = materialFr.ambient * lightColor; \n"

					"		vec3 res = (ambient); \n"
					"	 	color = vec4(res,1.0f); \n"
					"}\n"
					"else if(phase == 3)\n"
					"{ \n"
					"		vec3 ambient = materialFr.ambient * lightColor; \n"

					"		vec3 norm = normalize(vNorm);\n"
					"		vec3 vertPos = vec3(modelMatrix * vec4(vPos, 1.0));\n"
					"		vec3 lightDir = normalize(lightPosition - vertPos);\n"
					"		float diff = max(dot(norm, lightDir), 0.0);\n"
					"		vec3 diffuse = (diff * materialFr.diffuse) * lightColor;		  \n"

					"	    float specularStrength = 2.0f;\n"
					"		vec3 viewDir = normalize(viewPos - vertPos);\n"
					"    	vec3 reflectDir = reflect(-lightDir, norm);  \n"
					"    	float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialFr.shininess);\n"
					"    	vec3 specular = (materialFr.specular * spec) * lightColor;  \n"

					"		vec3 res = (ambient + diffuse + specular); \n"
					"	 	color = vec4(res,1.0f); \n"
					"	}\n"
					"else if(phase == 2) \n"
					"{ \n"
					"		vec3 ambient = materialFr.ambient * lightColor; \n"

					"		vec3 norm = normalize(vNorm);\n"
					"		vec3 vertPos = vec3(modelMatrix * vec4(vPos, 1.0));\n"
					"		vec3 lightDir = normalize(lightPosition - vertPos);\n"
					"		float diff = max(dot(norm, lightDir), 0.0);\n"
					"		vec3 diffuse = (diff * materialFr.diffuse) * lightColor;		  \n"

					"		vec3 res = (ambient + diffuse); \n"
					"	 	color = vec4(res,1.0f); \n"
					"}\n"
	  			"}\n"
	  			"else \n"
	  			"{ \n"
					"                                                                 \n"
					"    fColor = color;                                               \n"
				"}                                                                 \n"
		"}                                                                 \n"
};