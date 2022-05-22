## MP4v2
#### *A C/C++ library to create, modify and read MP4 files*
This is the *new* MP4v2 project, a fork of the [abandoned MP4v2 library](https://code.google.com/archive/p/mp4v2/) project now archived at Google Code.

The MP4v2 library provides an API to create and modify MP4 files as defined by ISO-IEC:14496-1:2001 MPEG-4 Systems. This file format is derived from Apple's QuickTime file format that has been used as a multimedia file format in a variety of platforms and applications. It is a very powerful and extensible format that can accommodate practically any type of media.

### Table of contents

1. [Motivation and goals](#motivation)
2. [Roadmap](#roadmap)
3. [License](#license)
4. [Documentation](#documentation)
5. [Download](#download)
6. [Building](#building)
7. [Support](#support)

### Motivation and goals <a name="motivation"/>
The MP4v2 library is used by various open source projects and commercial software products. Yet the project has been abandoned since 2015 with the last official release dating back even further to 2012.

Being unmaintained for such an extended period, a number of pain points have developed over time. The build system needs an update, bugs and security issues need to be fixed and new formats and standards have been developed that are not supported by the existing code.

Application developers have created and maintained their own sets of patches during this time to address the most urgent issues, but so far nobody has stepped up as a new maintainer for the project.

I would like to fill this gap. I would like to take on the task of bringing the existing code up to date and commit to long term maintenance of MP4v2.

My goal is to revive development of the MP4v2 library. That includes fixing reported bugs, addressing security issues, reviewing and integrating pull requests and adding new functionality.

I would like this new project to become the place to go for anything related to MP4v2 development. I welcome the community to report issues, suggest features and submit their own patches and improvements as pull requests.

Ultimately, my goal for this project is for it to be recognized as the official source of a renewed MP4v2.

### Past milestones
#### *MP4v2 v2.1.0*
- Build systems
	- Update/fix the Autotools build system and get ready to publish new releases ✔️
	- Add support for building with CMake ✔️
	- Update/add Visual Studio and Xcode projects ✔️
- Update code
	- Import fixes made in various MP4v2 forks since the abandonment of the original project ✔️
	- Fix compiler warnings and errors ✔️
	- Fix issues found by Cppcheck ✔️
	- Fix open CVEs:
		- [https://nvd.nist.gov/vuln/detail/CVE-2018-7339](https://nvd.nist.gov/vuln/detail/CVE-2018-7339) ❓
		(probably fixed by [d781fcf1](https://github.com/enzo1982/mp4v2/commit/d781fcf1) and [35c20e02](https://github.com/enzo1982/mp4v2/commit/35c20e02))
		- [https://nvd.nist.gov/vuln/detail/CVE-2018-17235](https://nvd.nist.gov/vuln/detail/CVE-2018-17235) ✔️
		- [https://nvd.nist.gov/vuln/detail/CVE-2018-17236](https://nvd.nist.gov/vuln/detail/CVE-2018-17236) ✔️
- New features
	- Add proper callback based I/O to replace MP4ReadProvider and MP4CreateProvider[Ex] ✔️
- Update documentation
	- Update outdated API man pages and move their contents to Doxygen comments in header files ✔️
	- Regenerate Doxygen API documentation ✔️

### Roadmap <a name="roadmap"/>
#### Short term - *MP4v2 v2.2.0*
- Import improvements made in various MP4v2 forks since the abandonment of the original project:
	- Support for ALAC, Enhanced AC-3 and LPCM audio tracks
	- Support for HEVC and AV1 video tracks
	- Support for Dolby Vision metadata
	- Support for Closed Captions tracks
	- Support for WebVTT tracks
	- Support for managing track references
	- Improve tagging support
- Add support for automatically switching to 64 bit mode when file size exceeds 4 GB

#### Near term - *MP4v2 v2.3.0*
- Add API functions for defining custom atoms
- Add support for additional media formats:
	- Support for FLAC and Opus audio tracks
	- Support for VVC and VPx video tracks
- Add/improve support for MP4 related standards:
	- Support for [DASH](https://en.wikipedia.org/wiki/Dynamic_Adaptive_Streaming_over_HTTP) files
	- Improve support for QuickTime (QTFF) files
	- Improve support for 3GPP files

#### Mid term - *MP4v2 v3.0.0*
- Revise public API for improved usability and consistency
- Add a public C++ API
- Use fuzzing to harden the library against errors and possible vulnerabilities

#### Long term - *MP4v2 future releases*
- Maintain the MP4v2 library by fixing issues, reviewing and integrating pull requests and adding new features as necessary

### License <a name="license"/>
MP4v2 is released under the terms of the Mozilla Public License (MPL) Version 1.1. Please refer to the [`COPYING`](https://raw.githubusercontent.com/enzo1982/mp4v2/main/COPYING) file in the Git repository for the full license text.

### Documentation <a name="documentation"/>
The automatically generated documentation for the public MP4v2 API can be found at [mp4v2.org/api](https://mp4v2.org/api).

### Download <a name="download"/>
Release packages of the MP4v2 library sources are available in the *[Releases](https://github.com/enzo1982/mp4v2/releases)* area on GitHub.

### Building <a name="building"/>
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

### Support <a name="support"/>
Please use the *[Issues](https://github.com/enzo1982/mp4v2/issues)* area on GitHub to report bugs and other issues or make feature requests. For questions and general discussion, please use the *[Discussions](https://github.com/enzo1982/mp4v2/discussions)* area.

If you need to contact the maintainer, please send an email to [support@mp4v2.org](mailto:support@mp4v2.org).
