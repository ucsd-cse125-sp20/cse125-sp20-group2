#version 330 core

// These inputs are received from the vertex buffers we pass in.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

// Texture/lighting outputs
out vec2 TexCoord;
out vec3 Norm;
out vec3 FragPos;

// Transformation matrices (uniforms that we have to pass in manually)
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMatrix;

void main()
{
    // note that we read the multiplication from right to left
    gl_Position = projection * view * model * vec4(aPos, 1.0);

	// Texture coordinates
	TexCoord = aTexCoord;

	// Normalize surface normals using inverse transpose of translation-less model matrix
	Norm = mat3(normalMatrix) * aNorm;  

	// Fragment position relative to object's model coords
	FragPos = vec3(model * vec4(aPos, 1.0));
}