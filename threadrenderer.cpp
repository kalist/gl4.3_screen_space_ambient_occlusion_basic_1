/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
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

#include "mainwindow.h"
#include "threadrenderer.h"
#include "renderer.h"
#include "texturenode.h"
#include "renderthread.h"
#include "datamanager.h"

#include <QtCore/QMutex>
#include <QtCore/QThread>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QGuiApplication>
#include <QtGui/QOffscreenSurface>

#include <QtQuick/QQuickWindow>
#include <qsgsimpletexturenode.h>


QList<QThread *> ThreadRenderer::threads;

ThreadRenderer::ThreadRenderer()
    :    m_window(0)
        ,m_renderThread(0)
{
    setFlag(ItemHasContents, true);
}

QSGNode *ThreadRenderer::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    TextureNode *node = static_cast<TextureNode *>(oldNode);

    if (!m_renderThread) {
        m_window = static_cast<MainWindow *>(window());
        QOpenGLContext *current = window()->openglContext();
        current->doneCurrent();
        m_renderThread = new RenderThread(window()->size(), current);
        current->makeCurrent(window());
        m_renderThread->moveToThread(m_renderThread);
        m_renderThread->start();
        connect(window(), &QQuickWindow::sceneGraphInvalidated, m_renderThread, &RenderThread::shutDown, Qt::QueuedConnection);

        connect(m_window->getDataManager(), &DataManager::dataChanged, m_renderThread, &RenderThread::setData, Qt::QueuedConnection);
        connect(m_window->getDataManager(), &DataManager::flagChanged, m_renderThread, &RenderThread::setFlag, Qt::QueuedConnection);
    }

    if (!node) {
        node = new TextureNode(static_cast<MainWindow *>(window()));

        /* Set up connections to get the production of FBO textures in sync with vsync on the
         * rendering thread.
         *
         * When a new texture is ready on the rendering thread, we use a direct connection to
         * the texture node to let it know a new texture can be used. The node will then
         * emit pendingNewTexture which we bind to QQuickWindow::update to schedule a redraw.
         *
         * When the scene graph starts rendering the next frame, the prepareNode() function
         * is used to update the node with the new texture. Once it completes, it emits
         * textureInUse() which we connect to the FBO rendering thread's renderNext() to have
         * it start producing content into its current "back buffer".
         *
         * This FBO rendering pipeline is throttled by vsync on the scene graph rendering thread.
         */

        connect(m_renderThread, &RenderThread::newDisplayTextureReady, node, &TextureNode::newDisplayTexture, Qt::DirectConnection);
        connect(node, &TextureNode::pendingNewDisplayTexture, window(), &QQuickView::update, Qt::QueuedConnection);

        connect(window(), &QQuickView::beforeRendering, node, &TextureNode::prepareDisplayTexture, Qt::DirectConnection);
        connect(node, &TextureNode::requestNewDispalyTexture, m_renderThread, &RenderThread::renderNextFrame, Qt::QueuedConnection);

        /* Set up connections that are used to keep actual window size for the the opengl context on the rendering thread
         * the resize event is transmited from QQuickWindow to thread through the texture node
         */

        //connect(m_window, &MainWindow::windowsizeChanged, node, &TextureNode::changeSize, Qt::DirectConnection);
        //connect(node, &TextureNode::sizeChanged, m_renderThread, &RenderThread::changeSize, Qt::QueuedConnection);
        connect(m_window, &MainWindow::windowResizeDone, node, &TextureNode::doneResize, Qt::DirectConnection);
        connect(node, &TextureNode::resizeDone, m_renderThread, &RenderThread::doneResize, Qt::QueuedConnection);

        connect(m_renderThread, &RenderThread::rendererReady, m_window, &MainWindow::update_gui_data, Qt::QueuedConnection);

        // Get the production of FBO textures started..
        QMetaObject::invokeMethod(m_renderThread, "renderNextFrame", Qt::QueuedConnection, Q_ARG(float, 0.005f));
        Sleep(20);
    }

    node->setRect(boundingRect());
    return node;
}
