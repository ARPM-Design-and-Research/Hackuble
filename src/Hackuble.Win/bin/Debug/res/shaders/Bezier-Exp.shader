#shader vertex
#version 430 core

layout(std430, binding = 0) buffer TVertex
{
    vec4 vertex[];
};

uniform mat4  u_mvp;
uniform vec2  u_resolution;
uniform float u_thickness;

uniform vec3 u_color;

out vec4 col;

void main()
{
    int line_i = gl_VertexID / 6;
    int tri_i = gl_VertexID % 6;

    vec4 va[4];
    for (int i = 0; i < 4; ++i)
    {
        va[i] = u_mvp * vertex[line_i + i];
        va[i].xyz /= va[i].w;
        va[i].xy = (va[i].xy + 1.0) * 0.5 * u_resolution;
    }

    vec2 v_line = normalize(va[2].xy - va[1].xy);
    vec2 nv_line = vec2(-v_line.y, v_line.x);

    vec4 pos;
    if (tri_i == 0 || tri_i == 1 || tri_i == 3)
    {
        vec2 v_pred = normalize(va[1].xy - va[0].xy);
        vec2 v_miter = normalize(nv_line + vec2(-v_pred.y, v_pred.x));

        pos = va[1];
       // float d = dot(v_miter, nv_line);

        //if (d < radians(90.0f)) {
        pos.xy += v_miter * u_thickness * (tri_i == 1 ? -0.5 : 0.5);///d;
        //}
    }
    else
    {
        vec2 v_succ = normalize(va[3].xy - va[2].xy);
        vec2 v_miter = normalize(nv_line + vec2(-v_succ.y, v_succ.x));

        pos = va[2];
       // float d = dot(v_miter, nv_line);

        //if (d > radians(90.0f)) {
        pos.xy += v_miter * u_thickness * (tri_i == 5 ? 0.5 : -0.5);///d;
        //}
    }

    pos.xy = pos.xy / u_resolution * 2.0 - 1.0;
    pos.xyz *= pos.w;
    gl_Position = pos;

    //float op = tri_i % 2;
    col = vec4(u_color.x,u_color.y,u_color.z, 1.0f);
}

#shader fragment
#version 430 core

out vec4 fragColor;
in vec4 col;

void main()
{
    if (col.w <= 0.0001f)
        discard;

    fragColor = vec4(col.x,col.y,col.z,1.0f);
}
