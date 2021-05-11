zirkhoa
=======

Higher Order Ambisonic Pure Data external for use with the Zirkonium spatialization server

https://zkm.de/zirkonium

Based on the HOA library from CICM France, version 2.2

https://github.com/CICM/HoaLibrary-Light

2015 Chikashi Miyama  
ZKM | Hertz-Lab (former ZKM | Institute for Music and Aocustics)

Build
-----

Make sure you have a copy of Pure Data installed to your system.

This repository includes the CICM HoaLibrary-Light source files as a git submodule. Before building, check out the submodules with:

    git submodule update --init --recursive

Compile with:

    make

Note: Currently, this external relies on an older version of the HoaLibrary-Light, namely the `HoaLibrary-v2.2` tag. It will not build with the main branch.

Usage
-----

To load this external in Pd, include the directory containing the following files in your Pd search paths:

* zirkhoa.pd_* or zirkhoa.d_fat: compiled external
* zirkhoa-help.pd: help file
