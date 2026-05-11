#version 330 core

out vec4 OutColor;

uniform sampler2D u_HDRTexture;
uniform sampler2D u_BloomTexture;
uniform float u_BloomStrength;

in VS_OUT
{
    vec2 TexCoord;
} frag_in;

vec3 TonemapACES(vec3 x) {
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

vec3 LinearToSRGB(vec3 c) {
    return pow(c, vec3(1.0 / 2.2));
}

void main() 
{
    vec3 hdrColor = texture(u_HDRTexture, frag_in.TexCoord).rgb;
    vec3 bloomColor = texture(u_BloomTexture, frag_in.TexCoord).rgb;

    vec3 combined = hdrColor + bloomColor * u_BloomStrength;

    // Tonemap
    vec3 mapped = TonemapACES(combined * 1.0);

    // Gamma correct
    mapped = LinearToSRGB(mapped);

    OutColor = vec4(mapped, 1.0);
}