#version 430 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texture;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec3 in_binormal;
layout(location = 4) in vec3 in_tangent;

layout(location = 5) uniform mat4 pMatrix;   // projection matrix
layout(location = 6) uniform mat4 vMatrix;   // view matrix
layout(location = 7) uniform mat4 mMatrix;   // model matrix

out vec3 vertex_position;
out vec3 vertex_normal;
out vec2 vertex_textureCoord;

void main(void)
{
    vertex_textureCoord = in_texture;
    vertex_normal = (vMatrix * mMatrix * vec4(in_normal, 0.0f)).xyz;
    vertex_position = (vMatrix * mMatrix * vec4(in_position, 1.0f)).xyz;

    gl_Position = pMatrix * vMatrix * mMatrix * vec4(in_position, 1.0);
}
