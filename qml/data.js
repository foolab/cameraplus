// -*- js -*-

// Shared between all QML components
//.pragma library

var __wb = [
    [WhiteBalance.Auto, qsTr("Automatic"),
     "icon-m-camera-whitebalance-auto-selected"],
    [WhiteBalance.Sunset, qsTr("Sunny"),
     "icon-m-camera-whitebalance-sunny-selected"],
    [WhiteBalance.Cloudy, qsTr("Cloudy"),
     "icon-m-camera-whitebalance-cloudy-selected"],
    [WhiteBalance.Flourescent, qsTr("Flourescent"),
     "icon-m-camera-whitebalance-fluorescent-selected"],
    [WhiteBalance.Tungsten, qsTr("Tungsten"),
     "icon-m-camera-whitebalance-tungsten-selected"],
];

var __cf = [
    [ColorTone.Normal, qsTr("Off"),
     "icon-m-camera-no-filter-selected"],
    [ColorTone.GrayScale, qsTr("Black & white"),
     "icon-m-camera-filter-black-white-selected"],
    [ColorTone.Sepia, qsTr("Sepia"),
     "icon-m-camera-filter-sepia-selected"],
    [ColorTone.Vivid, qsTr("Vivid"),
     "icon-m-camera-filter-vivid-selected"],
    [ColorTone.Negative, qsTr("Negative"),
     "icon-m-camera-filter-negative-selected"],
    [ColorTone.Solarize, qsTr("Solarize"),
     "icon-m-camera-filter-solarize-selected"]
];

function filterData(val, data, item) {
    var x = 0;
    var i = data.length;
    for (x = 0; x < i; x++) {
        if (data[x][0] == val) {
            return data[x][item];
        }
    }
}

function wbSelectedIcon(wb) {
    return filterData(wb, __wb, 2);
}

function wbName(wb) {
    return filterData(wb, __wb, 1);
}

function cfSelectedIcon(cf) {
    return filterData(cf, __cf, 2);
}

function cfName(cf) {
    return filterData(cf, __cf, 1);
}
