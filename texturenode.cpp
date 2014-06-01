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

#include "texturenode.h"

TextureNode::TextureNode(MainWindow *window)
    : m_id(0)
    , m_size(0, 0)
    , m_texture(0)
    , m_window(window)
{
    // Our texture node must have a texture, so use the default 0 texture.
    m_texture = m_window->createTextureFromId(0, QSize(1, 1));
    setTexture(m_texture);
    setFiltering(QSGTexture::Linear);
    m_elapsed_timer.start();
}

TextureNode::~TextureNode(){
    delete m_texture;
}

void TextureNode::changeSize(const QSize &size){
    emit sizeChanged(size);
}

void TextureNode::doneResize(const QSize &size){
    emit resizeDone(size);
}

// This function gets called on the FBO rendering thread and will store the
// texture id and size and schedule an update on the window.
void TextureNode::newDisplayTexture(int id, const QSize &size) {
    m_mutex.lock();
        m_id = id;
        m_size = size;
    m_mutex.unlock();

    // We cannot call QQuickWindow::update directly here, as this is only allowed
    // from the rendering thread or GUI thread.
    emit pendingNewDisplayTexture();
}

// Before the scene graph starts to render, we update to the pending texture
void TextureNode::prepareDisplayTexture() {
    m_mutex.lock();
        int newId = m_id;
        QSize size = m_size;
        m_id = 0;
    m_mutex.unlock();

    if (newId) {
        try{
            delete m_texture;
            m_texture = m_window->createTextureFromId(newId, size);
            setTexture(m_texture);

        }catch(std::exception e){
            qDebug()<< "frame skipped" << e.what();
        }

        m_elapsed_time = m_elapsed_timer.nsecsElapsed();
        m_elapsed_timer.restart();

        float dt = static_cast<float>(m_elapsed_time)/1000000000.0;
        //qDebug() << dt;

        // This will notify the rendering thread that the texture is now being rendered
        // and it can start rendering to the other one.
        emit requestNewDispalyTexture(dt);
       // emit pendingNewDisplayTexture();
    }
}

