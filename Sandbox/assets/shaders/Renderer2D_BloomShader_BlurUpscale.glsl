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
// uniform float u_Mix;

void main()
{
    // The filter kernel is applied with a radius, specified in texture
    // coordinates, so that the radius will vary across mip resolutions.
    const float x = 0.005;
    const float y = 0.005;

    // Take 9 samples around current texel:
    // a - b - c
    // d - e - f
    // g - h - i
    // === ('e' is the current texel) ===
    vec3 a = texture(u_Textures[1], vec2(v_TexCoord.x - x, v_TexCoord.y + y)).rgb;
    vec3 b = texture(u_Textures[1], vec2(v_TexCoord.x,     v_TexCoord.y + y)).rgb;
    vec3 c = texture(u_Textures[1], vec2(v_TexCoord.x + x, v_TexCoord.y + y)).rgb;

    vec3 d = texture(u_Textures[1], vec2(v_TexCoord.x - x, v_TexCoord.y)).rgb;
    vec3 e = texture(u_Textures[1], vec2(v_TexCoord.x,     v_TexCoord.y)).rgb;
    vec3 f = texture(u_Textures[1], vec2(v_TexCoord.x + x, v_TexCoord.y)).rgb;

    vec3 g = texture(u_Textures[1], vec2(v_TexCoord.x - x, v_TexCoord.y - y)).rgb;
    vec3 h = texture(u_Textures[1], vec2(v_TexCoord.x,     v_TexCoord.y - y)).rgb;
    vec3 i = texture(u_Textures[1], vec2(v_TexCoord.x + x, v_TexCoord.y - y)).rgb;

    // Apply weighted distribution, by using a 3x3 tent filter:
    //  1   | 1 2 1 |
    // -- * | 2 4 2 |
    // 16   | 1 2 1 |
    vec3 upsample = vec3(0);
    upsample = texture(u_Textures[0], v_TexCoord).rgb;
    upsample += e*4.0;
    upsample += (b+d+f+h)*2.0;
    upsample += (a+c+g+i);
    upsample *= 1.0 / 16.0;

    vec4 currentDownscaleTexture = texture(u_Textures[0], v_TexCoord);

    float u_Mix=1;
    o_Color = vec4(upsample,1);
    // o_Color = vec4(upsample,1) + (u_Mix*currentDownscaleTexture);
    o_Color.a = 1;
};
