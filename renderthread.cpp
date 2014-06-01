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
#include "renderthread.h"


RenderThread::RenderThread(const QSize &size, QOpenGLContext *context)
        : m_renderFboMs(0)
        , m_renderFbo(0)
        , m_displayFbo(0)
        , m_renderer(0)
        , m_fakeSurface(0)
        , m_size(size)
        , m_size_new(size)
        , m_size_new_flag(false)
{
    ThreadRenderer::threads << this;

    // Set up the QOpenGLContext to use for rendering in this thread. It is sharing
    // memory space with the GL context of the scene graph. This constructor is called
    // during updatePaintNode, so we are currently on the scene graph thread with the
    // scene graph's OpenGL context current.

    QSurfaceFormat *m_format = new QSurfaceFormat();
    m_format->setProfile(QSurfaceFormat::CompatibilityProfile);
    m_format->setVersion(4, 3);

    m_context = new QOpenGLContext();
    m_context->setShareContext(context);
    m_context->setFormat(*m_format);
    m_context->create();

    m_context->moveToThread(this);

    // We need a non-visible surface to make current in the other thread
    // and QWindows must be created and managed on the GUI thread.
    m_fakeSurface = new QOffscreenSurface();
    m_fakeSurface->setFormat(*m_format);
    m_fakeSurface->create();

    //m_timer = new QTimer();
    //m_timer->moveToThread(this);

    m_elapsed_timer = new QElapsedTimer();
    //m_elapsed_timer->start();

    m_initialization_done = false;
}

void RenderThread::doneResize(const QSize &size){
    m_mutex.lock();
        m_size_new.setWidth(size.width());
        m_size_new.setHeight(size.height());
        m_size_new_flag = true;
    m_mutex.unlock();
}

void RenderThread::changeSize(const QSize &size){

    //qDebug() << size.width() << " " << size.height();
    if(abs(size.width() - m_size.width() < 100 || abs(size.height() - m_size.height() < 100)))
        return;

    m_mutex.lock();
        m_size_new.setWidth(size.width());
        m_size_new.setHeight(size.height());
        m_size_new_flag = true;
    m_mutex.unlock();
}

void RenderThread::setData(DataManager::DataType type, float value){
    switch(type){
        case DataManager::LIGHT_POS_X :{
            //qDebug() <<value;
            m_renderer->getRendererData().setLightPositionX(value);
            break;
        }
        case DataManager::LIGHT_POS_Y :{
            //qDebug() <<value;
            m_renderer->getRendererData().setLightPositionY(value);
            break;
        }
        case DataManager::LIGHT_POS_Z :{
            //qDebug() <<value;
            m_renderer->getRendererData().setLightPositionZ(value);
            break;
        }
        case DataManager::SAMPLES_L0 :{
            //qDebug() <<value;
            m_renderer->getRendererData().setSamplesL0(static_cast<int>(value));
            break;
        }
        case DataManager::SAMPLES_L1 :{
            //qDebug() <<value;
            m_renderer->getRendererData().setSamplesL1(static_cast<int>(value));
            break;
        }
        case DataManager::SCALE :{
            //qDebug() <<value;
            m_renderer->getRendererData().setScale(value);
            break;
        }
        case DataManager::BIAS :{
            //qDebug() <<value;
            m_renderer->getRendererData().setBias(value);
            break;
        }
        case DataManager::RADIUS :{
            //qDebug() <<value;
            m_renderer->getRendererData().setRadius(value);
            break;
        }
        case DataManager::INTENSITY :{
            //qDebug() <<value;
            m_renderer->getRendererData().setIntensity(value);
            break;
        }
        case DataManager::DEPTH_TOLERANCE :{
            //qDebug() <<value;
            m_renderer->getRendererData().setDepthTolerance(value);
            break;
        }
    }
}

void RenderThread::setFlag(DataManager::FlagType type, bool value){
    switch(type){
        case DataManager::SSAO_TOOGLE :{
            //qDebug() <<value;
            m_renderer->getRendererData().setSsaoState(value);
            m_renderer->getRendererData().setRenderMode(type);
            break;
        }
        case DataManager::SSAO_RENDER_TOOGLE :{
            //qDebug() <<value;
            m_renderer->getRendererData().setSsaoRenderState(value);
            m_renderer->getRendererData().setRenderMode(type);
            break;
        }
        case DataManager::SSAO_VERTICAL_TOOGLE :{
            //qDebug() <<value;
            m_renderer->getRendererData().setSsaoRenderVState(value);
            m_renderer->getRendererData().setRenderMode(type);
            break;
        }
        case DataManager::SSAO_HORIZONTAL_TOOGLE :{
            //qDebug() <<value;
            m_renderer->getRendererData().setSsaoRenderVHState(value);
            m_renderer->getRendererData().setRenderMode(type);
            break;
        }
        case DataManager::SCENE_RENDER_TOOGLE :{
            //qDebug() <<value;
            m_renderer->getRendererData().setSceneRenderState(value);
            m_renderer->getRendererData().setRenderMode(type);
            break;
        }
        case DataManager::SCENE_NORMAL_TOOGLE :{
            //qDebug() <<value;
            m_renderer->getRendererData().setSceneNormalState(value);
            m_renderer->getRendererData().setRenderMode(type);
            break;
        }
        case DataManager::SCENE_DEPTH_TOOGLE :{
            //qDebug() <<value;
            m_renderer->getRendererData().setSceneDepthState(value);
            m_renderer->getRendererData().setRenderMode(type);
            break;
        }
        case DataManager::RESTORE_DEFAULTS :{
            //qDebug() <<value;
            m_renderer->restoreRenderDataDefaults();
            emit rendererReady(m_renderer->getRendererData());
            break;
        }
    }
}

float RenderThread::getData(DataManager::DataType type){
    switch(type){
        case DataManager::LIGHT_POS_X :{
            return m_renderer->getRendererData().getLightPositionX();
        }
        case DataManager::LIGHT_POS_Y :{
            return m_renderer->getRendererData().getLightPositionY();
        }
        case DataManager::LIGHT_POS_Z :{
            return m_renderer->getRendererData().getLightPositionZ();
        }
        case DataManager::SAMPLES_L0 :{
            return m_renderer->getRendererData().getSamplesL0();
        }
        case DataManager::SAMPLES_L1 :{
            return m_renderer->getRendererData().getSamplesL1();
        }
        case DataManager::SCALE :{
            return m_renderer->getRendererData().getScale();
        }
        case DataManager::BIAS :{
            return m_renderer->getRendererData().getBias();
        }
        case DataManager::RADIUS :{
            return m_renderer->getRendererData().getRadius();
        }
        case DataManager::INTENSITY :{
            return m_renderer->getRendererData().getIntensity();
        }
        case DataManager::DEPTH_TOLERANCE :{
            return m_renderer->getRendererData().getDepthTolerance();
        }
    }

    return 0;
}

bool RenderThread::getFlag(DataManager::FlagType type)
{
    switch(type){
        case DataManager::SSAO_TOOGLE :{
            return m_renderer->getRendererData().getSsaoState();
        }
        case DataManager::SSAO_RENDER_TOOGLE :{
            return m_renderer->getRendererData().getSsaoRenderState();
        }
        case DataManager::SSAO_VERTICAL_TOOGLE :{
            m_renderer->getRendererData().getSsaoRenderVState();
        }
        case DataManager::SSAO_HORIZONTAL_TOOGLE :{
            m_renderer->getRendererData().getSsaoRenderVHState();
        }
        case DataManager::SCENE_RENDER_TOOGLE :{
            m_renderer->getRendererData().getSceneRenderState();
        }
        case DataManager::SCENE_NORMAL_TOOGLE :{
            m_renderer->getRendererData().getSceneNormalState();
        }
        case DataManager::SCENE_DEPTH_TOOGLE :{
            m_renderer->getRendererData().getSceneDepthState();
        }
        case DataManager::RESTORE_DEFAULTS :{
            return true;
        }
    }
    return false;
}

void RenderThread::renderNextFrame(float dt)
{
    m_context->makeCurrent(m_fakeSurface);

    //m_elapsed_time = m_elapsed_timer->nsecsElapsed();
    //m_elapsed_timer->restart();

    if(m_size_new_flag){
        m_mutex.lock();
            m_size.setWidth(m_size_new.width());
            m_size.setHeight(m_size_new.height());
            if(m_renderer)
                m_renderer->changeSize(m_size.width(), m_size.height());
            m_size_new_flag = false;
        m_mutex.unlock();

        if(m_displayFbo){
            delete m_displayFbo;
            m_displayFbo = 0;
        }

        if(m_renderFbo){
            delete m_renderFbo;
            m_renderFbo = 0;
        }
    }

    if (!m_renderFbo) {

        if(!m_initialization_done){
             GLuint e = glewInit();
             m_format.setAttachment(QOpenGLFramebufferObject::Depth);

             m_renderer = new Renderer();
             m_renderer->initialize(m_size.width(), m_size.height());
             emit rendererReady(m_renderer->getRendererData());

             m_initialization_done = true;
        }

        // Initialize the buffers and renderer
        m_displayFbo = new QOpenGLFramebufferObject(m_size, m_format);
        m_renderFbo = new QOpenGLFramebufferObject(m_size, m_format);

        m_renderer->setScreenBuffer_h(m_renderFbo->handle());
    }

    m_renderer->render(dt);

    // We need to flush the contents to the FBO before posting
    // the texture to the other thread, otherwise, we might
    // get unexpected results.


    qSwap(m_renderFbo, m_displayFbo);
    m_renderer->setScreenBuffer_h(m_displayFbo->handle());
    emit newDisplayTextureReady(m_displayFbo->texture(), m_size);
}

void RenderThread::shutDown()
{
    m_context->makeCurrent(m_fakeSurface);
    delete m_renderFboMs;
    delete m_renderFbo;
    delete m_displayFbo;
    delete m_renderer;
    m_context->doneCurrent();
    delete m_context;

    // schedule this to be deleted only after we're done cleaning up
    m_fakeSurface->deleteLater();

    // Stop event processing, move the thread to GUI and make sure it is deleted.
    exit();
    moveToThread(QGuiApplication::instance()->thread());
}

