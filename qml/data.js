// -*- js -*-

// Shared between all QML components
//.pragma library

var __wb = [
    [WhiteBalance.Auto, qsTr("Automatic"),
     "icon-m-camera-whitebalance-auto-selected", "icon-m-camera-whitebalance-auto"],
    [WhiteBalance.Sunset, qsTr("Sunny"),
     "icon-m-camera-whitebalance-sunny-selected", "icon-m-camera-whitebalance-sunny"],
    [WhiteBalance.Cloudy, qsTr("Cloudy"),
     "icon-m-camera-whitebalance-cloudy-selected", "icon-m-camera-whitebalance-cloudy"],
    [WhiteBalance.Flourescent, qsTr("Flourescent"),
     "icon-m-camera-whitebalance-fluorescent-selected", "icon-m-camera-whitebalance-fluorescent"],
    [WhiteBalance.Tungsten, qsTr("Tungsten"),
     "icon-m-camera-whitebalance-tungsten-selected", "icon-m-camera-whitebalance-tungsten"],
];

var __cf = [
    [ColorTone.Normal, qsTr("Off"),
     "icon-m-camera-no-filter-selected", "icon-m-camera-no-filter"],
    [ColorTone.GrayScale, qsTr("Black & white"),
     "icon-m-camera-filter-black-white-selected", "icon-m-camera-filter-black-white"],
    [ColorTone.Sepia, qsTr("Sepia"),
     "icon-m-camera-filter-sepia-selected", "icon-m-camera-filter-sepia"],
    [ColorTone.Vivid, qsTr("Vivid"),
     "icon-m-camera-filter-vivid-selected", "icon-m-camera-filter-vivid"],
    [ColorTone.Negative, qsTr("Negative"),
     "icon-m-camera-filter-negative-selected", "icon-m-camera-filter-negative"],
    [ColorTone.Solarize, qsTr("Solarize"),
     "icon-m-camera-filter-solarize-selected", "icon-m-camera-filter-solarize"]
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

function wbIcon(wb) {
    return filterData(wb, __wb, 3);
}

function wbName(wb) {
    return filterData(wb, __wb, 1);
}

function cfSelectedIcon(cf) {
    return filterData(cf, __cf, 2);
}

function cfIcon(cf) {
    return filterData(cf, __cf, 3);
}

function cfName(cf) {
    return filterData(cf, __cf, 1);
}
