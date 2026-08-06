#!/bin/bash

APP_NAME="GQuestRedux"

sudo rm -rf "/opt/$APP_NAME"
sudo rm "/usr/local/bin/$APP_NAME"
sudo rm "/usr/share/applications/$APP_NAME.desktop"
sudo rm "/usr/share/icons/hicolor/scalable/apps/$APP_NAME.svg"

sudo update-desktop-database /usr/share/applications || true

echo "$APP_NAME removed."