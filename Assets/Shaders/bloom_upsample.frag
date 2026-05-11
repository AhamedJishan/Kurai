#version 330 core

out vec3 OutColor;

uniform sampler2D u_SrcTexture;
uniform vec2 u_SrcResolution;
uniform int u_BloomRadius;

in VS_OUT
{
    vec2 TexCoord;
} frag_in;

void main() 
{
    vec2 texelSize = 1.0 / u_SrcResolution;
    float x = texelSize.x * u_BloomRadius;
    float y = texelSize.y * u_BloomRadius;
    
    // 3x3 tent filter
    // a b c
    // d e f
    // g h i

    vec3 a = texture( u_SrcTexture, frag_in.TexCoord + vec2(-1.0 * x, -1.0 * y)).rgb;
    vec3 b = texture( u_SrcTexture, frag_in.TexCoord + vec2( 0.0 * x, -1.0 * y)).rgb;
    vec3 c = texture( u_SrcTexture, frag_in.TexCoord + vec2( 1.0 * x, -1.0 * y)).rgb;
    
    vec3 d = texture( u_SrcTexture, frag_in.TexCoord + vec2(-1.0 * x,  0.0 * y)).rgb;
    vec3 e = texture( u_SrcTexture, frag_in.TexCoord + vec2( 0.0 * x,  0.0 * y)).rgb;
    vec3 f = texture( u_SrcTexture, frag_in.TexCoord + vec2( 1.0 * x,  0.0 * y)).rgb;
    
    vec3 g = texture( u_SrcTexture, frag_in.TexCoord + vec2(-1.0 * x,  1.0 * y)).rgb;
    vec3 h = texture( u_SrcTexture, frag_in.TexCoord + vec2( 0.0 * x,  1.0 * y)).rgb;
    vec3 i = texture( u_SrcTexture, frag_in.TexCoord + vec2( 1.0 * x,  1.0 * y)).rgb;

    OutColor = (e)             * 4.0 / 16.0
             + (b + d + f + h) * 2.0 / 16.0
             + (a + c + g + i) * 1.0 / 16.0;
}