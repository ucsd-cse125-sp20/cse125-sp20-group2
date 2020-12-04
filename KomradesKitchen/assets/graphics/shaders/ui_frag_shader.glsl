#version 330

in vec3 vs_position;
in vec2 vs_texcoord;

out vec4 fs_color;

uniform sampler2D texture0;

void main( )
{
    fs_color = texture(texture0, vs_texcoord);
}
