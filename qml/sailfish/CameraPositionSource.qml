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
import QtPositioning 5.0

PositionSource {
    // NOTE: The source will not reset the position when we lose the signal.
    // This shouldn't be a big problem as we are course enough.
    // If we ever need street level updates then this will be an issue.
    // And we monitor horizontalAccuracyValid anyway
    id: positionSource

    updateInterval: 60000 // Once every 60 seconds should be enough.

    property real longitude: position.coordinate.longitude
    property real latitude: position.coordinate.latitude
    property real altitude: position.coordinate.altitude
    property real horizontalAccuracy: position.horizontalAccuracy
    property bool longitudeValid: position.longitudeValid
    property bool latitudeValid: position.latitudeValid
    property bool altitudeValid: position.altitudeValid
    property bool horizontalAccuracyValid: position.horizontalAccuracyValid
}
