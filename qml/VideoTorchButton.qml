// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0

Button {
        id: button
        width: 56
        height: 56
        opacity: 0.5

        iconSource: torch.on ? "image://theme/icon-m-camera-torch-on" : "image://theme/icon-m-camera-torch-off"
        onClicked: torch.on = !torch.on
        VideoTorch {
                id: torch
                camera: cam
        }
}
