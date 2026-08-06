#!/bin/bash

# set -e

# APP_NAME="GQuestRedux"
# SOURCE_BIN="./release/$APP_NAME"
# SOURCE_ICON="./release/$APP_NAME.svg"

# INSTALL_DIR="/opt/$APP_NAME"
# BIN="/usr/local/bin/$APP_NAME"
# ICON_DIR="/usr/share/icons/hicolor/scalable/apps"
# DESKTOP_DIR="/usr/share/applications"

# echo "Installing $APP_NAME..."

# # Check that release files exist
# if [ ! -f "$SOURCE_BIN" ]; then
#     echo "Error: executable not found: $SOURCE_BIN"
#     exit 1
# fi

# if [ ! -f "$SOURCE_ICON" ]; then
#     echo "Error: icon not found: $SOURCE_ICON"
#     exit 1
# fi

# # Install application files
# sudo mkdir -p "$INSTALL_DIR/bin"
# sudo mkdir -p "$INSTALL_DIR/icons"

# sudo cp "$SOURCE_BIN" "$INSTALL_DIR/bin/$APP_NAME"
# sudo cp "$SOURCE_ICON" "$INSTALL_DIR/icons/$APP_NAME.svg"

# # Make executable
# sudo chmod +x "$INSTALL_DIR/bin/$APP_NAME"

# # Create command available in PATH
# sudo ln -sf "$INSTALL_DIR/bin/$APP_NAME" "$BIN"

# # Install icon
# sudo mkdir -p "$ICON_DIR"
# sudo cp "$INSTALL_DIR/icons/$APP_NAME.svg" "$ICON_DIR/$APP_NAME.svg"

# # Create desktop launcher
# sudo tee "$DESKTOP_DIR/$APP_NAME.desktop" > /dev/null <<EOF
# [Desktop Entry]
# Name=GQuestRedux
# Comment=hype
# Exec=$BIN
# Icon=$APP_NAME
# Terminal=false
# Type=Application
# Categories=Utility;
# StartupWMClass=$APP_NAME
# EOF

# # Make desktop file valid
# sudo chmod +x "$DESKTOP_DIR/$APP_NAME.desktop"

# # Refresh menus
# sudo update-desktop-database "$DESKTOP_DIR" || true

# echo "$APP_NAME installed successfully."





#!/bin/bash

APP_NAME="GQuestRedux"

APP_DIR="$(cd "$(dirname "$0")" && pwd)"
BIN="$APP_DIR/release/$APP_NAME"
ICON="$APP_DIR/release/$APP_NAME.svg"

DESKTOP_FILE="$HOME/.local/share/applications/$APP_NAME.desktop"

mkdir -p "$HOME/.local/share/applications"

cat > "$DESKTOP_FILE" <<EOF
[Desktop Entry]
Name=$APP_NAME
Comment=hype
Exec=$BIN
Icon=$ICON
Terminal=false
Type=Application
Categories=Utility;
StartupWMClass=$APP_NAME
EOF

chmod +x "$DESKTOP_FILE"

echo "Installed launcher for $APP_NAME"