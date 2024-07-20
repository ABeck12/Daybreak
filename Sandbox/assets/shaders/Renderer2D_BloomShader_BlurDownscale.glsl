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

uniform float u_Directions;
uniform float u_Quality;
uniform float u_Size;
uniform vec2 u_Resolution;

void main()
{
    
    float Pi = 6.28318530718; // Pi*2
    
    float Directions = u_Directions;
    float Size = u_Size;
    float Quality = u_Quality;


    // GAUSSIAN BLUR SETTINGS {{{
    // float Directions = 16.0; // BLUR DIRECTIONS (Default 16.0 - More is better but slower)
    // float Quality = 10.0; // BLUR QUALITY (Default 4.0 - More is better but slower)
    // float Size = 32.0; // BLUR SIZE (Radius)
    // GAUSSIAN BLUR SETTINGS }}}

    vec4 previousBufferTexture = texture(u_Textures[0], v_TexCoord);

    // vec2 iResolution = vec2(1920,1080);
    vec2 iResolution = vec2(1280,720);
    iResolution = u_Resolution;


    vec2 Radius = Size/iResolution.xy;
    
    // const float dTheta = 0.05;
    // Blur calculations
    for( float d=0.0; d<Pi; d+=Pi/Directions)
    {
		for(float i=1.0/Quality; i<=1.0; i+=1.0/Quality)
        {
			o_Color += texture( u_Textures[0], v_TexCoord+vec2(cos(d),sin(d))*Radius*i);

            // vec4 color1 = texture( u_Textures[0], v_TexCoord+vec2(cos(d),sin(d))*Radius*i);
            // vec4 color2 = texture( u_Textures[0], v_TexCoord+vec2(cos(d+dTheta),sin(d+dTheta))*Radius*i);
            // vec4 color3 = texture( u_Textures[0], v_TexCoord+vec2(cos(d-dTheta),sin(d-dTheta))*Radius*i);

            // o_Color += (color1+color2+color3)/3;
            // Color += color1;
        }
    }
    
    // Output to screen
    o_Color /= Quality * Directions + 15;
    // o_Color /= Quality * Directions;
};