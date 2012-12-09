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
import CameraPlus 1.0

Item {
        id: item
        property bool isImage: mimetype.toLowerCase().search("video") < 0
        property bool error: false
        property bool currentItem: PathView.isCurrentItem
        onCurrentItemChanged: {
                if (currentItem) {
                        page.currentItem = item
                }
        }

        function stop() {
                if (loader.item) {
                        loader.item.stop();
                }
        }

        Label {
                anchors.fill: parent
                visible: (loader.status == Loader.Error || item.error) && page.status == PageStatus.Active
                text: qsTr("Failed to load preview");
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 32
        }

        Loader {
                id: loader
                anchors.fill: parent
                source: isImage ? Qt.resolvedUrl("PostCaptureImage.qml") : Qt.resolvedUrl("PostCaptureVideo.qml")
        }

        Binding {
                target: loader.item
                value: page.status == PageStatus.Activating || page.status == PageStatus.Active
                property: "visible"
        }

        Binding {
                target: item
                value: loader.item ? loader.item.error : false
                property: "error"
        }
}
