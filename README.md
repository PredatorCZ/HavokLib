# HavokLib
HavokLib is an independent Havok packfile serialize and XML store library for C++.

In addition, it can output packfile formats into XML under any available version. This library can possibly upgrade/downgrade between versions.

In another addition, library can store classic data for XML direct export.

Library is compilable under VS 2015, 2017, 2019.

## Supported formats/toolset versions
Havok 5.5.0 - 2014.1, (2015?), 2016

It also supports both endians, both x86 and x64 platforms and files compiled for all available compilers.

Version 2016 is only tested under x64 MSVC Little-endian.

## Supported classes
NOTE: this library was created mostly for reading hkaSkeleton classes

* hkaSkeleton: 5.5.0 - 2014.1, 2016
* hkRootLevelContainer: 5.5.0 - 2014.1, 2016
* hkaAnimationContainer: 5.5.0 - 2014.1, 2016
* hkaAnimation (not final): 5.5.0 - 2014.1
* hkaDeltaCompressedAnimation: 5.5.0 - 2010
* hkaWaveletCompressedAnimation: 5.5.0 - 2010
* hkaAnnotationTrack: 5.5.0 - 2014.1
* hkaInterleavedAnimation: 5.5.0 - 2014.1
* hkaAnimationBinding: 5.5.0 - 2014.1

## Supported XML classes
* hkRootLevelContainer
* hkaAnimationContainer
* hkaSkeleton
* hkaAnnotationTrack
* hkaInterleavedAnimation
* hkaAnimationBinding

## Testing/samples and usage
HavokLibSamples project contains code for testing and usage of library.

In order to test, you will need to unpack ***Bin/samples_to_unpack.7z***.

## License
This library is available under GPL v3 license. (See LICENSE.md)

This library uses following libraries:

* PugiXml, Copyright (c) 2006-2019 Arseny Kapoulkine
* PreCore, Copyright (c) 2016-2019 Lukas Cone
