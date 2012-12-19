// -*- qml -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012 Mohammed Sameer <msameer@foolab.org>
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
import com.nokia.meego 1.1
import QtCamera 1.0

Selector {
        id: button

        iconSource: settings.videoEvComp == 0 ? "image://theme/icon-m-camera-manual-exposure" : ""
        text: settings.videoEvComp == 0 ? "" : settings.videoEvComp.toFixed(1);
        font.pixelSize: 19
        timerConstraints: slider.pressed

        title: qsTr("Exposure compensation");

        widget: Slider {
                id: slider
                width: 500
                orientation: Qt.Horizontal
                minimumValue: cam.evComp.minimum
                maximumValue: cam.evComp.maximum
                value: settings.videoEvComp
                stepSize: 0.1
                onValueChanged: settings.videoEvComp = value.toFixed(1);
                Component.onCompleted: { slider.value = settings.videoEvComp.toFixed(1); }
        }
}
