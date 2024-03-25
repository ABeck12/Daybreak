#shader VERTEX
#version 460 core

layout (location = 0) in vec2 a_Position;

uniform mat4 u_ViewProjection;
uniform vec2 u_LightPosition;

out vec4 v_ClipSpacePos;

void main()
{
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0, 1.0f);
    v_ClipSpacePos = vec4(u_LightPosition,0,0);
}

#shader FRAGMENT
#version 460 core

#ifdef GL_ES
precision mediump float;
#endif

in vec4 v_ClipSpacePos;

layout(location = 0) out vec4 o_Color;
uniform mat4 u_ViewProjection;
uniform vec2 u_Resolution;
uniform vec3 u_LightColor;
uniform vec2 u_LightPosition;
uniform float u_InnerRadius;
uniform float u_OuterRadius;
uniform float u_Intensity;

void main()
{
    // vec4 unitVector = u_ViewProjection * vec4(1);
    // float conversionFactor = unitVector.z / (2*unitVector.w);

    vec2 st = gl_FragCoord.xy / u_Resolution * 2.0 - 1.0;
    float aspectRatio = u_Resolution.x / u_Resolution.y;

    // Temp
    vec4 u_LightColor = vec4(1);
    st.x *= aspectRatio;

    // vec2 screenSpaceCoords = (v_ClipSpacePos.xy / v_ClipSpacePos.w) * 0.5 + 0.5;
    // vec2 screenSpaceCoords = gl_FragCoord.xy *2;

    // float circle = distance(st, vec2(screenSpaceCoords.x * aspectRatio, screenSpaceCoords.y));
    // float circle = length(v_ClipSpacePos.xyz);
    float circle = length(vec3(1,1.5,0));
    // float u_OuterRadius = 1;
    // if (circle > u_OuterRadius*conversionFactor)
    // {
    //     circle = 1;
    // }
    // else
    //     discard;
    // circle = smoothstep(0.8,0.203,circle);

    gl_FragColor = vec4(vec3(1) * circle,1);
}