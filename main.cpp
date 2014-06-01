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

#include <QtCore/QThread>
#include <QGuiApplication>

#include <QtGui/private/qguiapplication_p.h>
#include <QtGui/qpa/qplatformintegration.h>

#include <QPalette>
#include <QtQuick/QQuickView>

#include "mainwindow.h"
#include "threadRenderer.h"
#include "rendererdata.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    if (!QGuiApplicationPrivate::platform_integration->hasCapability(QPlatformIntegration::ThreadedOpenGL)) {
        QQuickView view;
        view.setSource(QUrl("qrc:///content/windows/error.qml"));
        view.show();
        return app.exec();
    }

    qmlRegisterType<ThreadRenderer>("SceneGraphRendering", 1, 0, "Renderer");
    qmlRegisterType<DataManager>("DataManager", 1, 0, "DataManager");

    qRegisterMetaType<DataManager::DataType>("DataType");
    qRegisterMetaType<DataManager::FlagType>("FlagType");
    qRegisterMetaType<RendererData>("RendererData");

    int execReturn = 0;

    {
        MainWindow view;
        view.setSource(QUrl("qrc:///content/windows/main.qml"));
        view.setTitle("OpenGL 4.3 AmbientOcclusionBasic - Piotr Sydow");
        view.show();
        view.raise();

        execReturn = app.exec();
    }

    // As the render threads make use of our QGuiApplication object
    // to clean up gracefully, wait for them to finish before
    // QGuiApp is taken off the heap.
    foreach (QThread *t, ThreadRenderer::threads) {
        t->wait();
        delete t;
    }

    return execReturn;
}
