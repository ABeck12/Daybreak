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
uniform vec2 u_Resolution;

void main()
{

    vec2 srcTexelSize = 2 / u_Resolution;
    float x = srcTexelSize.x;
    float y = srcTexelSize.y;

    // Take 13 samples around current texel:
    // a - b - c
    // - j - k -
    // d - e - f
    // - l - m -
    // g - h - i
    // === ('e' is the current texel) ===
    vec3 a = texture(u_Textures[0], vec2(v_TexCoord.x - 2*x, v_TexCoord.y + 2*y)).rgb;
    vec3 b = texture(u_Textures[0], vec2(v_TexCoord.x,       v_TexCoord.y + 2*y)).rgb;
    vec3 c = texture(u_Textures[0], vec2(v_TexCoord.x + 2*x, v_TexCoord.y + 2*y)).rgb;

    vec3 d = texture(u_Textures[0], vec2(v_TexCoord.x - 2*x, v_TexCoord.y)).rgb;
    vec3 e = texture(u_Textures[0], vec2(v_TexCoord.x,       v_TexCoord.y)).rgb;
    vec3 f = texture(u_Textures[0], vec2(v_TexCoord.x + 2*x, v_TexCoord.y)).rgb;

    vec3 g = texture(u_Textures[0], vec2(v_TexCoord.x - 2*x, v_TexCoord.y - 2*y)).rgb;
    vec3 h = texture(u_Textures[0], vec2(v_TexCoord.x,       v_TexCoord.y - 2*y)).rgb;
    vec3 i = texture(u_Textures[0], vec2(v_TexCoord.x + 2*x, v_TexCoord.y - 2*y)).rgb;

    vec3 j = texture(u_Textures[0], vec2(v_TexCoord.x - x, v_TexCoord.y + y)).rgb;
    vec3 k = texture(u_Textures[0], vec2(v_TexCoord.x + x, v_TexCoord.y + y)).rgb;
    vec3 l = texture(u_Textures[0], vec2(v_TexCoord.x - x, v_TexCoord.y - y)).rgb;
    vec3 m = texture(u_Textures[0], vec2(v_TexCoord.x + x, v_TexCoord.y - y)).rgb;

    // Apply weighted distribution:
    // 0.5 + 0.125 + 0.125 + 0.125 + 0.125 = 1
    // a,b,d,e * 0.125
    // b,c,e,f * 0.125
    // d,e,g,h * 0.125
    // e,f,h,i * 0.125
    // j,k,l,m * 0.5
    // This shows 5 square areas that are being sampled. But some of them overlap,
    // so to have an energy preserving downsample we need to make some adjustments.
    // The weights are the distributed, so that the sum of j,k,l,m (e.g.)
    // contribute 0.5 to the final color output. The code below is written
    // to effectively yield this sum. We get:
    // 0.125*5 + 0.03125*4 + 0.0625*4 = 1
    vec3 downsample = vec3(0);
    downsample = e*0.125;
    downsample += (a+c+g+i)*0.03125;
    downsample += (b+d+f+h)*0.0625;
    downsample += (j+k+l+m)*0.125;

    o_Color = vec4(downsample,1);
};