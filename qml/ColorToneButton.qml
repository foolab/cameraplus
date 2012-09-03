// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.1
import QtCamera 1.0


Selector {
        id: button

        property alias value: ct.value

        timerConstraints: true // Never trigger

        iconSource: ctIcon(ct.value);

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
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-no-filter.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-no-filter-selected.png"
                        controller: ct
                        value: ColorTone.Normal
                }

                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-filter-black-white.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-filter-black-white-selected.png"
                        controller: ct
                        value: ColorTone.GrayScale
                }

                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-filter-sepia.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-filter-sepia-selected.png"
                        controller: ct
                        value: ColorTone.Sepia
                }

                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-filter-vivid.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-filter-vivid-selected.png"
                        controller: ct
                        value: ColorTone.Vivid
                }

                CheckButton {
                        normalIcon: "/usr/share/themes//blanco/meegotouch/icons/icon-m-camera-filter-negative.png"
                        checkedIcon: "/usr/share/themes//blanco/meegotouch/icons/icon-m-camera-filter-negative-selected.png"
                        controller: ct
                        value: ColorTone.Negative
                }

                CheckButton {
                        normalIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-filter-solarize.png"
                        checkedIcon: "/usr/share/themes/blanco/meegotouch/icons/icon-m-camera-filter-solarize-selected.png"
                        controller: ct
                        value: ColorTone.Solarize
                }
        }
}
