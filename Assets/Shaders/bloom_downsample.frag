#version 330 core

out vec3 OutColor;

uniform sampler2D u_SrcTexture;
uniform vec2 u_SrcResolution;

in VS_OUT
{
    vec2 TexCoord;
} frag_in;

void main() 
{
    vec2 texelSize = 1.0 / u_SrcResolution;
    float x = texelSize.x;
    float y = texelSize.y;

    // 13 tap box filter
    // a  .  b  .  c
	// .  j  .  k  .
	// d  .  e  .  f
	// .  l  .  m  .
	// g  +  h  .  i

    vec3 a = texture( u_SrcTexture, frag_in.TexCoord + vec2(-2.0 * x, -2.0 * y)).rgb;
    vec3 b = texture( u_SrcTexture, frag_in.TexCoord + vec2( 0.0 * x, -2.0 * y)).rgb;
    vec3 c = texture( u_SrcTexture, frag_in.TexCoord + vec2( 2.0 * x, -2.0 * y)).rgb;
    
    vec3 d = texture( u_SrcTexture, frag_in.TexCoord + vec2(-2.0 * x,  0.0 * y)).rgb;
    vec3 e = texture( u_SrcTexture, frag_in.TexCoord + vec2( 0.0 * x,  0.0 * y)).rgb;
    vec3 f = texture( u_SrcTexture, frag_in.TexCoord + vec2( 2.0 * x,  0.0 * y)).rgb;
    
    vec3 g = texture( u_SrcTexture, frag_in.TexCoord + vec2(-2.0 * x,  2.0 * y)).rgb;
    vec3 h = texture( u_SrcTexture, frag_in.TexCoord + vec2( 0.0 * x,  2.0 * y)).rgb;
    vec3 i = texture( u_SrcTexture, frag_in.TexCoord + vec2( 2.0 * x,  2.0 * y)).rgb;
    
    vec3 j = texture( u_SrcTexture, frag_in.TexCoord + vec2(-1.0 * x,  -1.0 * y)).rgb;
    vec3 k = texture( u_SrcTexture, frag_in.TexCoord + vec2( 1.0 * x,  -1.0 * y)).rgb;
    vec3 l = texture( u_SrcTexture, frag_in.TexCoord + vec2(-1.0 * x,   1.0 * y)).rgb;
    vec3 m = texture( u_SrcTexture, frag_in.TexCoord + vec2( 1.0 * x,   1.0 * y)).rgb;

    OutColor = (j + k + l + m)  * 0.5  / 4.0
             + (a + b + d + e) * 0.125 / 4.0
             + (b + c + e + f) * 0.125 / 4.0
             + (d + e + g + h) * 0.125 / 4.0
             + (e + f + h + i) * 0.125 / 4.0;

    OutColor = clamp(OutColor, vec3(0), vec3(1000.0));
}