%define __provides_exclude_from ^%{_datadir}/.*$
%define __requires_exclude ^libqtcamera.so.1|libavcodec.so.56|libavformat.so.56|libavutil.so.54|libavdevice.so.56|libavfilter.so.5|libpostproc.so.53|libswresample.so.1|libswscale.so.3|libffmpegthumbnailer.so.4|libquill-qt5.so.1$

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
BuildRequires:  pkgconfig(Qt5Test)
BuildRequires:  pkgconfig(libresourceqt5)
BuildRequires:  pkgconfig(Qt5SystemInfo)
BuildRequires:  pkgconfig(contextkit-statefs)
BuildRequires:  pkgconfig(sndfile)
BuildRequires:  pkgconfig(libpulse)
BuildRequires:  pkgconfig(exempi-2.0)
BuildRequires:  desktop-file-utils
BuildRequires:  cmake
BuildRequires:  libjpeg-devel
BuildRequires:  pkgconfig(quillmetadata-qt5)
BuildRequires:  pkgconfig(quillimagefilter-qt5)
BuildRequires:  pkgconfig(libexif)
Requires:       qt5-qtdeclarative-import-positioning
Requires:       qt5-qtdeclarative-import-sensors

%description
Cameraplus is an advanced easy to use camera

%package tools
Summary: Tools for cameraplus
Group: Applications/Multimedia
Requires:  harbour-cameraplus = %{version}-%{release}

%description tools
%{summary}

%prep
%setup -q

%build
cmake  -DPLATFORM=sailfish -DDATA_DIR=/usr/share/harbour-cameraplus/share/qtcamera/config/ -DCMAKE_INSTALL_PREFIX=/usr/share/harbour-cameraplus/

make %{?jobs:-j%jobs} VERBOSE=1

%install
%make_install

rm $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/libqtcamera.so

mv $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/bin/ $RPM_BUILD_ROOT/%{_bindir}

mkdir -p $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/
mv $RPM_BUILD_ROOT/%{_libdir}/* $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/

# icon
mkdir -p $RPM_BUILD_ROOT/usr/share/icons/hicolor/86x86/apps/
cp %SOURCE2 $RPM_BUILD_ROOT/usr/share/icons/hicolor/86x86/apps/

# qtcamera configuration
mkdir -p $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/
mkdir -p $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/jp-1301/
mkdir -p $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/jt-1501/
mkdir -p $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/hammerhead/
mkdir -p $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/bacon/
mkdir -p $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/find5/
cp data/sailfish/device.ini $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/
cp data/sailfish/qtcamera.ini $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/
cp data/sailfish/jp-1301/device.ini $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/jp-1301/
cp data/sailfish/jt-1501/device.ini $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/jt-1501/
cp data/sailfish/hammerhead/device.ini $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/hammerhead/
cp data/sailfish/video.gep $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/
cp data/sailfish/image.gep $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/
cp data/sailfish/properties.ini $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/
cp data/sailfish/qtcamera.ini $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/qtcamera/config/

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

mv $RPM_BUILD_ROOT/%{_bindir}/cameraplus $RPM_BUILD_ROOT/%{_bindir}/harbour-cameraplus

mkdir -p $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/sounds/
cp sounds/*.wav $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/sounds/

mkdir -p $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/modes/
cp modes/image.ini modes/image-timer.ini modes/image-sequential.ini modes/image-panorama.ini modes/video.ini $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/share/modes/

# dependencies
cp -a deps/usr/lib/lib*.so.* $RPM_BUILD_ROOT/usr/share/harbour-cameraplus/lib/

%files
%defattr(-,root,root,-)
%{_bindir}/harbour-cameraplus
%{_datadir}/harbour-cameraplus/lib/libqtcamera.so.1
%{_datadir}/harbour-cameraplus/share/cameraplus/*
%{_datadir}/harbour-cameraplus/share/qtcamera/*
%{_datadir}/harbour-cameraplus/share/sounds/*
%{_datadir}/harbour-cameraplus/share/modes/*
%{_datadir}/harbour-cameraplus/lib/qt5/qml/QtCamera/*
%{_datadir}/harbour-cameraplus/lib/qt5/qml/CameraPlus/Panorama/*
%{_datadir}/applications/harbour-cameraplus.desktop
%{_datadir}/icons/hicolor/86x86/apps/harbour-cameraplus.png
# dependencies
%{_datadir}/harbour-cameraplus/lib/*

%files tools
%defattr(-,root,root,-)
%{_bindir}/dump_resolutions

