#version 330

layout (location = 0) in vec3 Position;                                             
layout (location = 1) in vec2 TexCoord;                                             
layout (location = 2) in vec3 Normal;  

uniform mat4 MVP;

out vec4 Color;

void main()
{
    gl_Position = MVP * vec4(Position, 1.0);
    Color = vec4(0.0, 0.0, 0.0, 1.0);
}
