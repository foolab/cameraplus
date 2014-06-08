%define __provides_exclude_from ^%{_datadir}/.*$
%define __requires_exclude ^libqtcamera.so.1|ld-linux-armhf.so.3|libFLAC.so.8|libavcodec.so.54|libavformat.so.54|libavutil.so.52|libbz2.so.1|libc.so.6|libdbus-qeventloop-qt5.so.1|libdl.so.2|libffmpegthumbnailer.so.4|libgstapp-1.0.so.0|libgstaudio-1.0.so.0|libgstbase-1.0.so.0|libgstbasecamerabinsrc-1.0.so.0|libgstcodecparsers-1.0.so.0|libgstfft-1.0.so.0|libgstnemointerfaces-1.0.so.0|libgstnemometa-1.0.so.0|libgstnet-1.0.so.0|libgstpbutils-1.0.so.0|libgstphotography-1.0.so.0|libgstreamer-1.0.so.0|libgstriff-1.0.so.0|libgstrtp-1.0.so.0|libgstrtsp-1.0.so.0|libgstsdp-1.0.so.0|libgsttag-1.0.so.0|libgstvideo-1.0.so.0|libjpeg.so.62|libogg.so.0|libopus.so.0|liborc-0.4.so.0|liborc-test-0.4.so.0|libquill-qt5.so.1|libquillimagefilter-qt5.so.1|libquillmetadata-qt5.so.1|libresourceqt5.so.1|libsndfile.so.1|libspeex.so.1|libswscale.so.2|libtheoradec.so.1|libtheoraenc.so.1|libvo-aacenc.so.0|libvorbis.so.0|libvorbisenc.so.2|libresource.so.0|libm.so.6|libgstdroidmemory-1.0.so.0|libm.so.6(GLIBC_2.15)$

Name:           harbour-cameraplus
Summary:        Cameraplus is an advanced easy to use camera
Version:        0
Release:        1
Group:          Applications/Multimedia
License:        LGPL v2.1+
URL:            http://gitorious.org/cameraplus
Source0:        %{name}-%{version}.tar.gz
Source1:        harbour-cameraplus.desktop
Source2:        harbour-cameraplus.png
Source3:        qmake.conf
Source10:       binaries.tgz
Source11:       binaries-droid.tgz
BuildRequires:  pkgconfig(gstreamer-1.0)
BuildRequires:  pkgconfig(gstreamer-plugins-base-1.0)
BuildRequires:  pkgconfig(gstreamer-video-1.0)
BuildRequires:  pkgconfig(gstreamer-plugins-bad-1.0)
BuildRequires:  pkgconfig(gstreamer-pbutils-1.0)
BuildRequires:  pkgconfig(nemo-gstreamer-interfaces-1.0)
BuildRequires:  pkgconfig(nemo-gstreamer-meta-1.0)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(qdeclarative5-boostable)
BuildRequires:  pkgconfig(Qt5Gui)
BuildRequires:  pkgconfig(Qt5Network)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Sparql)
BuildRequires:  pkgconfig(Qt5Sensors)
BuildRequires:  pkgconfig(libresourceqt5)
BuildRequires:  pkgconfig(Qt5SystemInfo)
BuildRequires:  pkgconfig(quill-qt5)
BuildRequires:  pkgconfig(contextkit-statefs)
BuildRequires:  pkgconfig(sndfile)
BuildRequires:  pkgconfig(libpulse)
BuildRequires:  desktop-file-utils
Requires:       qt5-qtdeclarative-import-positioning
Requires:       qt5-qtdeclarative-import-sensors
#Requires:       gstreamer1.0-plugins-good
%description
Cameraplus is an advanced easy to use camera

%prep
%setup -q

%build
cp %SOURCE3 .qmake.conf
%qmake5

make %{?jobs:-j%jobs}

%install
%qmake5_install

# icon
mkdir -p $RPM_BUILD_ROOT/usr/share/icons/hicolor/86x86/apps/
cp %SOURCE2 $RPM_BUILD_ROOT/usr/share/icons/hicolor/86x86/apps/

# qtcamera configuration
mkdir -p $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/
cp data/sailfish/qtcamera.ini $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/
cp data/sailfish/resolutions.ini $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/
cp data/sailfish/video.gep $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/
cp data/sailfish/image.gep $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/
cp data/sailfish/properties.ini $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/

# cameraplus configuration
mkdir -p $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/cameraplus/config/
cp data/sailfish/cameraplus.ini $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/cameraplus/config/
cp data/sailfish/features.ini $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/cameraplus/config/

# desktop file
mkdir -p $RPM_BUILD_ROOT/usr/share/applications/
cp %SOURCE1 $RPM_BUILD_ROOT/usr/share/applications/

desktop-file-install --delete-original                   \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

mv $RPM_BUILD_ROOT/usr/bin/cameraplus $RPM_BUILD_ROOT/usr/bin/harbour-cameraplus

mkdir -p $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/
mv $RPM_BUILD_ROOT/usr/lib/libqtcamera.so.1.0.0 $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/libqtcamera.so.1
rm $RPM_BUILD_ROOT/usr/lib/libqtcamera.so.1.0
rm $RPM_BUILD_ROOT/usr/lib/libqtcamera.so.1
rm $RPM_BUILD_ROOT/usr/lib/libqtcamera.so

mkdir -p $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/qt5/qml
mv $RPM_BUILD_ROOT/usr/lib/qt5/imports/QtCamera $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/qt5/qml/
rm -rf $RPM_BUILD_ROOT/usr/lib/qt5/imports

mkdir -p $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/sounds/
cp sounds/*.wav $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/sounds/

mkdir -p $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/modes/
cp modes/*.ini $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/modes/

tar -zpxvf %SOURCE10 -C $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/
tar -zpxvf %SOURCE11 -C $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/

# for now we remove libgstvideoparsersbad.so
rm -rf $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/gstreamer-1.0/libgstvideoparsersbad.so

cp /usr/lib/libsndfile.so.1 $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/
cp /lib/libdl.so.2 $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/
cp /lib/libc.so.6 $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/
cp /usr/lib/libjpeg.so.62 $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/
cp /usr/lib/libbz2.so.1 $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/
cp /usr/lib/libtheoraenc.so.1 $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/
cp /usr/lib/libtheoradec.so.1 $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/
cp /usr/lib/libogg.so.0 $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/
cp /usr/lib/libvorbisenc.so.2 $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/
cp /usr/lib/libvorbis.so.0 $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/
#cp /usr/lib/libsoup-2.4.so.1 $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/
cp /usr/lib/libspeex.so.1 $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/
cp /usr/lib/libFLAC.so.8 $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/
cp /usr/lib/libquillmetadata-qt5.so.1 $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/
cp /usr/lib/libquillimagefilter-qt5.so.1 $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/
cp /usr/lib/libresourceqt5.so.1 $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/
cp /usr/lib/libdbus-qeventloop-qt5.so.1 $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/
cp /usr/lib/libresource.so.0 $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/
cp /lib/libm.so.6 $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/

%files
%defattr(-,root,root,-)
%{_bindir}/harbour-cameraplus
%{_datadir}/harbour-cameraplus/lib/libqtcamera.so.1
%{_datadir}/harbour-cameraplus/share/cameraplus/*
%{_datadir}/harbour-cameraplus/share/qtcamera/*
%{_datadir}/harbour-cameraplus/share/sounds/*
%{_datadir}/harbour-cameraplus/share/modes/*
%{_datadir}/harbour-cameraplus/lib/qt5/qml/QtCamera/*
%{_datadir}/applications/harbour-cameraplus.desktop
%{_datadir}/icons/hicolor/86x86/apps/harbour-cameraplus.png
# dependencies
%{_datadir}/harbour-cameraplus/share/*
%{_datadir}/harbour-cameraplus/lib/*
%{_datadir}/harbour-cameraplus/libexec/*
%{_datadir}/harbour-cameraplus/etc/*

%changelog
* Thu May 29 2014 Mohammed Sameer <msameer@foolab.org> 0.0.24-1
- GPS and face detection are the only visible indicators
- Move the resolution label back beside the indicators.
- Added left handed mode
- Take into account sensor orientation when rendering the viewfinder
- Make sure we render the viewfinder correctly while zooming
- Set proper exif orientation data to captured images
- Fixed tapping the glowing dot in the resolutions settings page not switching the resolution

* Thu May 29 2014 Mohammed Sameer <msameer@foolab.org> 0.0.23-1
- Initial RPM release
