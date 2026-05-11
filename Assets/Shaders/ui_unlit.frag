#version 330 core

out vec4 OutColor;

uniform sampler2D u_texture;

in VS_OUT
{
    vec2 TexCoord;
} frag_in;

void main()
{
    OutColor = texture(u_texture, frag_in.TexCoord);
}