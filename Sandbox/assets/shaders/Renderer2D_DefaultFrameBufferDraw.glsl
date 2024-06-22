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
	vec4 textureColor = texture(u_Textures[0],v_TexCoord);
	vec4 textureDepthScencil = texture(u_Textures[1],v_TexCoord);

    o_Color = vec4(textureColor);


    // o_Color2 = vec4(vec3(1.0 - texture(u_Textures[0],v_TexCoord)), 1.0); // inverted colors
	// o_Color2 = vec4(textureColor.z, textureColor.x, textureColor.y, 1.0); // Debug texture
    // o_Color = vec4(textureColor) * o_Color;
};