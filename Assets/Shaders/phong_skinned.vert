#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in vec3 a_Tangent;
layout (location = 4) in ivec4 a_JointIds;
layout (location = 5) in vec4 a_Weights;

// to be set by Renderer
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_MatrixPalette[128];

out VS_OUT
{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec3 Tangent;
} vs_out;

void main()
{
    mat4 skinMatrix = mat4(0.0);
    for (int i = 0; i < 4 ; i++)
    {
        skinMatrix += u_MatrixPalette[a_JointIds[i]] * a_Weights[i];
    }

    vec4 localPos = skinMatrix * vec4(a_Position, 1.0);
	vec4 worldPos = u_Model * localPos;
    gl_Position = u_Projection * u_View * worldPos;

    vs_out.FragPos = worldPos.xyz;
    vs_out.TexCoord = a_TexCoord;

    mat3 skinnedNormalMatrix = mat3(transpose(inverse(u_Model))) * mat3(skinMatrix);
    vs_out.Normal = normalize(skinnedNormalMatrix * a_Normal);
    vs_out.Tangent = normalize(skinnedNormalMatrix * a_Tangent);
}