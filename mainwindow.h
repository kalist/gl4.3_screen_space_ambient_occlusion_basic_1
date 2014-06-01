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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtQuick/QQuickView>
#include <QTimer>
#include <QQmlContext>
#include <QDebug>
#include <QCoreApplication>

#include "datamanager.h"
#include "rendererdata.h"

class MainWindow : public QQuickView
{
    Q_OBJECT
    Q_PROPERTY(QSize windowsize READ windowsize WRITE setWindowsize NOTIFY windowsizeChanged)

public:
    explicit MainWindow(QWindow * parent = 0);
    virtual ~MainWindow();

    void setWindowsize(const QSize &size);
    DataManager *getDataManager();
    QSize windowsize() const;

public slots:
    void update_gui_data(const RendererData &data);

signals:
    void windowResizeDone(const QSize &size);
    void windowsizeChanged(const QSize &size);

protected:
     bool event(QEvent *event) override;
     void resizeEvent(QResizeEvent *event) override;
     void keyPressEvent(QKeyEvent *event) override;
     void keyReleaseEvent(QKeyEvent *event) override;
     void mouseReleaseEvent(QMouseEvent *event) override;
     void mouseMoveEvent(QMouseEvent *event) override;
     void mousePressEvent(QMouseEvent *event) override;

private:
     QTimer m_resizeTimer;
     QTimer m_processTimer;

     void resizeTimerTimeout();
     void processTimerTimeout();

     DataManager *wdm;
};

#endif // MAINWINDOW_H
