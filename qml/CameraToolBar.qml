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
import CameraPlus 1.0
import "CameraToolBar.js" as Layout

Rectangle {
    id: toolBar

    property bool expanded: true
    property real targetWidth: parent.width - anchors.leftMargin - anchors.rightMargin
    property bool manualBack
    property bool hideBack
    signal clicked

    property CameraToolBarTools tools
    property CameraToolBarTools __currentTools

    function push(tools) {
        if (expanded) {
            __currentTools = Layout.pushAndShow(tools, dock, stack)
        }
        else {
            __currentTools = Layout.push(tools, dock, stack)
        }
    }

    function pop() {
        __currentTools = Layout.pop(dock, stack);
    }

    function depth() {
        return stack.length
    }

    onToolsChanged: {
        push(tools)
    }

    onExpandedChanged: {
        if (stack.length == 0) {
            return
        }

        if (expanded) {
            Layout.showLast(dock, stack)
        }
        else {
            Layout.hideLast(dock, stack)
        }
    }

    Component.onDestruction: Layout.clear(dock, stack)

    width: expanded ? targetWidth : menu.width
    height: menu.height
    color: "black"
    border.color: "gray"
    radius: 20

    Behavior on width {
        PropertyAnimation { duration: 100 }
    }

    CameraToolIcon {
        visible: !parent.hideBack
        id: menu
        rotation: parent.expanded ? 360 : 180
        iconSource: cameraTheme.cameraToolBarMenuIcon
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        Behavior on rotation {
            NumberAnimation { duration: 250 }
        }

        onClicked: {
            if (parent.manualBack) {
                parent.clicked()
            } else if (!parent.expanded) {
                parent.expanded = true
            } else if (stack.length == 1) {
                expanded = false
            } else {
                __currentTools = Layout.pop(dock, stack)
            }
        }
    }

    Stack {
        id: stack
    }

    Item {
        id: dock
        property real menuWidth: parent.hideBack ? 0 : menu.width
        property real leftMargin: __currentTools ? (parent.width - __currentTools.childrenWidth - menuWidth) / __currentTools.childrenLen : 0
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.hideBack ? parent.left : menu.right
        anchors.leftMargin: parent.hideBack ? 0 : leftMargin
    }
}
