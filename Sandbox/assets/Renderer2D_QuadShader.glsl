#shader VERTEX
#version 460 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec4 a_Color;
layout (location = 3) in float a_TexIndex;
layout (location = 4) in float a_TilingFactor;


uniform mat4 u_ViewProjection;

out vec2 v_TexCoord;
out float v_TexIndex;
out vec4 v_Color;

void main()
{
	gl_Position = u_ViewProjection * vec4(a_Position.x, a_Position.y, a_Position.z, 1.0f);
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_Color = a_Color;
};

#shader FRAGMENT
#version 460 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float v_TexIndex;
in vec4 v_Color;

uniform sampler2D u_Textures[32];

void main()
{
	vec4 textureColor = texture(u_Textures[int(v_TexIndex)],v_TexCoord) * v_Color;

	if (textureColor.a < 0.1)
	{
		discard;
	}
	color = textureColor;

	//color = vec4(v_TexCoord.x, v_TexCoord.y, 0.0f, 1.0f); //DEBUG color checker
};