#version 330 core

out vec4 OutColor;

uniform vec3 u_TopColor;
uniform vec3 u_HorizonColor;
uniform float u_TopIntensity;
uniform float u_HorizonIntensity;

in VS_OUT
{
    vec3 VertPosModelSpace;
} frag_in;

vec3 SRGBToLinear(vec3 c) {
    return pow(c, vec3(2.2));
}

void main()
{
    float t = clamp(normalize(frag_in.VertPosModelSpace).y, 0.0, 1.0);
    t = pow(t, 0.6);

    vec3 top = u_TopColor;
    vec3 horizon = u_HorizonColor;

    vec3 color = mix(horizon, top, t);

    OutColor = vec4(color, 1.0);
}