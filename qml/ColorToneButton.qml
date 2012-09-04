// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0


Selector {
        id: button

        property alias value: ct.value

        timerConstraints: true // Never trigger

        iconSource: ctIcon(ct.value);

        title: qsTr("Color filter");

        ColorTone {
                id: ct
                camera: cam
                // TODO: hardcoding
                value: ColorTone.Normal
        }

        function ctIcon(val) {
                var x = row.children.length;
                var i = 0;
                for (i = 0; i < x; i++) {
                        if (row.children[i].value == val) {
                                return row.children[i].normalIcon;
                        }
                }
        }

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
                        normalIcon: "image://theme/icon-m-camera-no-filter"
                        checkedIcon: "image://theme/icon-m-camera-no-filter-selected"
                        controller: ct
                        value: ColorTone.Normal
                        doClose: false
                }

                CheckButton {
                        normalIcon: "image://theme/icon-m-camera-filter-black-white"
                        checkedIcon: "image://theme/icon-m-camera-filter-black-white-selected"
                        controller: ct
                        value: ColorTone.GrayScale
                        doClose: false
                }

                CheckButton {
                        normalIcon: "image://theme/icon-m-camera-filter-sepia"
                        checkedIcon: "image://theme/icon-m-camera-filter-sepia-selected"
                        controller: ct
                        value: ColorTone.Sepia
                        doClose: false
                }

                CheckButton {
                        normalIcon: "image://theme/icon-m-camera-filter-vivid"
                        checkedIcon: "image://theme/icon-m-camera-filter-vivid-selected"
                        controller: ct
                        value: ColorTone.Vivid
                        doClose: false
                }

                CheckButton {
                        normalIcon: "image://theme/icon-m-camera-filter-negative"
                        checkedIcon: "image://theme/icon-m-camera-filter-negative-selected"
                        controller: ct
                        value: ColorTone.Negative
                        doClose: false
                }

                CheckButton {
                        normalIcon: "image://theme/icon-m-camera-filter-solarize"
                        checkedIcon: "image://theme/icon-m-camera-filter-solarize-selected"
                        controller: ct
                        value: ColorTone.Solarize
                        doClose: false
                }
        }
}
