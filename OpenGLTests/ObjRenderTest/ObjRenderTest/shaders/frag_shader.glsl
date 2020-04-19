#version 330 core
out vec4 FragColor;

// Texture data coming from the vertex shader.
in vec2 TexCoord;

// Uniform variable texture data, supplied by the main program.
uniform sampler2D texture1;
uniform sampler2D texture2;

// Ignored, for the time being.
uniform vec4 inColor;

void main()
{
	//FragColor = texture(texture1, TexCoord);
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}