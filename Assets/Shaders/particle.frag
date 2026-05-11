#version 330 core

out vec4 OutColor;

in vec4 FragColor;

void main()
{
    OutColor = FragColor;
}