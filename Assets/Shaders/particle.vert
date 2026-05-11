#version 330 core

layout (location = 0) in vec3 a_VertexPos;  // mesh vertex, per vertex
layout (location = 1) in float a_T;         // per instance
layout (location = 2) in vec3 a_Position;   // per instance

uniform mat4 u_View;
uniform mat4 u_Projection;

// should remain in sync with ParticleSystem.h
#define MAX_CURVE_KEYS 5

uniform int u_NumScaleKeys;
uniform float u_ScaleKeyTimes[MAX_CURVE_KEYS];
uniform vec3 u_ScaleKeyValues[MAX_CURVE_KEYS];
uniform int u_NumColorKeys;
uniform float u_ColorKeyTimes[MAX_CURVE_KEYS];
uniform vec4 u_ColorKeyValues[MAX_CURVE_KEYS];

// Returns vec3(lowIndex, highIndex, localT)
vec3 SampleCurve(float t, float keyTimes[MAX_CURVE_KEYS], int numKeys)
{
    if (t <= keyTimes[0])           return vec3(0, 0, 0);
    if (t >= keyTimes[numKeys - 1]) return vec3(numKeys - 1, numKeys - 1, 0);

    for (int i = 0; i < numKeys - 1; i++)
    {
        if (t >= keyTimes[i] && t <= keyTimes[i + 1])
        {
            float localT = (t - keyTimes[i]) / (keyTimes[i + 1] - keyTimes[i]);
            return vec3(i, i + 1, localT);
        }
    }

    return vec3(numKeys - 1, numKeys - 1, 0);
}

out vec4 FragColor;

void main()
{
    vec3 scaleSample = SampleCurve(a_T, u_ScaleKeyTimes, u_NumScaleKeys);
    vec3 scale = mix(u_ScaleKeyValues[int(scaleSample.x)], u_ScaleKeyValues[int(scaleSample.y)], scaleSample.z);

    vec3 colorSample = SampleCurve(a_T, u_ColorKeyTimes, u_NumColorKeys);
    vec4 color = mix(u_ColorKeyValues[int(colorSample.x)], u_ColorKeyValues[int(colorSample.y)], colorSample.z);
    FragColor = color;

    vec3 transformedPos = a_VertexPos * scale + a_Position;
    gl_Position = u_Projection * u_View * vec4(transformedPos, 1.0);
}