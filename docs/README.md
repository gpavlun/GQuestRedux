## Dependencies

This project is currently aimed at Linux developers. Since there is no official release yet, the dependency list may change as development continues.

### Required

- **GCC** (or another C compiler)
- **CMake**
- **tmux** (used by the development workflow)
- **SDL2**
  - SDL2 is currently the supported version.
  - SDL3 support is planned, but the project has not migrated yet.
- **OpenGL development libraries**
- **Cargo/Rust** *(main branch only)*
  - The current main branch contains some Rust code.
  - A **pure C** branch exists without this dependency.

---

## Install Commands

### Ubuntu / Debian

```bash
sudo apt update
sudo apt install \
    build-essential \
    gcc \
    cmake \
    tmux \
    cargo \
    libsdl2-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    mesa-common-dev
```

### Fedora

```bash
sudo dnf install \
    gcc \
    cmake \
    tmux \
    cargo \
    SDL2-devel \
    mesa-libGL-devel \
    mesa-libGLU-devel
```

### Arch Linux

```bash
sudo pacman -S \
    base-devel \
    gcc \
    cmake \
    tmux \
    rust \
    sdl2 \
    mesa
```

### openSUSE Tumbleweed

```bash
sudo zypper install \
    gcc \
    cmake \
    tmux \
    cargo \
    libSDL2-devel \
    Mesa-libGL-devel \
    glu-devel
```

---

## OpenGL

The renderer currently targets **OpenGL**, so your system will need the OpenGL development libraries installed (included in the commands above).

If your distribution packages OpenGL differently, ensure you have:

- OpenGL (`libGL`)
- GLU (`libGLU`)
- Mesa development headers

These are typically provided by your distribution's Mesa/OpenGL development packages.