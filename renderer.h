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
#ifndef RENDERER_H
#define RENDERER_H

#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFramebufferObject>

#include "rendererdata.h"
#include "mesh.h"
#include "quadmesh.h"

#include <cstdio>
#include <stdio.h>
#include <vector>
#include <set>

#include <QTime>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QTextStream>
#include <QResource>
#include <QIODevice>

#include <mtools/vmath.h>
#include <mtools/glm/glm.hpp>
#include <mtools/glm/gtc/type_ptr.hpp>
#include <mtools/glm/gtc/matrix_inverse.hpp>
#include <mtools/glm/gtc/matrix_transform.hpp>
#include <freeimage/FreeImage.h>


class Renderer {

public:
    Renderer();
    ~Renderer();

    void render(float dt);
    void initialize(int width, int height);
    RendererData& getRendererData() const{
        return *data;
    }
    bool initializeImages(bool loadFromFile);
    bool initializeMeshes();
    bool initializeShaders();
    bool initializeFrameBuffers();
    bool initializeData(int width, int height);

    void changeSize(int width, int height);
    void restoreRenderDataDefaults();

    void setScreenBuffer(QOpenGLFramebufferObject *screen_buffer);
    void setScreenBuffer_h(GLuint screen_buffer);
    void createSamplingPatternL0(int samples, float fluctuations);
    void createSamplingPatternL1(int samples);

private:
    QOpenGLShader *loadShader(QString filename, QOpenGLShader::ShaderTypeBit shaderType, QObject *parent);
    inline void oglBindScreenBuffer() {if(m_screen_buffer) {m_screen_buffer->bind(); glDrawBuffer(GL_COLOR_ATTACHMENT0);}}
    void oglBindScreenBuffer_h();

    QuadMesh *qm;
    Mesh *m;

    vmath::mat4 mMatrix;
    vmath::mat4 vMatrix;
    glm::mat4 pMatrix;
    glm::mat4 ipMatrix;
    vmath::vec2 noise_offset;
    vmath::vec2 noise_size;

    std::vector<vmath::vec2> sampling_pattern_L0;
    std::vector<vmath::vec2> sampling_pattern_L1;

    QOpenGLShaderProgram dataCollectPassProgram;
    QOpenGLShaderProgram calculateSSAOPassProgram;
    QOpenGLShaderProgram filterSSAOPassProgram;
    QOpenGLShaderProgram renderPassProgram;

    QOpenGLShaderProgram composePassProgram;

    GLuint pMatLocation_generic;
    GLuint vMatLocation_generic;
    GLuint mMatLocation_generic;

    GLuint light_position_loc;

    GLuint ipMatLocation_loc;
    GLuint sampling_pattern_L0_loc;
    GLuint sampling_pattern_L1_loc;
    GLuint ao_samples_L0_loc;
    GLuint ao_samples_L1_loc;
    GLuint ao_scale_loc;
    GLuint ao_bias_loc;
    GLuint ao_radius_loc;
    GLuint ao_intensity_loc;
    GLuint ao_depth_tolerance_loc;
    GLuint noise_offset_loc;

    GLuint h_gaussian_filter_index;
    GLuint v_gaussian_filter_index;

    GLuint render_mode_loc;

    int m_width;
    int m_height;

    int m_ssao_width;
    int m_ssao_height;

    GLuint dataCollectFbo;
    GLuint ssaoResultFbo;
    GLuint blurResultFbo;
    GLuint renderFboMs;
    GLuint renderFbo;

    GLuint render_texture;
    GLuint render_texture_ms;
    GLuint render_buffer_ms;

    GLuint normal_texture;
    GLuint depth_texture;

    GLuint ssao_texture;

    GLuint vblur_texture;
    GLuint hblur_texture;

    GLuint noise_texture;

    GLuint gl_result;

    GLuint m_screen_buffer_h;
    QOpenGLFramebufferObject *m_screen_buffer;

    RendererData* data;
};
#endif
