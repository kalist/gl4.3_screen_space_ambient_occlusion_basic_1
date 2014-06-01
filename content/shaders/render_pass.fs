#version 430 core

in vec3 vertex_position;    // interpolated eye space vertex position
in vec3 vertex_normal;      // interpolated eye space normal vector
in vec2 vertex_textureCoord;// interpolated texture space coordinates

uniform vec3 light_position = vec3(0.0f, 0.0f, 0.0f);

uniform vec4 light_ambient_color = vec4(0.1f, 0.1f, 0.1f, 1.0f);
uniform vec4 light_diffuse_color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

layout(location = 0) out vec4 color;

void main(void)
{
    vec3 normal_vector = normalize(vertex_normal);
    vec3 light_vector = normalize(light_position - vertex_position);
    vec4 ambient_color = light_ambient_color;
    vec4 diffuse_color = max(0.0f, dot(light_vector, normal_vector)) * light_diffuse_color;

    color = ambient_color + diffuse_color;
}
