# OpenCVGodot

This is a GDExtension to interface godot projects with the OpenCV library. My long term objective is to fully support OpenCV library, but this is still very early stage, feel free to contact me if you want to contribute.

## How To Use

The extension is built to Demo/bin for quick testing, after building you may move the extension to your own project bin folder. (In the future we will setup autobuilds so you can download it directly)

## Build & Install

Here's an example of how to build & install a release version (use the terminal to run the following commands in the parent directory of this repo):

### Not MSVC

```sh
sh build.bash
```

Or

```sh
$ cmake -B GDExtensionTemplate-build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=GDExtensionTemplate-install GDExtensionTemplate
$ cmake --build GDExtensionTemplate-build --parallel
$ cmake --install GDExtensionTemplate-build
```

### MSVC

```sh
$ cmake -B GDExtensionTemplate-build -G"Visual Studio 17 2022"  -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=GDExtensionTemplate-install GDExtensionTemplate
$ cmake --build GDExtensionTemplate-build --config Release
$ cmake --install GDExtensionTemplate-build
```

Be sure to switch between Debug and realease flags acording to your current needs.
