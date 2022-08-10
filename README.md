# libggafx
![](https://i.imgur.com/T8zLGlI.png)

An experimental GBA PPU emulation library with an ImGui front-end

This project is still work in progress, feel free to send me an issue if you:
- Found a bug
- Think of a feature that you find useful/interesting

# Build

I have been tested the whole build process with MSYS2 and linux system with gcc toolchain

## Library dependencies
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

## Usage
- The front end need 4 VRAM area dumps to work:
  - IO (04000000-040003FE)
  - Palette (05000000-050003FF)
  - VRAM (06000000-06017FFF)
  - OAM (07000000-070003FF)
- You can use NO$GBA's memory dump feature to get them.
- Use "io.bin", "palette.bin", "vram.bin", "oam.bin" as their names, then put them with executable.

## Todo:
- Bitmap mode
- Full functional sprite emulation
- Mosaic and alpha blending effect 
