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
#ifndef RENDERTHREAD_H
#define RENDERTHREAD_H

#include "renderer.h"
#include "rendererdata.h"
#include "threadrenderer.h"
#include "datamanager.h"

#include <QtCore/QMutex>
#include <QtCore/QThread>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QGuiApplication>
#include <QtGui/QOffscreenSurface>

#include <QtQuick/QQuickWindow>
#include <qsgsimpletexturenode.h>

#include <QTime>
#include <QTimer>
#include <QElapsedTimer>

/*
 * The render thread shares a context with the scene graph and will
 * render into two separate FBOs, one to use for display and one
 * to use for rendering
 */

class RenderThread : public QThread
{
    Q_OBJECT

public:
    explicit RenderThread(const QSize &size, QOpenGLContext *context);
    void setSurface(QOffscreenSurface *surface) {m_fakeSurface = surface;}

public slots:
    void doneResize(const QSize &size);
    void changeSize(const QSize &size);
    void renderNextFrame(float dt = 0.002);
    void setData(DataManager::DataType type, float value);
    void setFlag(DataManager::FlagType type, bool value);
    float getData(DataManager::DataType type);
    bool getFlag(DataManager::FlagType type);

    void shutDown();

signals:
    void newDisplayTextureReady(int id, const QSize &size);
    void rendererReady(const RendererData &data);
    void massDataReady(float mass);

private:
    QOpenGLFramebufferObject *m_renderFboMs;
    QOpenGLFramebufferObject *m_renderFbo;
    QOpenGLFramebufferObject *m_displayFbo;

    QOpenGLFramebufferObjectFormat m_formatMs;
    QOpenGLFramebufferObjectFormat m_format;

    bool m_initialization_done;

    Renderer *m_renderer;

    QOffscreenSurface *m_fakeSurface;
    QOpenGLContext *m_context;
    QSize m_size;
    QSize m_size_new;
    bool m_size_new_flag;

    QTime *m_time;
    QTimer *m_timer;

    QElapsedTimer *m_elapsed_timer;
    qint64 m_elapsed_time;

    QMutex m_mutex;
};


#endif // RENDERTHREAD_H
