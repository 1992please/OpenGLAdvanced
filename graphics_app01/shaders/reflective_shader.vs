#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

uniform mat4 gMVP;
uniform mat4 gModel;

void main()
{
    Normal = mat3(transpose(inverse(gModel))) * aNormal;
    Position = vec3(gModel * vec4(aPos, 1.0));
    gl_Position = gMVP * vec4(aPos, 1.0);
}