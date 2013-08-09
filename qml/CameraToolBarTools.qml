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

import QtQuick 2.0
import "CameraToolBar.js" as Layout

Row {
    visible: false
    anchors.fill: parent
    layoutDirection: Qt.LeftToRight
    // We are explicitly setting a width here to prevent the item from dynamically
    // calculating it based on spacing.
    // If we don't do so then we get a binding loop problem between spacing and width
    // the value doesn't affect anything else as CameraToolBar will fix it before it shows us
    width: 0
    property real childrenWidth: Layout.calculateChildrenWidth(children)
    property int childrenLen: Layout.countVisibleChildren(children)
    spacing: (width - childrenWidth) / (childrenLen - 1)

    opacity: visible ? 1.0 : 0

    Behavior on opacity {
        NumberAnimation { duration: 100 }
    }
}
