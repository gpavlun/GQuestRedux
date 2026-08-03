#!/bin/bash

OPTION=${1:-0}

if [ "$OPTION" -eq 0 ]; then
    cmake --build build --target render 2>&1
else
    x-terminal-emulator -e bash -c 'cmake --build build --target render --verbose; echo; read -n1 -rsp "press any key to close"' &
fi