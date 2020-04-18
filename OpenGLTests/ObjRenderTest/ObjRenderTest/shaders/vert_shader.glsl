#version 330 core

// These inputs are received from the vertex buffers we pass in.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

// Texture coordinates output
out vec2 TexCoord;

// Transformation matrices (uniforms that we have to pass in manually)
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // note that we read the multiplication from right to left
    gl_Position = projection * view * model * vec4(aPos, 1.0);

	// Texture colors + coordinates
	TexCoord = aTexCoord;
}