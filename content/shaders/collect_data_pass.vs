#version 430 core

layout(location = 0) in vec3 in_position; // object space vertex position
layout(location = 1) in vec2 in_texture;  // texture space coordinates
layout(location = 2) in vec3 in_normal;   // object space normal vector
layout(location = 3) in vec3 in_binormal; // object space binormal vector
layout(location = 4) in vec3 in_tangent;  // object space tangent vector

layout(location = 5) uniform mat4 pMatrix;   // projection matrix
layout(location = 6) uniform mat4 vMatrix;   // view matrix
layout(location = 7) uniform mat4 mMatrix;   // model matrix

out vec3 vertex_normal;     // eye space normal vector

void main(void)
{
    vertex_normal = (vMatrix * mMatrix * vec4(in_normal, 0.0f)).xyz;    // calculate eye space vertex normal
    gl_Position = pMatrix * vMatrix * mMatrix * vec4(in_position, 1.0); // calculate clipspace vertex position
}
