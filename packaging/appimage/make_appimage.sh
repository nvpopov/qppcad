#!/bin/bash

#download appimage tools
wget -c https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage
chmod +x ./appimagetool-x86_64.AppImage

wget -c https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage
chmod +x ./linuxdeployqt-continuous-x86_64.AppImage

wget -c https://github.com/probonopd/uploadtool/raw/master/upload.sh
chmod +x ./upload.sh

#create appimage dir structure
mkdir qppcad
mkdir qppcad/usr
mkdir qppcad/usr/bin
mkdir qppcad/usr/lib
cp ../../bin/qppcad qppcad/usr/bin/
chmod +x qppcad/usr/bin/qppcad

cp ../../data/images/icon.svg qppcad/icon.svg
cp ./qppcad.desktop qppcad/qppcad.desktop

./linuxdeployqt-continuous-x86_64.AppImage qppcad/qppcad.desktop -appimage -bundle-non-qt-libs -extra-plugins=iconengines/libqsvgicon.so

rm qppcad/AppRun
cp AppRun.in qppcad/AppRun
chmod +x qppcad/AppRun
rm -rf qppcad/include
rm -rf qppcad/lib/python*/test
rm -rf qppcad/bin/idle*

./appimagetool-x86_64.AppImage qppcad








