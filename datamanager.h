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

#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QDebug>
#include <QtQml>

class DataManager : public QObject
{
    Q_OBJECT
    Q_ENUMS(DataType)
    Q_ENUMS(FlagType)

public:
    DataManager();

   static enum DataType{
        LIGHT_POS_X,
        LIGHT_POS_Y,
        LIGHT_POS_Z,
        SAMPLES_L0,
        SAMPLES_L1,
        SCALE,
        BIAS,
        RADIUS,
        INTENSITY,
        DEPTH_TOLERANCE
    };

   static enum FlagType{
        SSAO_TOOGLE = 0,
        SSAO_RENDER_TOOGLE = 1,
        SSAO_VERTICAL_TOOGLE = 2,
        SSAO_HORIZONTAL_TOOGLE = 3,
        SCENE_RENDER_TOOGLE = 4,
        SCENE_DEPTH_TOOGLE = 5,
        SCENE_NORMAL_TOOGLE = 6,
        RESTORE_DEFAULTS
    };

public slots:
    void setData(DataType type, float value);
    void setFlag(FlagType type, bool value);

signals:
    void dataChanged(DataType type, float value);
    void flagChanged(FlagType type, bool value);
};

#endif // DATAMANAGER_H
