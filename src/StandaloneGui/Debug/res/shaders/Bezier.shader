#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 col;

uniform mat4 u_MVP;
out vec3 v_color;

void main() {

	gl_Position = u_MVP * vec4(position.x, position.y, position.z, 1.0f);
	v_color = col;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_color;

void main() {
	color = vec4(v_color.x,v_color.y,v_color.z,1.0f);
};