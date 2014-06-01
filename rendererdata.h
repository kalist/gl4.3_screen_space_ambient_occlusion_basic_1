/****************************************************************************
**
** Piotr Sydow, 2014
** Contact: piotr.sydow.gm@gmail.com
**
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#pragma once
#ifndef RENDERERDATA_H
#define RENDERERDATA_H

#include <mtools/vmath.h>



class RendererData{
public:
    RendererData(){
        defaults();
    }

    RendererData(RendererData *data){
        light_pos_x = data->light_pos_x;
        light_pos_y = data->light_pos_y;
        light_pos_z = data->light_pos_z;

        ao_samples_L0 = data->ao_samples_L0;
        ao_samples_L1 = data->ao_samples_L1;

        ao_scale = data->ao_scale;
        ao_bias = data->ao_bias;
        ao_radius = data->ao_radius;
        ao_intensity = data->ao_intensity;
        ao_depth_tolerance = data->ao_depth_tolerance;

        ao_toggle = data->ao_toggle;
    }

    void defaults(){

        light_pos_x = 0.0f;
        light_pos_y = 0.0f;
        light_pos_z = 0.0f;

        ao_samples_L0 = 12;
        ao_samples_L1 = 8;

        ao_scale = 0.11f;
        ao_bias = -0.04f;
        ao_radius = 0.03f;
        ao_intensity = 6.0f;
        ao_depth_tolerance = 0.00001f;

        ao_toggle = true;
        render_mode = 0;
    }

    vmath::vec3 getLigthPosition(){
        return vmath::vec3(light_pos_x, light_pos_y, light_pos_z);
    }

    void setLightPositionX(float x){
        light_pos_x = x;
    }

    void setLightPositionY(float y){
        light_pos_y = y;
    }

    void setLightPositionZ(float z){
        light_pos_z = z;
    }

    float getLightPositionX() const{
       return light_pos_x;
    }

    float getLightPositionY() const{
        return light_pos_y;
    }

    float getLightPositionZ() const{
        return light_pos_z;
    }



    void setSamplesL0(int value){
        ao_samples_L0 = value;
    }

    int getSamplesL0() const{
        return ao_samples_L0;
    }



    void setSamplesL1(int value){
        ao_samples_L1 = value;
    }

    int getSamplesL1() const{
        return ao_samples_L1;
    }



    void setScale(float value){
        ao_scale = value;
    }

    float getScale() const{
        return ao_scale;
    }



    void setBias(float value){
        ao_bias = value;
    }

    float getBias() const{
        return ao_bias;
    }



    void setRadius(float value){
        ao_radius = value;
    }

    float getRadius() const{
        return ao_radius;
    }



    void setIntensity(float value){
        ao_intensity = value;
    }

    float getIntensity() const{
        return ao_intensity;
    }



    void setDepthTolerance(float value){
        ao_depth_tolerance = value;
    }

    float getDepthTolerance() const{
        return ao_depth_tolerance;
    }

// FLAGS

    void setSsaoState(bool state){
        ao_toggle = state;
    }
    bool getSsaoState() const {
        return ao_toggle;
    }


    void setSsaoRenderState(bool state){
        ao_render = state;
    }
    bool getSsaoRenderState() const {
        return ao_render;
    }


    void setSsaoRenderVState(bool state){
        ao_render_vsmooth = state;
    }
    bool getSsaoRenderVState() const {
        return ao_render_vsmooth;
    }


    void setSsaoRenderVHState(bool state){
        ao_render_vhsmooth = state;
    }
    bool getSsaoRenderVHState() const {
        return ao_render_vhsmooth;
    }


    void setSceneRenderState(bool state){
        scene_render = state;
    }
    bool getSceneRenderState() const {
        return scene_render;
    }


    void setSceneDepthState(bool state){
        scene_depth = state;
    }
    bool getSceneDepthState() const {
        return scene_depth;
    }


    void setSceneNormalState(bool state){
        scene_normal = state;
    }
    bool getSceneNormalState() const {
        return scene_normal;
    }


    void setRenderMode(int value){
        render_mode = value;
    }
    int getRenderMode() const {
        return render_mode;
    }


 private:
    float light_pos_x;
    float light_pos_y;
    float light_pos_z;

    int ao_samples_L0;
    int ao_samples_L1;

    float ao_scale;
    float ao_bias;
    float ao_radius;
    float ao_intensity;

    float ao_depth_tolerance;

    bool ao_toggle;
    bool ao_render;
    bool ao_render_vsmooth;
    bool ao_render_vhsmooth;
    bool scene_render;
    bool scene_depth;
    bool scene_normal;

    int render_mode;
};

#endif // RENDERERDATA_H
