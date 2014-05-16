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
BuildRequires:  pkgconfig(Qt5Location)
BuildRequires:  pkgconfig(Qt5OpenGL)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(libcanberra)
BuildRequires:  pkgconfig(Qt5Sparql)
BuildRequires:  pkgconfig(qmsystem2-qt5)
BuildRequires:  pkgconfig(libresourceqt5)
BuildRequires:  pkgconfig(Qt5SystemInfo)
BuildRequires:  pkgconfig(Qt5OpenGLExtensions)
BuildRequires:  pkgconfig(quill-qt5)
BuildRequires:  pkgconfig(contextkit-statefs)
BuildRequires:  desktop-file-utils
Requires:       qt5-qtdeclarative-import-location
Requires:       qt5-qtlocation-plugin-geoservices-osm

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

%changelog
