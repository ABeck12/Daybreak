#shader VERTEX
#version 460 core

layout (location = 0) in vec3 a_WorldPosition;
layout (location = 1) in vec3 a_LocalPosition;
layout (location = 2) in vec4 a_Color;
layout (location = 3) in float a_Fade;
layout (location = 4) in float a_Thickness;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec3 v_LocalPosition;
out float v_Fade;
out float v_Thickness;

void main()
{
	gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0f);
	v_Color = a_Color;
    v_LocalPosition = a_LocalPosition;
    v_Fade = a_Fade;
    v_Thickness = a_Thickness;
};

#shader FRAGMENT
#version 460 core

layout(location = 0) out vec4 o_Color;

in vec3 v_LocalPosition;
in vec4 v_Color;
in float v_Fade;
in float v_Thickness;

void main()
{
    float distance = 1.0 - length(v_LocalPosition);

    float circle = smoothstep(0, v_Fade, distance);
    circle *= smoothstep(v_Thickness + v_Fade, v_Thickness, distance);

	if (circle == 0.0)
		discard;

    o_Color = v_Color;
    o_Color.a *= circle;

    // o_Color = vec4(v_Color.xyz * circle, v_Color.a);
};