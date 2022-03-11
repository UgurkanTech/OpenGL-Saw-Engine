#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

// material parameters
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

// lights
uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 camPos;

uniform vec2 u_resolution;
uniform vec2 u_mouse;
uniform float u_time;


#ifdef GL_ES
precision mediump float;
#endif



void main()
{		
    //vec3 albedo = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2));

    //vec2 st = gl_FragCoord.xy/u_resolution.xy;

    vec3 color = vec3(0.792, 0, 0.839);

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 
    //FragColor = vec4(color,1.0);
    FragColor = vec4(color , 1.0);

}
