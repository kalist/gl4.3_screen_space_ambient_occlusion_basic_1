#version 430 core

in vec2 texture_coords;

uniform int render_mode;

layout(binding = 0)uniform sampler2D render_texture;
layout(binding = 1)uniform sampler2D ssao_vh_texture;
layout(binding = 2)uniform sampler2D ssao_v_texture;
layout(binding = 3)uniform sampler2D ssao_texture;
layout(binding = 4)uniform sampler2D normal_texture;
layout(binding = 5)uniform sampler2D depth_texture;

layout(location = 0) out vec4 color;

void main(void)
{
    float r;
    switch(render_mode){
        case 0:
                r = texture(ssao_vh_texture, texture_coords).r;
                color = texture(render_texture, texture_coords) * r; break;
        case 1:
                r = texture(ssao_texture, texture_coords).r;
                color = vec4(r, r, r, 1.0f); break;
        case 2:
                r = texture(ssao_v_texture, texture_coords).r;
                color = vec4(r, r, r, 1.0f); break;
        case 3:
                r = texture(ssao_vh_texture, texture_coords).r;
                color = vec4(r, r, r, 1.0f); break;
        case 4:
                color = texture(render_texture, texture_coords); break;
        case 5:
                color = texture(depth_texture, texture_coords); break;
        case 6:
                color = texture(normal_texture, texture_coords); break;
        default:
                color = vec4(1.0f);
    }
}
