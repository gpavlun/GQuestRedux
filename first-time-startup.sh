#! /bin/bash
echo "Hello! You most likely will want to run everything from the dev-studio." 
echo "That is how this is designed to be used, after all!"
echo ""
echo "Make sure you have SDL2 installed, or this code will not work!"
echo ""
echo "Ubuntu / Debian : sudo apt install libsdl2-dev"
echo "Fedora          : sudo dnf install SDL2-devel"
echo "Arch / Manjaro  : sudo pacman -S sdl2"
echo ""
echo "Another note, the Makefiles you find are largely depricated."
echo "Use them with extreme caution. The code is instead compiled with CMake."
echo "The commands are simple, assuming this script finishes,"
echo "you would only need to run 'cmake --build build'"
echo "The Devstudio handles all of that though! When in doubt, stay there."
echo ""
echo "Dependency list:"
echo "1. cmake"
echo "2. gcc"
echo "3. cargo"
echo "4. pkg-config"
echo "5. sdl2"
echo "6. tmux"
read -n1
echo "[1/6] Checking dependencies..."
command -v cmake >/dev/null || {
    echo "Error: cmake not installed"
    exit 1
}

echo "[2/6] Checking dependencies..."

command -v gcc >/dev/null || {
    echo "Error: gcc not installed"
    exit 1
}

echo "[3/6] Checking dependencies..."

command -v cargo >/dev/null || {
    echo "Error: cargo not installed"
    exit 1
}

echo "[4/6] Checking dependencies..."

command -v pkg-config >/dev/null || {
    echo "Error: pkg-config not installed"
    exit 1
}

echo "[5/6] Checking dependencies..."

pkg-config --exists sdl2 || {
    echo "Error: SDL2 development package not installed"
    exit 1
}

echo "[6/6] Checking dependencies..."

pkg-config -v tmux || {
    echo "Error: SDL2 development package not installed"
    exit 1
}

echo "dependency checking complete"

chmod +x ./scripts/*
chmod +x ./launch.sh
cmake -B build
cmake --build build

# give user option not to launch dev studio
printf "Press q then Enter to quit, or any other key to continue: "
read answer

if [ "$answer" = "q" ]; then
    exit 0
fi

./launch.sh
