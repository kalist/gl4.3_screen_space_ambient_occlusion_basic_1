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

import QtQuick 2.0
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.1
import SceneGraphRendering 1.0
import DataManager 1.0

Item {
    width: 512
    height: 512

    Renderer {
        id: renderer
        anchors.fill: parent
        anchors.margins: 0
        objectName: "Renderer"
    }

    Button {
       id: visible
       x: 10
       y: 10
       width: 100
       height: 23
       style: buttonStyle
       text: "Hide Controls"

       property bool current_flag : true;

       onClicked: {
            current_flag = !current_flag;

           if(!current_flag)
               text = "Show Controls";
           else
               text = "Hide Controls";

            reset_data.visible = ! reset_data.visible;
            spinbox_light_pos_x.visible = !spinbox_light_pos_x.visible;
            spinbox_light_pos_y.visible = !spinbox_light_pos_y.visible;
            spinbox_light_pos_z.visible = !spinbox_light_pos_z.visible;
            samples_L0.visible = !samples_L0.visible;
            samples_L1.visible = !samples_L1.visible;
            scale.visible = !scale.visible;
            bias.visible = !bias.visible;
            radius.visible = !radius.visible;
            intensity.visible = !intensity.visible;
            depth_tolerance.visible = !depth_tolerance.visible;
            ao_mode.visible = !ao_mode.visible;
            render_mode.visible = !render_mode.visible;
            ao_render.visible = !ao_render.visible;
            ao_render_v.visible = !ao_render_v.visible;
            ao_render_vh.visible = !ao_render_vh.visible;
            scene_depth.visible = !scene_depth.visible;
            scene_normal.visible = !scene_normal.visible;
            label.visible = !label.visible;
            labelFrame.visible = !labelFrame.visible;

       }
   }

    Button {
       id: reset_data
       x: 10
       y: 40
       width: 100
       height: 23
       style: buttonStyle
       text: "Restore Defaults"

       property bool current_flag : true;

       onClicked: {
           current_flag = !current_flag;
           dataManager.setFlag(DataManager.RESTORE_DEFAULTS, current_flag);
       }
   }

    SpinBox {
        x: 10
        y: 90
        id: spinbox_light_pos_x
        objectName: "light_position_x"
        decimals: 3
        opacity: 0.9
        maximumValue: 20
        minimumValue: -20
        stepSize: 0.1
        value: 0
        style: spinboxStyle
        onValueChanged:{
            dataManager.setData(DataManager.LIGHT_POS_X ,value);
        }
        Text {
            id: spinbox_light_pos_x_text
            anchors.bottom: spinbox_light_pos_x.top
            wrapMode: Text.WordWrap
            text: "light position x"
            renderType: Text.NativeRendering;
            color: "white"
        }
    }

    SpinBox {
        x: 10
        y: 125
        id: spinbox_light_pos_y
        objectName: "light_position_y"
        decimals: 3
        opacity: 0.9
        maximumValue: 20
        minimumValue: -20
        stepSize: 0.1
        value: 0
        style: spinboxStyle
        onValueChanged:{
            dataManager.setData(DataManager.LIGHT_POS_Y, value);
        }
        Text {
            id: spinbox_light_pos_y_text
            anchors.bottom: spinbox_light_pos_y.top
            wrapMode: Text.WordWrap
            text: "light position x"
            renderType: Text.NativeRendering;
            color: "white"
        }
    }

    SpinBox {
        x: 10
        y: 160
        id: spinbox_light_pos_z
        objectName: "light_position_z"
        decimals: 3
        opacity: 0.9
        maximumValue: 20
        minimumValue: -20
        stepSize: 0.1
        value: 0
        style: spinboxStyle
        onValueChanged:{
            dataManager.setData(DataManager.LIGHT_POS_Z, value);
        }
        Text {
            id: spinbox_light_pos_z_text
            anchors.bottom: spinbox_light_pos_z.top
            wrapMode: Text.WordWrap
            text: "light position z"
            renderType: Text.NativeRendering;
            color: "white"
        }
    }

    SpinBox {
        x: 10
        y: 195
        id: samples_L0
        objectName: "samples_L0"
        decimals: 0
        opacity: 0.9
        maximumValue: 32
        minimumValue: 0
        stepSize: 1
        value: 8
        style: spinboxStyle
        onValueChanged:{
            dataManager.setData(DataManager.SAMPLES_L0, value);
        }
        Text {
            id: samples_L0_text
            anchors.bottom: samples_L0.top
            wrapMode: Text.WordWrap
            text: "samples L0"
            renderType: Text.NativeRendering;
            color: "white"
        }
    }

    SpinBox {
        x: 10
        y: 230
        id: samples_L1
        objectName: "samples_L1"
        decimals: 0
        opacity: 0.9
        maximumValue: 32
        minimumValue: 0
        stepSize: 1
        value: 8
        style: spinboxStyle
        onValueChanged:{
            dataManager.setData(DataManager.SAMPLES_L1, value);
        }
        Text {
            id: samples_L1_text
            anchors.bottom: samples_L1.top
            wrapMode: Text.WordWrap
            text: "samples L1"
            renderType: Text.NativeRendering;
            color: "white"
        }
    }

    SpinBox {
        x: 10
        y: 265
        id: scale
        objectName: "scale"
        decimals: 3
        opacity: 0.9
        maximumValue: 10
        minimumValue: -10
        stepSize: 0.01
        value: 0
        style: spinboxStyle
        onValueChanged:{
            dataManager.setData(DataManager.SCALE, value);
        }
        Text {
            id: scale_text
            anchors.bottom: scale.top
            wrapMode: Text.WordWrap
            text: "scale"
            renderType: Text.NativeRendering;
            color: "white"
        }
    }

    SpinBox {
        x: 10
        y: 300
        id: bias
        objectName: "bias"
        decimals: 3
        opacity: 0.9
        maximumValue: 10
        minimumValue: -10
        stepSize: 0.01
        value: 0
        style: spinboxStyle
        onValueChanged:{
            dataManager.setData(DataManager.BIAS, value);
        }
        Text {
            id: bias_text
            anchors.bottom: bias.top
            wrapMode: Text.WordWrap
            text: "bias"
            renderType: Text.NativeRendering;
            color: "white"
        }
    }

    SpinBox {
        x: 10
        y: 335
        id: radius
        objectName: "radius"
        decimals: 3
        opacity: 0.9
        maximumValue: 10
        minimumValue: 0.01
        stepSize: 0.01
        value:0.20
        style: spinboxStyle
        onValueChanged:{
            dataManager.setData(DataManager.RADIUS, value);
        }
        Text {
            id: radius_text
            anchors.bottom: radius.top
            wrapMode: Text.WordWrap
            text: "radius"
            renderType: Text.NativeRendering;
            color: "white"
        }
    }

    SpinBox {
        x: 10
        y: 370
        id: intensity
        objectName: "intensity"
        decimals: 3
        opacity: 0.9
        maximumValue: 10
        minimumValue: 0.001
        stepSize: 0.001
        value: 1
        style: spinboxStyle
        onValueChanged:{
            dataManager.setData(DataManager.INTENSITY, value);
        }
        Text {
            id: intensity_text
            anchors.bottom: intensity.top
            wrapMode: Text.WordWrap
            text: "intensity"
            renderType: Text.NativeRendering;
            color: "white"
        }
    }

    SpinBox {
        x: 10
        y: 405
        id: depth_tolerance
        objectName: "depth_tolerance"
        decimals: 6
        opacity: 0.9
        maximumValue: 1
        minimumValue: 0.000001
        stepSize: 0.000001
        value: 0.00001
        style: spinboxStyle
        onValueChanged:{
            dataManager.setData(DataManager.DEPTH_TOLERANCE, value);
        }
        Text {
            id: depth_tolerance_text
            anchors.bottom: depth_tolerance.top
            wrapMode: Text.WordWrap
            text: "depth tolerance"
            renderType: Text.NativeRendering;
            color: "white"
        }
    }

    Button {
       id: ao_mode
       x: 10
       y: 440
       width: 100
       height: 23
       style: buttonStyle
       text: "SSAO Mode"

       property bool current_flag : true;

       onClicked: {
           current_flag = !current_flag;
           dataManager.setFlag(DataManager.SSAO_TOOGLE, current_flag);
       }
   }

    Button {
       id: render_mode
       x: 10
       y: 470
       width: 100
       height: 23
       style: buttonStyle
       text: "Render Mode"

       property bool current_flag : true;

       onClicked: {
           current_flag = !current_flag;
           dataManager.setFlag(DataManager.SCENE_RENDER_TOOGLE, current_flag);
       }
   }

    Button {
       id: ao_render
       x: 10
       y: 500
       width: 100
       height: 23
       style: buttonStyle
       text: "SSAO Texture"

       property bool current_flag : true;

       onClicked: {
           current_flag = !current_flag;
           dataManager.setFlag(DataManager.SSAO_RENDER_TOOGLE, current_flag);
       }
    }

    Button {
       id: ao_render_v
       x: 10
       y: 530
       width: 100
       height: 23
       style: buttonStyle
       text: "SSAO T.v-smooth"

       property bool current_flag : true;

       onClicked: {
           current_flag = !current_flag;
           dataManager.setFlag(DataManager.SSAO_VERTICAL_TOOGLE, current_flag);
       }
   }

    Button {
       id: ao_render_vh
       x: 10
       y: 560
       width: 100
       height: 23
       style: buttonStyle
       text: "SSAO T.vh-smooth"

       property bool current_flag : true;

       onClicked: {
           current_flag = !current_flag;
           dataManager.setFlag(DataManager.SSAO_HORIZONTAL_TOOGLE, current_flag);
       }
   }


    Button {
       id: scene_normal
       x: 10
       y: 590
       width: 100
       height: 23
       style: buttonStyle
       text: "Scene Normals"

       property bool current_flag : true;

       onClicked: {
           current_flag = !current_flag;
           dataManager.setFlag(DataManager.SCENE_NORMAL_TOOGLE, current_flag);
       }
   }

    Button {
       id: scene_depth
       x: 10
       y: 620
       width: 100
       height: 23
       style: buttonStyle
       text: "Scene Depth"

       property bool current_flag : true;

       onClicked: {
           current_flag = !current_flag;
           dataManager.setFlag(DataManager.SCENE_DEPTH_TOOGLE, current_flag);
       }
   }


    Rectangle {
        id: labelFrame
        anchors.margins: -10
        radius: 5
        color: "white"
        border.color: "black"
        opacity: 0.9
        anchors.fill: label
    }

    Text {
        id: label
        anchors.bottom: renderer.bottom
        anchors.left: renderer.left
        anchors.right: renderer.right
        anchors.margins: 20
        wrapMode: Text.WordWrap
        text: "Example shows Basic AmbientOcclusion technique that makes illusion of global illumination. Created based on OpenGL 4.3 graphics pileline. Use mouse-wheel or arrow-keys to adjust animation controling factors. Change the current rendering mode with buttons."
        renderType: Text.NativeRendering;
    }

    property Component spinboxStyle: SpinBoxStyle{
        background: Rectangle {
            id: rect
            implicitWidth: 100
            implicitHeight: 20
            border.color: "black"
            radius: 4
        }
        selectionColor: "transparent"
        textColor : "black"
        selectedTextColor: "black"
        renderType: Text.NativeRendering
    }

    property Component buttonStyle: ButtonStyle {
        background: Rectangle {
            implicitHeight: 22
            opacity: 0.9
            color: control.pressed ? "lightGray" : control.activeFocus ? "#cdd" : control.hovered ? "#ddd" : "white"
            antialiasing: true
            border.color: "black"
            radius: 4
            Rectangle {
                anchors.fill: parent
                anchors.margins: 1
                color: "transparent"
                antialiasing: true
                visible: !control.pressed
                border.color: "#aaffffff"
                radius: 4
            }
        }
    }
}
