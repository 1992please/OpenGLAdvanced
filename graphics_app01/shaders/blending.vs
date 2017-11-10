#version 330 core

layout (location = 0) in vec3 Position;                                             
layout (location = 1) in vec2 TexCoordIn;

uniform mat4 MVP;

out vec2 TexCoord;

void main()
{
    gl_Position = MVP * vec4(Position, 1.0);
    TexCoord = TexCoordIn;
}