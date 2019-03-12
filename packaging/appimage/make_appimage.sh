#!/bin/bash

#download appimage tools

wget -c https://raw.githubusercontent.com/TheAssassin/linuxdeploy-plugin-conda/master/linuxdeploy-plugin-conda.sh
chmod +x ./linuxdeploy-plugin-conda.sh

wget -c https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage
chmod +x ./appimagetool-x86_64.AppImage

wget -c https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
chmod +x ./linuxdeploy-x86_64.AppImage

wget -c https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
chmod +x ./linuxdeploy-plugin-qt-x86_64.AppImage

wget -c https://github.com/probonopd/uploadtool/raw/master/upload.sh
chmod +x ./upload.sh

#create appimage dir structure
mkdir qppcad
mkdir qppcad/usr
mkdir qppcad/usr/bin
cp ../../bin/qppcad qppcad/usr/bin/
chmod +x qppcad/usr/bin/qppcad

cp ../../data/images/icon.svg qppcad/icon.svg
cp ./qppcad.desktop qppcad/qppcad.desktop
cp ./AppRun.in qppcad/AppRun

export CONDA_PYTHON_VERSION=3.6
./linuxdeploy-x86_64.AppImage --appdir qppcad --plugin qt
./linuxdeploy-x86_64.AppImage --appdir qppcad --plugin conda
./appimagetool-x86_64.AppImage qppcad







