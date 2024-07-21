#shader VERTEX
#version 460 core

layout (location = 0) in vec3 a_ScreenCoord;
layout (location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

void main()
{
	gl_Position = vec4(a_ScreenCoord,1);
	v_TexCoord = a_TexCoord;
};

#shader FRAGMENT
#version 460 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

uniform sampler2D u_Textures[32];

void main()
{
    vec4 draw2DColor = texture(u_Textures[0], v_TexCoord);
    vec4 draw2DDepth = texture(u_Textures[1], v_TexCoord);
    vec4 light2DColor = texture(u_Textures[2], v_TexCoord);

    o_Color = vec4(draw2DColor * light2DColor);
	gl_FragDepth = draw2DDepth.z;
};