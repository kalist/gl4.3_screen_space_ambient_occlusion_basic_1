#version 430 core

layout(location = 0) in vec2 in_position; // vertex in object space

out vec2 texture_coords;

void main(void)
{
    texture_coords = in_position;  // object space texture coords;
    gl_Position = vec4((in_position * 2.0f) - 1.0f, 0.0f, 1.0f); // clipspace position
}
