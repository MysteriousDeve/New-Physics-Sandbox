#version 460 core

uniform vec2 Position = vec2(0, 0);
uniform vec2 Size = vec2(0.5, 0.5);
uniform vec4 Color;

uniform mat4x4 CameraMatrix = mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
uniform vec2 Pan = vec2(0, 0);

layout (location = 0) in vec3 aPos;

out vec2 quadCoord;

void main()
{
    gl_Position = CameraMatrix * vec4(aPos.xy * Size + Position - Pan, aPos.z, 1.0);
    quadCoord = vec2(aPos.x * 2, aPos.y * 2);
}