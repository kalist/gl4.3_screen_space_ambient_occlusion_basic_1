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

#include <GL/glew.h>
#include <GL/GL.h>
#include "renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

QOpenGLShader *Renderer::loadShader(QString filename,  QOpenGLShader::ShaderTypeBit shaderType, QObject *parent){

    QFile mFile(filename);

    if(!mFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "could not open file for read";
        return 0;
    }

    QTextStream in(&mFile);
    QString mText = in.readAll();
    mFile.close();

    QOpenGLShader *shader = new QOpenGLShader(shaderType, parent);
    shader->compileSourceCode(mText);

    return shader;
}

bool Renderer::initializeImages(bool loadFromFile){

    BYTE* data(0);
    GLint internalFormat(GL_RGB8);
    GLenum format(GL_BGR);
    GLenum type(GL_UNSIGNED_BYTE);
    unsigned int width(0), height(0);

    if(loadFromFile){
        char filename[] = "noise.png";
        FREE_IMAGE_FORMAT fileformat = FIF_UNKNOWN;
        FIBITMAP *dib(0);

        bool isTransparent(false);
        int bpp;

        fileformat = FreeImage_GetFileType(filename, 0);

        if(fileformat == FIF_UNKNOWN)
            fileformat = FreeImage_GetFIFFromFilename(filename);

        if(fileformat == FIF_UNKNOWN)
            return false;

        if(FreeImage_FIFSupportsReading(fileformat))
            dib = FreeImage_Load(fileformat, filename);

        if(!dib)
            return false;

        isTransparent = FreeImage_IsTransparent(dib);
        bpp = FreeImage_GetBPP(dib);

        if((!isTransparent && bpp != 32) || (isTransparent && bpp == 32)){
            width = FreeImage_GetWidth(dib);
            height = FreeImage_GetHeight(dib);
            FreeImage_FlipVertical(dib);
            data = FreeImage_GetBits(dib);
            FreeImage_Unload(dib);
        }
        else{
            FIBITMAP *dib32(0);
            FreeImage_FlipVertical(dib);
            dib32 = FreeImage_ConvertTo32Bits(dib);
            FreeImage_Unload(dib);
            width = FreeImage_GetWidth(dib32);
            height = FreeImage_GetHeight(dib32);
            bpp = FreeImage_GetBPP(dib32);
            data = new BYTE[width * height * 4];
            FreeImage_ConvertToRawBits(data, dib32, width * 4, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, false);
            FreeImage_Unload(dib32);
        }
        qDebug() << "isTransparent:" << isTransparent << "bpp:" << bpp;

        if(isTransparent){
            internalFormat = GL_RGBA8;
            format = GL_BGRA;
        }
        noise_size = vmath::vec2(width, height);

        glGenTextures(1, &noise_texture);
        glBindTexture(GL_TEXTURE_2D, noise_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    }
    else{
        width = 64;
        height = 64;

        unsigned length = width * height;
        std::vector<vmath::vec3> noise_data(length);

        for(unsigned int i = 0; i < length; i++){
            noise_data[i][0] = static_cast<float>(rand()) / RAND_MAX;
            noise_data[i][1] = static_cast<float>(rand()) / RAND_MAX;
            noise_data[i][2] = static_cast<float>(rand()) / RAND_MAX;
        }

        noise_size = vmath::vec2(width, height);

        glGenTextures(1, &noise_texture);
        glBindTexture(GL_TEXTURE_2D, noise_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, noise_data.data());
    }

    return true;
}

bool Renderer::initializeMeshes(){

    m = new Mesh("Temple/temple.obj", true);
    qm = new QuadMesh(true, true);

    return true;
}

bool Renderer::initializeShaders(){

    // calculate ssao required data
    QOpenGLShader *dataCollectPassVertexShader = loadShader(":/content/shaders/collect_data_pass.vs", QOpenGLShader::Vertex, &dataCollectPassProgram);
    QOpenGLShader *dataCollectPassFragmentShader = loadShader(":/content/shaders/collect_data_pass.fs", QOpenGLShader::Fragment, &dataCollectPassProgram);

    dataCollectPassProgram.addShader(dataCollectPassVertexShader);
    dataCollectPassProgram.addShader(dataCollectPassFragmentShader);
    dataCollectPassProgram.link();

    pMatLocation_generic = 5;
    vMatLocation_generic = 6;
    mMatLocation_generic = 7;



    // calculate the amount of ssao factor per pixel
    QOpenGLShader *calculateSSAOPassVertexShader = loadShader(":/content/shaders/calculate_ssao_pass.vs", QOpenGLShader::Vertex, &calculateSSAOPassProgram);
    QOpenGLShader *calculateSSAOPassFragmentShader = loadShader(":/content/shaders/calculate_ssao_pass.fs", QOpenGLShader::Fragment, &calculateSSAOPassProgram);

    calculateSSAOPassProgram.addShader(calculateSSAOPassVertexShader);
    calculateSSAOPassProgram.addShader(calculateSSAOPassFragmentShader);
    calculateSSAOPassProgram.link();

    ipMatLocation_loc = glGetUniformLocation(calculateSSAOPassProgram.programId(), "ipMatrix");
    sampling_pattern_L0_loc = glGetUniformLocation(calculateSSAOPassProgram.programId(), "sampling_pattern_L0");
    sampling_pattern_L1_loc = glGetUniformLocation(calculateSSAOPassProgram.programId(), "sampling_pattern_L1");
    ao_samples_L0_loc = glGetUniformLocation(calculateSSAOPassProgram.programId(), "ao_samples_L0");
    ao_samples_L1_loc = glGetUniformLocation(calculateSSAOPassProgram.programId(), "ao_samples_L1");
    ao_scale_loc = glGetUniformLocation(calculateSSAOPassProgram.programId(), "ao_scale");
    ao_bias_loc = glGetUniformLocation(calculateSSAOPassProgram.programId(), "ao_bias");
    ao_radius_loc = glGetUniformLocation(calculateSSAOPassProgram.programId(), "ao_radius");
    ao_intensity_loc = glGetUniformLocation(calculateSSAOPassProgram.programId(), "ao_intensity");
    ao_depth_tolerance_loc = glGetUniformLocation(calculateSSAOPassProgram.programId(), "ao_depth_tolerance");
    noise_offset_loc = glGetUniformLocation(calculateSSAOPassProgram.programId(), "noise_offset");


    // blur the ssao algorithm result
    QOpenGLShader *filterSSAOPassVertexShader = loadShader(":/content/shaders/filter_ssao_pass.vs", QOpenGLShader::Vertex, &filterSSAOPassProgram);
    QOpenGLShader *filterSSAOPassFragmentShader = loadShader(":/content/shaders/filter_ssao_pass.fs", QOpenGLShader::Fragment, &filterSSAOPassProgram);

    filterSSAOPassProgram.addShader(filterSSAOPassVertexShader);
    filterSSAOPassProgram.addShader(filterSSAOPassFragmentShader);
    filterSSAOPassProgram.link();

    h_gaussian_filter_index = glGetSubroutineIndex(filterSSAOPassProgram.programId(), GL_FRAGMENT_SHADER, "h_gussian_filter");
    v_gaussian_filter_index = glGetSubroutineIndex(filterSSAOPassProgram.programId(), GL_FRAGMENT_SHADER, "v_gussian_filter");


    // render image and resolve multisampling
    QOpenGLShader *renderPassVertexShader = loadShader(":/content/shaders/render_pass.vs", QOpenGLShader::Vertex, &renderPassProgram);
    QOpenGLShader *renderPassFragmentShader = loadShader(":/content/shaders/render_pass.fs", QOpenGLShader::Fragment, &renderPassProgram);

    renderPassProgram.addShader(renderPassVertexShader);
    renderPassProgram.addShader(renderPassFragmentShader);
    renderPassProgram.link();

    pMatLocation_generic = 5;
    vMatLocation_generic = 6;
    mMatLocation_generic = 7;
    light_position_loc  = glGetUniformLocation(renderPassProgram.programId(), "light_position");



    // compose final image render with processed ssao
    QOpenGLShader *composePassVertexShader = loadShader(":/content/shaders/compose_pass.vs", QOpenGLShader::Vertex, &composePassProgram);
    QOpenGLShader *composePassFragmentShader = loadShader(":/content/shaders/compose_pass.fs", QOpenGLShader::Fragment, &composePassProgram);

    composePassProgram.addShader(composePassVertexShader);
    composePassProgram.addShader(composePassFragmentShader);
    composePassProgram.link();

    render_mode_loc = glGetUniformLocation(composePassProgram.programId(), "render_mode");

    return true;
}

bool Renderer::initializeFrameBuffers(){

    glActiveTexture(GL_TEXTURE0);

    // fbo collect screen space normals and depth
    glGenFramebuffers(1, &dataCollectFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, dataCollectFbo);

    glGenTextures(1, &normal_texture);
    glGenTextures(1, &depth_texture);

    glBindTexture(GL_TEXTURE_2D, normal_texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, m_ssao_width, m_ssao_height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, normal_texture, 0);

    glBindTexture(GL_TEXTURE_2D, depth_texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, m_ssao_width, m_ssao_height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture, 0);

    qDebug() << "FBO COMPLETE:" << GL_FRAMEBUFFER_COMPLETE << "FBO RESULT:" << glCheckFramebufferStatus(GL_FRAMEBUFFER);


    // fbo ssao result
    glGenFramebuffers(1, &ssaoResultFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoResultFbo);

    glGenTextures(1, &ssao_texture);

    glBindTexture(GL_TEXTURE_2D, ssao_texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, m_ssao_width, m_ssao_height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssao_texture, 0);

    qDebug() << "FBO COMPLETE:" << GL_FRAMEBUFFER_COMPLETE << "FBO RESULT:" << glCheckFramebufferStatus(GL_FRAMEBUFFER);


    // fbo filter ssao data horizontaly and verticaly
    glGenFramebuffers(1, &blurResultFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, blurResultFbo);

    glGenTextures(1, &vblur_texture);
    glGenTextures(1, &hblur_texture);

    glBindTexture(GL_TEXTURE_2D, vblur_texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, m_ssao_width, m_ssao_height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, vblur_texture, 0);

    glBindTexture(GL_TEXTURE_2D, hblur_texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, m_ssao_width, m_ssao_height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, hblur_texture, 0);

    qDebug() << "FBO COMPLETE:" << GL_FRAMEBUFFER_COMPLETE << "FBO RESULT:" << glCheckFramebufferStatus(GL_FRAMEBUFFER);


    // fbo render with multisampling
    glGenFramebuffers(1, &renderFboMs);
    glBindFramebuffer(GL_FRAMEBUFFER, renderFboMs);

    glGenTextures(1, &render_texture_ms);
    glGenRenderbuffers(1, &render_buffer_ms);

    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, render_texture_ms);
    glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_RGBA8, m_width, m_height, GL_FALSE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, render_texture_ms, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, render_buffer_ms);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_DEPTH_COMPONENT24, m_width, m_height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, render_buffer_ms);

    qDebug() << "FBO COMPLETE:" << GL_FRAMEBUFFER_COMPLETE << "FBO RESULT:" << glCheckFramebufferStatus(GL_FRAMEBUFFER);


    // fbo render with antialiazing resolved
    glGenFramebuffers(1, &renderFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, renderFbo);

    glGenTextures(1, &render_texture);

    glBindTexture(GL_TEXTURE_2D, render_texture);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, m_width, m_height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, render_texture, 0);

    qDebug() << "FBO COMPLETE:" << GL_FRAMEBUFFER_COMPLETE << "FBO RESULT:" << glCheckFramebufferStatus(GL_FRAMEBUFFER);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

 bool Renderer::initializeData(int width, int height){

     m_width = width;
     m_height = height;
     m_ssao_width = m_width;
     m_ssao_height = m_height;

     noise_offset = vmath::vec2(m_ssao_width / noise_size[0], m_ssao_height / noise_size[1]);

     pMatrix = glm::perspective(50.0f, (float)m_width / (float)m_height, 5.0f, 150.0f);
     ipMatrix = glm::inverse(pMatrix);
     vMatrix = vmath::translate(0.0f, 5.0f, -10.0f) * vmath::rotate(0.0f, 0.0f, 180.0f);
     mMatrix = vmath::translate(0.0f, 0.0f, 0.0f) * vmath::rotate(0.0f, 20.0f, 0.0f);

     data = new RendererData();

     return true;
 }

void Renderer::initialize(int width, int height)
{
    initializeData(width, height);
    initializeShaders();
    initializeFrameBuffers();
    initializeImages(false);
    initializeMeshes();

    createSamplingPatternL0(32, 0.07f);
    createSamplingPatternL1(8);
}

void Renderer::createSamplingPatternL0(int samples, float fluctuations){

    sampling_pattern_L0.resize(samples);

    int i(1);
    float r(0);
    float angle(static_cast<float>(M_PI_4));
    for(std::vector<vmath::vec2>::iterator it = sampling_pattern_L0.begin(); it != sampling_pattern_L0.end(); ++it, ++i){
        r = static_cast<float>(i) / static_cast<float>(samples);
        (*it)[0] = cos(angle) * r;
        (*it)[1] = sin(angle) * r;
        r = (((std::rand() / RAND_MAX) * 2.0f) - 1.0f) * fluctuations;
        angle += static_cast<float>(M_PI_2) + r;

        if(!(i%4))
            angle += static_cast<float>(M_PI_2) + r;
    }
}

void Renderer::createSamplingPatternL1(int samples){

    samples = 8;
    sampling_pattern_L1.resize(samples);
    sampling_pattern_L1[0] = vmath::vec2(-1.0f, -1.0f);
    sampling_pattern_L1[1] = vmath::vec2(1.0f, 1.0f);
    sampling_pattern_L1[2] = vmath::vec2(1.0f, -1.0f);
    sampling_pattern_L1[3] = vmath::vec2(-1.0f, 1.0f);
    sampling_pattern_L1[4] = vmath::vec2(1.0f, 0.0f);
    sampling_pattern_L1[5] = vmath::vec2(-1.0f, 0.0f);
    sampling_pattern_L1[6] = vmath::vec2(0.0f ,1.0f);
    sampling_pattern_L1[7] = vmath::vec2(0.0f, -1.0f);
}

void Renderer::changeSize(int width, int height){
    m_width = width;
    m_height = height;

    m_ssao_width = m_width;
    m_ssao_height = m_height;

    noise_offset = vmath::vec2(m_ssao_width / noise_size[0], m_ssao_height / noise_size[1]);

    pMatrix = glm::perspective(50.0f, (float)m_width / (float)m_height, 5.0f, 50.0f);
    ipMatrix = glm::inverse(pMatrix);
}

void Renderer::setScreenBuffer(QOpenGLFramebufferObject *screen_buffer){
    m_screen_buffer = screen_buffer;
}

void Renderer::setScreenBuffer_h(GLuint screen_buffer){
    m_screen_buffer_h = screen_buffer;
}

void Renderer::oglBindScreenBuffer_h(){
    if(m_screen_buffer_h != GL_INVALID_VALUE) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_screen_buffer_h);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
    }
}

void Renderer::restoreRenderDataDefaults(){
    data->defaults();
}

void Renderer::render(float dt)
{
    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // collect data
    glBindFramebuffer(GL_FRAMEBUFFER, dataCollectFbo);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glViewport(0, 0, m_ssao_width, m_ssao_height);
    glColorMask(true, true, true, true);
    glDepthMask(true);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(dataCollectPassProgram.programId());

    glUniformMatrix4fv(pMatLocation_generic, 1, GL_FALSE, glm::value_ptr(pMatrix));
    glUniformMatrix4fv(vMatLocation_generic, 1, GL_FALSE, vMatrix);
    glUniformMatrix4fv(mMatLocation_generic, 1, GL_FALSE, mMatrix);

    m->Draw();

    // calculate ssao factor
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoResultFbo);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glViewport(0, 0, m_ssao_width, m_ssao_height);
    glColorMask(true, true, true, true);
    glDepthMask(true);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(calculateSSAOPassProgram.programId());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, normal_texture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depth_texture);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, noise_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glUniformMatrix4fv(ipMatLocation_loc, 1, GL_FALSE, glm::value_ptr(ipMatrix));
    glUniform2fv(sampling_pattern_L0_loc, 32, &(sampling_pattern_L0[0][0]));
    glUniform2fv(sampling_pattern_L1_loc, 8, &(sampling_pattern_L1[0][0]));
    glUniform2fv(noise_offset_loc, 1, &(noise_offset[0]));

    glUniform1i(ao_samples_L0_loc, data->getSamplesL0());
    glUniform1i(ao_samples_L1_loc, data->getSamplesL1());
    glUniform1f(ao_scale_loc, data->getScale());
    glUniform1f(ao_bias_loc, data->getBias());
    glUniform1f(ao_radius_loc, data->getRadius());
    glUniform1f(ao_intensity_loc, data->getIntensity());
    glUniform1f(ao_depth_tolerance_loc, data->getDepthTolerance());

    qm->Draw();

    // first step filter - vertical smoothing
    glBindFramebuffer(GL_FRAMEBUFFER, blurResultFbo);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glViewport(0, 0, m_ssao_width, m_ssao_height);
    glColorMask(true, true, true, true);
    glDepthMask(true);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(filterSSAOPassProgram.programId());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ssao_texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &v_gaussian_filter_index);

    qm->Draw();

    // second step filter - horizontal smoothing
    glBindFramebuffer(GL_FRAMEBUFFER, blurResultFbo);
    glDrawBuffer(GL_COLOR_ATTACHMENT1);
    glViewport(0, 0, m_ssao_width, m_ssao_height);
    glColorMask(true, true, true, true);
    glDepthMask(true);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(filterSSAOPassProgram.programId());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, vblur_texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &h_gaussian_filter_index);

    qm->Draw();

    // render original image with diffuse basic lighting
    glBindFramebuffer(GL_FRAMEBUFFER, renderFboMs);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    glViewport(0, 0, m_width, m_height);
    glColorMask(true, true, true, true);
    glDepthMask(true);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(renderPassProgram.programId());

    glUniformMatrix4fv(pMatLocation_generic, 1, GL_FALSE, glm::value_ptr(pMatrix));
    glUniformMatrix4fv(vMatLocation_generic, 1, GL_FALSE, vMatrix);
    glUniformMatrix4fv(mMatLocation_generic, 1, GL_FALSE, mMatrix);
    glUniform3fv(light_position_loc, 1, data->getLigthPosition());

    m->Draw();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, renderFboMs);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderFbo);
    glBlitFramebuffer(0,0, m_width, m_height, 0,0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    // compose final image with filtered ssao_texture
    oglBindScreenBuffer_h();
    glViewport(0, 0, m_width, m_height);
    glColorMask(true, true, true, true);
    glDepthMask(true);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(composePassProgram.programId());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, render_texture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, hblur_texture);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, vblur_texture);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, ssao_texture);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, normal_texture);

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, depth_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);

    glUniform1i(render_mode_loc, data->getRenderMode());

    qm->Draw();
}


