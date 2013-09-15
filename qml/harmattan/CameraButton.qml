// -*- qml -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2013 Mohammed Sameer <msameer@foolab.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

import QtQuick 1.1

Item {
    id: button
    property bool capitalize
    width: platformStyle.buttonWidth
    height: platformStyle.buttonHeight

    signal clicked
    property bool checked
    property bool checkable
    property alias text: label.text
    property alias pressed: mouse.pressed
    property alias enabled: mouse.enabled
    property CameraButtonStyle platformStyle: CameraButtonStyle {}

    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: parent.clicked()
    }

    BorderImage {
        id: background
        anchors.fill: parent
        border { left: button.platformStyle.backgroundMarginLeft; top: button.platformStyle.backgroundMarginTop;
                 right: button.platformStyle.backgroundMarginRight; bottom: button.platformStyle.backgroundMarginBottom }

        source: !enabled ?
                  (checked ? button.platformStyle.checkedDisabledBackground : button.platformStyle.disabledBackground) :
                  pressed ?
                      button.platformStyle.pressedBackground :
                  checked ?
                      button.platformStyle.checkedBackground :
                      button.platformStyle.background;
    }

    CameraLabel {
        id: label
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        horizontalAlignment: Text.AlignHCenter
        font.bold: true
        font.capitalization: parent.capitalize ? Font.Capitalize : Font.MixedCase
        color: !enabled ? button.platformStyle.disabledTextColor :
               pressed ? button.platformStyle.pressedTextColor :
               checked ? button.platformStyle.checkedTextColor :
                         button.platformStyle.textColor;
        elide: Text.ElideRight
    }
}
