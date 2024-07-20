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
uniform float u_Gamma;
uniform float u_Exposure;

// vec3 aces(vec3 x) 
// {
//     const float a = 2.51;
//     const float b = 0.03;
//     const float c = 2.43;
//     const float d = 0.59;
//     const float e = 0.14;
//     return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
// }

// vec3 tonemap(vec3 hdrColor)
// {
//     // const float gamma = 2.2;
//     // const float exposure = 0.1;
//     float gamma = u_Gamma;
//     float exposure = u_Exposure;
  
//     // exposure tone mapping
//     vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
//     // gamma correction 
//     mapped = pow(mapped, vec3(1.0 / gamma));
//     return mapped;
// }

void main()
{
    vec4 initialTexture = texture(u_Textures[0], v_TexCoord);
    vec4 lastUpscaleTexture = texture(u_Textures[1], v_TexCoord);

    o_Color = initialTexture + lastUpscaleTexture;
    // o_Color.xyz = aces(o_Color.xyz);
    // o_Color.xyz = tonemap(o_Color.xyz);
};