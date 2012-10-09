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
import "data.js" as Data

Selector {
        id: button

        property int value: settings.imageSceneMode

        iconSource: "image://theme/" + Data.ismIcon(scene.value);

        title: qsTr("Scene mode");

        widget: Row {
                id: row
                height: button.checked ? 64 : 0
                width: button.checked ? (children.length * height) +  (children.length - 1) * spacing : 0
                spacing: 10

                Behavior on width {
                        // TODO: seems animation is not working
                        PropertyAnimation { duration: 250; }
                }

                CheckButton {
                        normalIcon: "image://theme/" + Data.ismIcon(value);
                        checkedIcon: "image://theme/" + Data.ismSelectedIcon(value);
                        savedValue: settings.imageSceneMode
                        onClicked: settings.imageSceneMode = value;
                        value: Scene.Auto
                }

                CheckButton {
                        normalIcon: "image://theme/" + Data.ismIcon(value);
                        checkedIcon: "image://theme/" + Data.ismSelectedIcon(value);
                        savedValue: settings.imageSceneMode
                        onClicked: settings.imageSceneMode = value;
                        value: Scene.Closeup
                }

                CheckButton {
                        normalIcon: "image://theme/" + Data.ismIcon(value);
                        checkedIcon: "image://theme/" + Data.ismSelectedIcon(value);
                        savedValue: settings.imageSceneMode
                        onClicked: settings.imageSceneMode = value;
                        value: Scene.Landscape
                }

                CheckButton {
                        normalIcon: "image://theme/" + Data.ismIcon(value);
                        checkedIcon: "image://theme/" + Data.ismSelectedIcon(value);
                        savedValue: settings.imageSceneMode
                        onClicked: settings.imageSceneMode = value;
                        value: Scene.Portrait
                }

                CheckButton {
                        normalIcon: "image://theme/" + Data.ismIcon(value);
                        checkedIcon: "image://theme/" + Data.ismSelectedIcon(value);
                        savedValue: settings.imageSceneMode
                        onClicked: settings.imageSceneMode = value;
                        value: Scene.Night
                }

                CheckButton {
                        normalIcon: "image://theme/" + Data.ismIcon(value);
                        checkedIcon: "image://theme/" + Data.ismSelectedIcon(value);
                        savedValue: settings.imageSceneMode
                        onClicked: settings.imageSceneMode = value;
                        value: Scene.Sport
                }
        }
}
