#version 430 core

in vec2 texture_coords;

uniform mat4 ipMatrix;
uniform vec2 sampling_pattern_L0[32];
uniform vec2 sampling_pattern_L1[8];

uniform int ao_samples_L0 = 6;
uniform int ao_samples_L1 = 8;
uniform float ao_scale = 1.0f;
uniform float ao_bias = 0.1f;
uniform float ao_radius = 0.3f;
uniform float ao_intensity = 1.0f;
uniform float ao_depth_tolerance = 0.00001f;
uniform vec2 noise_offset;

layout(binding = 0)uniform sampler2D normal_texture;
layout(binding = 1)uniform sampler2D depth_texture;
layout(binding = 2)uniform sampler2D noise_texture;

layout(location = 0) out float color;

vec2 rand2f_a(){
    vec2 rand_vector = vec2(0);
    rand_vector.x = fract(sin(dot(texture_coords.xy, vec2(12.9898f, 78.233f))) * 43758.5453f);
    rand_vector.y = fract(sin(dot(texture_coords.yx, vec2(12.9898f, 78.233f))) * 43758.5453f);
    return normalize(rand_vector * 2.0f - 1.0f);
}

vec2 rand2f_b(){
    vec2 rand_vector = texture(noise_texture, texture_coords * noise_offset).rg;
    return normalize(rand_vector * 2.0f - 1.0f);
}

void main(void)
{
    float depth = texture(depth_texture, texture_coords).r;

    if(depth < 1.0f)
    {
        vec4 vertex_position = ipMatrix * vec4(texture_coords, depth, 1.0f);
        vertex_position.xyz /= vertex_position.w;

        vec3 normal_vector = texture(normal_texture, texture_coords).xyz * 2.0f - 1.0f;
        vec2 rand_vector = rand2f_b();

        float ao_factor = 0.0f;
        for(int i = 0; i < ao_samples_L0; ++i)
        {
            vec2 offset_L0 = sampling_pattern_L0[i] * rand_vector * ao_radius;

            for(int j = 0; j < ao_samples_L1; ++j)
            {
                vec2 new_texture_coords = texture_coords + offset_L0 * sampling_pattern_L1[j];
                vec4 new_vertex_position = ipMatrix * vec4(new_texture_coords, texture(depth_texture, new_texture_coords).r, 1.0f);
                new_vertex_position.xyz /= new_vertex_position.w;

                vec3 dv = new_vertex_position.xyz - vertex_position.xyz - ao_depth_tolerance;
                float dl = length(dv) * ao_scale;
                vec3 difference_vector = normalize(dv);

                ao_factor += max(0.1f, dot(normal_vector, difference_vector) - ao_bias) * (1.0f / (1.0f + dl)) * ao_intensity;
            }
        }

        ao_factor = 1.0f / (ao_factor /(ao_samples_L0 * ao_samples_L1));
        color = ao_factor;
    }
}
