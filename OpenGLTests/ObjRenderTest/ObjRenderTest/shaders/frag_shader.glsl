#version 330 core
out vec4 FragColor;

// Texture/normal data coming from the vertex shader.
in vec2 TexCoord;
in vec3 Norm;

// Fragment light position
in vec3 FragPos;  

// Uniform variable texture data, supplied by the main program.
uniform sampler2D texture1;
uniform sampler2D texture2;

// Ambient light color
uniform vec3 lightColor;
uniform float ambientStrength;

// Used to calculate specular lighting - camera position
uniform vec3 viewPos;
uniform float specularStrength;

// Default object color if no textures.
uniform vec3 defaultObjColor;
uniform float noColorPrecision;

// Light direction
uniform vec3 lightPos;

void main()
{
	// Ambient lighting
	vec3 ambient = ambientStrength * lightColor;

	// get the light direction and normalize (into unit vectors) the surface normals and light direction
	vec3 norm = normalize(Norm);
	vec3 lightDir = normalize(lightPos - FragPos);
	
	// Calculate diffuse lighting
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// Calculate specular lighting based on reflection off of the surface of the object
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;  

	// Calculate object color from textures (replace with something else if needed...)
	vec4 objectColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
	
	// If no textures, use default color (white)
	if (objectColor.x < noColorPrecision && objectColor.y < noColorPrecision && objectColor.z < noColorPrecision) {
		objectColor = vec4(defaultObjColor.x, defaultObjColor.y, defaultObjColor.z, 0);
	}

	// Apply lighting options to original color
	vec3 result = (ambient + diffuse + specular) * vec3(objectColor.x, objectColor.y, objectColor.z);
	FragColor = vec4(result, 1.0);
}