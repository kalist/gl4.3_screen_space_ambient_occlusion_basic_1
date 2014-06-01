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

#include "mainwindow.h"

#include <QQuickItem>

MainWindow::MainWindow(QWindow *parent)
    : QQuickView(parent)
{
    // Rendering in a thread introduces a slightly more complicated cleanup
    // so we ensure that no cleanup of graphics resources happen until the
    // application is shutting down.

    QSurfaceFormat *m_format = new QSurfaceFormat();
    //m_format->setDepthBufferSize(24);
    //m_format->setVersion(4, 3);
    //m_format->setSamples(8);
    m_format->setProfile(QSurfaceFormat::CompatibilityProfile);
    m_format->setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    setFormat(*m_format);

    setPersistentOpenGLContext(true);
    setPersistentSceneGraph(true);

    setClearBeforeRendering(false);
    setResizeMode(QQuickView::SizeRootObjectToView);

    setMinimumSize(QSize(720, 720));

    m_resizeTimer.setSingleShot(true);
    connect(&m_resizeTimer, &QTimer::timeout, this, &MainWindow::resizeTimerTimeout, Qt::DirectConnection);

    connect(&m_processTimer, &QTimer::timeout, this, &MainWindow::processTimerTimeout, Qt::DirectConnection);
    m_processTimer.start(5);

    wdm = new DataManager();
    rootContext()->setContextProperty("dataManager", wdm);
}

MainWindow::~MainWindow(){
    delete wdm;
}

DataManager *MainWindow::getDataManager(){
    return wdm;
}

void MainWindow::update_gui_data(const RendererData &data){

    rootObject()->findChild<QObject*>(QString("light_position_x"))->setProperty("value", data.getLightPositionX());
    rootObject()->findChild<QObject*>(QString("light_position_y"))->setProperty("value", data.getLightPositionY());
    rootObject()->findChild<QObject*>(QString("light_position_z"))->setProperty("value", data.getLightPositionZ());

    rootObject()->findChild<QObject*>(QString("samples_L0"))->setProperty("value", data.getSamplesL0());
    rootObject()->findChild<QObject*>(QString("samples_L1"))->setProperty("value", data.getSamplesL1());

    rootObject()->findChild<QObject*>(QString("scale"))->setProperty("value", data.getScale());
    rootObject()->findChild<QObject*>(QString("bias"))->setProperty("value", data.getBias());
    rootObject()->findChild<QObject*>(QString("radius"))->setProperty("value", data.getRadius());
    rootObject()->findChild<QObject*>(QString("intensity"))->setProperty("value", data.getIntensity());
    rootObject()->findChild<QObject*>(QString("depth_tolerance"))->setProperty("value", data.getDepthTolerance());
}

void MainWindow::setWindowsize(const QSize &size){
    resize(size);
    emit windowsizeChanged(size);
}

QSize MainWindow::windowsize() const {
    return size();
}

bool MainWindow::event(QEvent *event){
    //qDebug() << ev->type();
    return QQuickView::event(event);
}

void MainWindow::resizeTimerTimeout(){
   emit windowResizeDone(size());
}

void MainWindow::processTimerTimeout(){
   //update();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    emit windowsizeChanged(event->size());
    m_resizeTimer.start(100);
    QQuickView::resizeEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    QQuickView::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
    QQuickView::keyPressEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    QQuickView::mousePressEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    QQuickView::mouseReleaseEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    QQuickView::mouseMoveEvent(event);
}


