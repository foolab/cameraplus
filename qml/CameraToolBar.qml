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

Rectangle {
    id: toolBar

    property bool expanded: true
    property real targetWidth: parent.width - anchors.leftMargin - anchors.rightMargin
    property bool manualBack: false
    property bool hideBack: false
    signal clicked

    property CameraToolBarTools tools
    property CameraToolBarTools __currentTools

    function push(tools) {
        if (expanded) {
            __currentTools = Layout.pushAndShow(tools)
        }
        else {
            __currentTools = Layout.push(tools)
        }
    }

    function pop() {
        __currentTools = Layout.pop();
    }

    function depth() {
        return Layout.depth()
    }

    onToolsChanged: {
        push(tools)
    }

    onExpandedChanged: {
        if (Layout.isEmpty()) {
            return
        }

        if (expanded) {
            Layout.showLast()
        }
        else {
            Layout.hideLast()
        }
    }

    Component.onDestruction: Layout.clear()

    width: expanded ? targetWidth : menu.width
    height: menu.height
    color: "black"
    border.color: "gray"
    radius: 20

    states: [
        State {
            name: "expanded"
            when: expanded
        },
        State {
            name: "collapsed"
            when: !expanded
        }
    ]

    transitions: [
        Transition {
            from: "expanded"
            to: "collapsed"

            PropertyAnimation {
                property: "rotation"
                target: menu
                from: 0
                to: 180
                duration: 250
            }
        },
        Transition {
            from: "collapsed"
            to: "expanded"
            PropertyAnimation {
                property: "rotation"
                target: menu
                from: 180
                to: 360
                duration: 250
            }
        }
    ]

    Behavior on width {
        PropertyAnimation { duration: 100 }
    }

    CameraToolIcon {
        visible: !parent.hideBack
        id: menu
        iconSource: cameraTheme.cameraToolBarMenuIcon
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        onClicked: {
            if (parent.manualBack) {
                parent.clicked()
            } else if (!parent.expanded) {
                parent.expanded = true
            } else if (Layout.stack.length == 1) {
                expanded = false
            } else {
                __currentTools = Layout.pop()
            }
        }
    }

    Rectangle {
        id: dock
        property real menuWidth: parent.hideBack ? 0 : menu.width
        property real leftMargin: (parent.width - __currentTools.childrenWidth - menuWidth) / __currentTools.childrenLen
        color: "transparent"
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.hideBack ? parent.left : menu.right
        anchors.leftMargin: parent.hideBack ? 0 : leftMargin
    }

    Component {
        id: toolsContainer
        Item {
            property Item tools
            property Item owner
        }
    }
}
