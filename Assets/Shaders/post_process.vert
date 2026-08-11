#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

uniform vec2 u_SrcSize;
uniform vec2 u_DstSize;

out VS_OUT
{
    vec2 TexCoord;
} vs_out;

void main()
{
    float srcAspectRatio = u_SrcSize.x / u_SrcSize.y;
    float dstAspectRatio = u_DstSize.x / u_DstSize.y;

    vec3 ndcPosition = a_Position;

    if (srcAspectRatio > dstAspectRatio)
        ndcPosition.y *= dstAspectRatio / srcAspectRatio;
    else if (dstAspectRatio > srcAspectRatio)
        ndcPosition.x *= srcAspectRatio / dstAspectRatio;

    gl_Position =  vec4(ndcPosition, 1.0);

    vs_out.TexCoord = a_TexCoord;
}