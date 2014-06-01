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
#ifndef TEXTURENODE_H
#define TEXTURENODE_H

#include "mainwindow.h"
#include "threadrenderer.h"

#include <QtCore/QMutex>
#include <QtCore/QThread>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QGuiApplication>
#include <QtGui/QOffscreenSurface>

#include <QtQuick/QQuickWindow>
#include <qsgsimpletexturenode.h>

#include <QElapsedTimer>

class TextureNode : public QObject, public QSGSimpleTextureNode
{
    Q_OBJECT

public:
    explicit TextureNode(MainWindow *window);
    ~TextureNode();

signals:
    void pendingNewDisplayTexture();
    void requestNewDispalyTexture(float dt);
    void sizeChanged(const QSize &size);
    void resizeDone(const QSize &size);

public slots:
    void changeSize(const QSize &size);
    void doneResize(const QSize &size);

    // This function gets called on the FBO rendering thread and will store the
    // texture id and size and schedule an update on the window.
    void newDisplayTexture(int id, const QSize &size);

    // Before the scene graph starts to render, we update to the pending texture
    void prepareDisplayTexture();

private:
    QElapsedTimer m_elapsed_timer;
    qint64 m_elapsed_time;

    int m_id;
    QSize m_size;

    QMutex m_mutex;
    MainWindow *m_window;
    QSGTexture *m_texture;
};




#endif // TEXTURENODE_H
