#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 gCameraPos;
uniform samplerCube gCubeMap;

void main()
{   
	 float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - gCameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    FragColor = vec4(texture(gCubeMap, R).rgb, 1.0);
}