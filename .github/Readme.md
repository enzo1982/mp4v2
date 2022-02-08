## MP4v2 - *A C/C++ library to create, modify and read MP4 files*
[![Last commit](https://img.shields.io/github/last-commit/enzo1982/mp4v2.svg)](https://github.com/enzo1982/mp4v2/commits/main) [![Last commit](https://img.shields.io/github/v/release/enzo1982/mp4v2)](https://github.com/enzo1982/mp4v2/releases)

This is the *new* MP4v2 project, a fork of the [abandoned MP4v2 library](https://code.google.com/archive/p/mp4v2/) project now archived at Google Code.

The MP4v2 library provides an API to create and modify MP4 files as defined by ISO-IEC:14496-1:2001 MPEG-4 Systems. This file format is derived from Apple's QuickTime file format that has been used as a multimedia file format in a variety of platforms and applications. It is a very powerful and extensible format that can accommodate practically any type of media.

Please visit [mp4v2.org](https://mp4v2.org) for more information about this project and its goals.

### License
MP4v2 is released under the terms of the Mozilla Public License (MPL) Version 1.1. Please refer to the [`COPYING`](https://raw.githubusercontent.com/enzo1982/mp4v2/main/COPYING) file for the full license text.

### Download
Release packages of the MP4v2 library sources are available in the *[Releases](https://github.com/enzo1982/mp4v2/releases)* area.

### Building
The MP4v2 library and utilities can be built using the following build systems:

 - **GNU Autotools**
	1. Run `autoreconf -i` in the MP4v2 project base folder
	(only necessary when building directly from repository sources)
	2. Run `./configure && make` in the MP4v2 project base folder

- **CMake**
	- Run `cmake . && make` in the MP4v2 project base folder

- **Visual Studio**
	- Use the Visual Studio solution at `vstudio/mp4v2.sln`

- **Xcode**
	- Use the Xcode project at `xcode/mp4v2.xcodeproj`

MP4v2 has no special dependencies other than a working C++ compiler and build environment.

### Support
Please use the *[Issues](https://github.com/enzo1982/mp4v2/issues)* area to report bugs and other issues or make feature requests. For questions and general discussion, please use the *[Discussions](https://github.com/enzo1982/mp4v2/discussions)* area.

If you need to contact the maintainer, please send an email to [support@mp4v2.org](mailto:support@mp4v2.org).
