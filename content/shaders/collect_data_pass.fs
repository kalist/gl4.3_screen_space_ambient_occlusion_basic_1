#version 430 core

in vec3 vertex_normal;      // interpolated eye space normal vector

layout(location=0) out vec4 color_normal;

void main(void)
{
    vec3 normal = normalize(vertex_normal) * 0.5f + 0.5f; // scale and bias normal vector to fit fs output
    color_normal = vec4(normal, 1.0f);
}
