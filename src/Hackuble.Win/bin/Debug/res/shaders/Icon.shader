#shader vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord0;

uniform mat4 u_MVP;

out vec2 uv0;

void main()
{
    gl_Position = u_MVP * vec4(position.x, position.y, position.z, 1.0f);
    uv0 = texCoord0;
};

#shader fragment
#version 330 core

uniform sampler2D mainTex;

in vec2 uv0;
out vec4 fragColor;

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

float screenPxRange() {
    vec2 unitRange = vec2(4.0f) / vec2(textureSize(mainTex, 0));
    vec2 screenTexSize = vec2(1.0) / fwidth(0.1f);
    return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

void main()
{
    vec3 msd = texture(mainTex, uv0).rgb;
    float sd = median(msd.r, msd.g, msd.b);
    float screenPxDistance = 1.0f * (sd - 0.5);
    float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);

    if (opacity < 0.5f)
        discard;

    fragColor = vec4(1.0f, 1.0f, 1.0f, opacity);
};
