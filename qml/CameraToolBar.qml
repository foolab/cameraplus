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
import com.nokia.meego 1.1
import "CameraToolBar.js" as Layout

Rectangle {
        id: tools
        property bool expanded: false
        property list<Item> items
        property int targetWidth: parent.width - (2 * anchors.leftMargin)
        property alias menuWidth: menu.width
        property bool manualBack: false
        signal clicked

        height: menu.height
        width: expanded ? targetWidth : menu.width
        color: expanded ? "black" : width == menu.width ? "transparent" : "black"
        border.color: expanded ? "gray" : width == menu.width ? "transparent" : "gray"
        radius: 20

        Behavior on width {
                PropertyAnimation { duration: 100; }
        }

        ToolIcon {
                property bool __isMenu: true
                id: menu
                anchors.verticalCenter: parent.verticalCenter
                iconSource: "image://theme/icon-m-toolbar-back-white"
                onClicked: {
                        if (tools.manualBack) {
                                tools.clicked();
                                return;
                        }

                        if (!expanded) {
                                expanded = true;
                        }
                        else if (Layout.stack.length == 1) {
                                expanded = false;
                        }
                        else {
                                Layout.pop();
                        }
                }

                anchors.left: parent.left
                rotation: 180
        }

        onExpandedChanged: {
                if (tools.expanded) {
                        tools.push(tools.items);
                }
                else {
                        tools.pop();
                }
        }

        onWidthChanged: Layout.layout();
        onTargetWidthChanged: Layout.layout();

        function push(items) {
                return Layout.push(items);
        }

        function pop() {
                return Layout.pop();
        }

        state: "collapsed"
        states: [
        State {
                name: "expanded"
                when: tools.expanded
        },
        State {
                name: "collapsed"
                when: !tools.expanded
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
                        duration: 500
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
                        duration: 500
                }
        }
        ]
}
