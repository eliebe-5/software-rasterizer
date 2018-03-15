#version 430

layout(location=0) in vec3 colorGammaCorrected;
layout(location=1) in vec2 uv;

out vec3 Color;

uniform sampler2D texSampler;

void main()
{
    Color = texture(texSampler,uv).rgb * colorGammaCorrected;
}