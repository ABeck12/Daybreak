#shader VERTEX
#version 460 core

layout (location = 0) in vec3 a_WorldPosition;
layout (location = 1) in vec3 a_LocalPosition;
layout (location = 2) in vec3 a_Color;
layout (location = 3) in float a_Radius;
layout (location = 4) in float a_Intensity;

// in vec3 a_WorldPosition;
// in vec3 a_LocalPosition;
// in vec3 a_Color;
// in float a_InnerRadius;
// in float a_OuterRadius;
// in float a_Intensity;

uniform mat4 u_ViewProjection;

out vec3 v_LocalPosition;
out vec3 v_Color;
out float v_Radius;
out float v_Intensity;

void main()
{
    gl_Position = u_ViewProjection * vec4(a_WorldPosition,1);

    v_LocalPosition = a_LocalPosition;
    v_Color = a_Color;
    v_Radius = a_Radius;
    v_Intensity = a_Intensity;
}

#shader FRAGMENT
#version 460 core

#ifdef GL_ES
precision mediump float;
#endif

in vec3 v_LocalPosition;
in vec3 v_Color;
in float v_Radius;
in float v_Intensity;

uniform vec2 u_Resolution;

layout(location = 0) out vec4 o_Color;

void main()
{
    float distance = length(v_LocalPosition);
    float circle = smoothstep(1, 0, distance);
    if (distance > 1)
        discard;

    o_Color = vec4(v_Color * circle, 1);
}






