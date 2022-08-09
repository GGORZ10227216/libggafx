# libggafx
An experimental GBA PPU emulation library with an ImGui front-end

This project is still work in progress, feel free to send me an issue if you:
- Found a bug
- Think of a feature that you find useful/interesting

# Build

I have been tested the whole build process with MSYS2 and linux system with gcc toolchain

## Dependency library
- libsdl2
- libfmt-dev

## Step
```
git clone https://github.com/GGORZ10227216/libggafx.git
cd ./libggafx /
mkdir build && cd build
cmake ../
make
```

# Todo:
- Bitmap mode
- Full functional sprite emulation
- Mosaic and alpha blending effect 
