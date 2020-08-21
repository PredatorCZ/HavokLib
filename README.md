# HavokLib

[![Build Status](https://travis-ci.org/PredatorCZ/HavokLib.svg?branch=master)](https://travis-ci.org/PredatorCZ/HavokLib)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Coverage Status](https://coveralls.io/repos/github/PredatorCZ/HavokLib/badge.svg?branch=master)](https://coveralls.io/github/PredatorCZ/HavokLib?branch=master)

HavokLib is an independent Havok packfile serialize and XML store library for C++.\
In addition, it can output packfile formats into XML/Bin under any available version. This library can possibly upgrade/downgrade between versions.\
Library can also store classic data for XML direct export.\
Library uses wrapper for a python 2.7 module.\
Library is compilable under VS 2017+, GCC 5+, Clang 3.5+.

![Library Diagram](http://www.plantuml.com/plantuml/proxy?cache=no&src=https://raw.github.com/predatorcz/havoklib/master/doc/diag.puml)

## Supported formats/toolset versions

Havok 5.0.0 - 2017

It also supports both endians, both x86 and x64 platforms and files compiled for all available compilers.\
For versions 2015+, there aren't any samples for unit tests. Only MSVC formats tested!\
This library doesn't support loading tagfiles and never will.\
Binary exporting is an experimental feature for now.

## Supported classes

* hkaSkeleton: 5.0.0 - 2017
* hkRootLevelContainer: 5.0.0 - 2017
* hkaAnimationContainer: 5.0.0 - 2017
* hkaAnimation (not final): 5.0.0 - 2017
* hkaDeltaCompressedAnimation: 5.0.0 - 2010
* hkaWaveletCompressedAnimation: 5.0.0 - 2010 (/wo decompressor)
* hkaAnnotationTrack: 5.0.0 - 2017
* hkaInterleavedAnimation: 5.0.0 - 2015
* hkaAnimationBinding: 5.0.0 - 2017
* hkxEnvironment: 5.0.0 - 2014
* hkaSplineCompressedAnimation: 5.0.0 - 2017
* hkaDefaultAnimatedReferenceFrame: 5.0.0 - 2014

## Supported XML classes

* hkRootLevelContainer
* hkaAnimationContainer
* hkaSkeleton
* hkaAnnotationTrack
* hkaInterleavedAnimation
* hkaAnimationBinding
* hkxEnvironment

## Python wrapper classes

* hkRootLevelContainer
* hkaAnimationContainer
* hkaSkeleton
* hkxEnvironment

## Binary export classes (v5 - v2014)

* hkRootLevelContainer (read-only)
* hkaAnimationContainer (read-only)
* hkaSkeleton (read-only)
* hkxEnvironment (read-only)
* hkNamedVariant
* hkaPartition

## License

This library is available under GPL v3 license. (See LICENSE.md)

This library uses following libraries:

* PugiXml, Copyright (c) 2006-2019 Arseny Kapoulkine
* PreCore, Copyright (c) 2016-2020 Lukas Cone
