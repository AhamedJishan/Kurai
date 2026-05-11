#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

// to be set by Renderer
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out VS_OUT
{
    vec3 VertPosModelSpace;
} vs_out;

void main()
{
    // strip out the translation from view
    mat4 viewMatNoTranslation = mat4(mat3(u_View));
    gl_Position = u_Projection * viewMatNoTranslation * u_Model * vec4(a_Position, 1.0);

    vs_out.VertPosModelSpace = a_Position;
}