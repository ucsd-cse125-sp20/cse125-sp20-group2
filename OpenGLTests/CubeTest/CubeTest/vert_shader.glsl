#version 330 core

// "in" denotes an input variable
layout(location = 0) in vec3 aPos;

void main()
{
	// gl_Position is the predefined output variable
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}