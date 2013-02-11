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
import "ImageSettingsDialog.js" as Util

Column {
        spacing: 10

        SectionHeader {
                text: qsTr("Aspect ratio");
        }

        ButtonRow {
                id: aspectRatioRow
                width: parent.width

                exclusive: false
                onCheckedButtonChanged: {
                        // This is needed to initially setup the
                        // resolutions buttons
                        Util.resetAspectRatio(aspectRatioRow);
                }

                Repeater {
                        model: imageSettings.aspectRatios
                        delegate: Button {
                                property string aspect: modelData;
                                text: qsTr(modelData);
                                checked: settings.imageAspectRatio == modelData;
                                onClicked: Util.setAspectRatio(modelData);
                        }
                }
        }

        SectionHeader {
                text: qsTr("Resolution");
        }

        ButtonRow {
                id: resolutionsRow
                width: parent.width

                exclusive: false

                Repeater {
                        id: resolutions
                        model: imageSettings.resolutions

                        delegate: Button {
                                property string resolution: resolutionName
                                property string aspectRatio: resolutionAspectRatio
                                text: qsTr("%1 Mpx").arg(megaPixels);
                                checked: settings.imageResolution == resolutionName
                                onClicked: Util.setResolution(resolutionName);
                        }
                }
        }
}
