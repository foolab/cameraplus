// -*- qml -*-

/*!
 * This file is part of CameraPlus.
 *
 * Copyright (C) 2012-2014 Mohammed Sameer <msameer@foolab.org>
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

import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.TransferEngine 1.0

Item {
    id: share

    property variant settings
    anchors.fill: parent

    function shareUrl(url, mime, file) {
        if (url == "" || mime == "" || file == "") {
            return false
        }

        pageStack.push(sharePage, {"fileName": file, "mime": mime, "url": url})
        return true
    }

    Component {
        id: sharePage

        CameraPage {
            id: page
            property string mime
            property url url
            property string fileName

            ShareMethodList {
                anchors.fill: parent
                filter: mime
                source: url
                header: PageHeader {
                    title: qsTr("Share %1").arg(fileName)
                }
            }
        }
    }
}
