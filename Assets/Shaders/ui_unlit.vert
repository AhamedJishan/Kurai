#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

out VS_OUT
{
    vec2 TexCoord;
} vs_out;

void main()
{
    gl_Position = u_ViewProjection * u_Model * vec4(aPos, 0, 1);

    vs_out.TexCoord = aTexCoord;
}