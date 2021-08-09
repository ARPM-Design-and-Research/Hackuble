#shader vertex
#version 330 core

layout(location = 0) in vec4 position1;
layout(location = 1) in vec4 position2;
layout(location = 2) in vec4 position3;
layout(location = 3) in vec4 position4;
layout(location = 4) in vec4 corner;
layout(location = 5) in vec4 edge;

layout(location = 6) in vec4 instance1;
layout(location = 7) in vec4 instance2;
layout(location = 8) in vec4 instance3;

uniform mat4 u_MVP;
uniform mat4 u_MV;

out vec4 edge_position;
out float edge_width;
out vec3 v_color;

void main() {

	//Need to think about model view matrix?
	vec2 p = vec2(instance1.xy);
	vec2 s = vec2(instance1.zw);
	vec4 cR = instance2;
	float mn = min(s.x, s.y) / 2.0f;
	float zDepth = instance3.w;

	v_color = instance3.rgb;

	//Calculates vertex position to draw triangles
	vec2 vertex;
	vertex.x = cR.x * position1.x + cR.y * position1.y + cR.z * position1.z + cR.w * position1.w + s.x * position2.x + s.y * position2.y + mn * position2.z;
	vertex.y = cR.x * position3.x + cR.y * position3.y + cR.z * position3.z + cR.w * position3.w + s.x * position4.x + s.y * position4.y + mn * position4.z;
	gl_Position = u_MVP *vec4(vertex.x+p.x,vertex.y+p.y,zDepth,1.0f);

	//Calculates center of edge for circle or boundary of rectangle to pass to fragment shader
	vec2 e;
	e.x = corner.x * cR.x + corner.y * (s.x-cR.y) + corner.z * (s.x-cR.z) + corner.w * cR.w + edge.x*vertex.x + edge.y*vertex.x + edge.z*mn + edge.w*(s.x - mn);
	e.y = corner.x * cR.x + corner.y * cR.y + corner.z *( s.y-cR.z) + corner.w * (s.y-cR.w) + edge.x*mn + edge.y*(s.y-mn) + edge.z*vertex.y + edge.w*vertex.y;
	edge_position = u_MV*vec4(e.x+p.x,e.y+p.y,0.0f,1.0f);

	//Passes radius of corner or edge width threshold to fragment
	edge_width = u_MV[0][0]*(corner.x * cR.x + corner.y * cR.y + corner.z * cR.z + corner.w * cR.w + edge.x*mn + edge.y*(s.y-mn) + edge.z*mn + edge.w*(s.x-mn));
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
layout(origin_upper_left) in vec4 gl_FragCoord;

in vec4 edge_position;
in float edge_width;
in vec3 v_color;

void main() {

	vec2 pixel_pos = gl_FragCoord.xy;

	float distance_to_edge = length(pixel_pos - edge_position.xy) - edge_width;

	//For Aliasing
	float pixel_opacity = clamp(0.5 - distance_to_edge, 0, 1);

	if (pixel_opacity < 0.5f)
		discard;

	color = vec4(v_color.x, v_color.y, v_color.z, 1.0f);
	//color = vec4(1.0f,0.0f,0.0f,1.0f);
};