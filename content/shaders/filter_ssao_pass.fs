#version 430 core

in vec2 texture_coords;

layout(binding = 0)uniform sampler2D ssao_texture;

// gaussian filtering samples
const float filter_kernel[] = float[21] (0.000272337,  0.00089296, 0.002583865, 0.00659813,  0.014869116,
                                         0.029570767, 0.051898313, 0.080381679, 0.109868729, 0.132526984,
                                         0.14107424,  0.132526984, 0.109868729, 0.080381679, 0.051898313,
                                         0.029570767, 0.014869116, 0.00659813,  0.002583865, 0.00089296, 0.000272337);


layout(location = 0) out float color;

subroutine float filterModelType();
subroutine uniform filterModelType filterModel;

subroutine(filterModelType)
float h_gussian_filter(){
    int  index = 20;
    float color = 0.0f;
    float dh = 1.0f / textureSize(ssao_texture, 0).x;

    for(int i=-10; i<=10; ++i, --index){
        color += filter_kernel[index] * texture(ssao_texture, texture_coords + vec2(i*dh, 0)).r;
    }

    return color;
}

subroutine(filterModelType)
float v_gussian_filter(){
    int  index = 20;
    float color = 0.0f;
    float dv = 1.0f / textureSize(ssao_texture, 0).y;

    for(int i=-10; i<=10; ++i, --index){
        color += filter_kernel[index] * texture(ssao_texture, texture_coords + vec2(0, i*dv)).r;
    }

    return color;
}

void main(void)
{
    color = filterModel();
}
