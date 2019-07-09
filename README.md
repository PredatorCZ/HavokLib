# HavokLib
[![Build Status](https://travis-ci.org/PredatorCZ/HavokLib.svg?branch=master)](https://travis-ci.org/PredatorCZ/HavokLib)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

HavokLib is an independent Havok packfile serialize and XML store library for C++.

In addition, it can output packfile formats into XML under any available version. This library can possibly upgrade/downgrade between versions.

In another addition, library can store classic data for XML direct export.

Library is compilable under VS 2015, 2017, 2019 and GCC 6, 7, 8, 9.

## Supported formats/toolset versions
Havok 5.5.0 - 2014.1, 2015, 2016, 2017

It also supports both endians, both x86 and x64 platforms and files compiled for all available compilers.

Versions 2015, 2016, 2017 are only tested under x64 MSVC Little-endian and they aren't provided as samples.

## Supported classes
* hkaSkeleton: 5.5.0 - 2017
* hkRootLevelContainer: 5.5.0 - 2017
* hkaAnimationContainer: 5.5.0 - 2017
* hkaAnimation (not final): 5.5.0 - 2017
* hkaDeltaCompressedAnimation: 5.5.0 - 2010
* hkaWaveletCompressedAnimation: 5.5.0 - 2010
* hkaAnnotationTrack: 5.5.0 - 2017
* hkaInterleavedAnimation: 5.5.0 - 2015
* hkaAnimationBinding: 5.5.0 - 2017
* hkxEnvironment: 5.5.0 - 2014.1
* hkaSplineCompressedAnimation: 5.5.0 - 2017

## Supported XML classes
* hkRootLevelContainer
* hkaAnimationContainer
* hkaSkeleton
* hkaAnnotationTrack
* hkaInterleavedAnimation
* hkaAnimationBinding
* hkxEnvironment

## Testing/samples and usage
HavokLibSamples project contains code for testing and usage of library.

In order to test, you will need to unpack ***Bin/samples_to_unpack.7z***.

## License
This library is available under GPL v3 license. (See LICENSE.md)

This library uses following libraries:

* PugiXml, Copyright (c) 2006-2019 Arseny Kapoulkine
* PreCore, Copyright (c) 2016-2019 Lukas Cone
