zirkhrtf~
=========

Modified version of the earplug~ Pure Data external for use with the Zirkonium spatialization server

https://zkm.de/zirkonium

Adapted in 2015 Chikashi Miyama  
ZKM | Hertz-Lab (former ZKM | Institute for Music and Aocustics)

Original Readme
---------------

RT binaural filter: earplug~  
based on KEMAR impulse measurement  
Pei Xiang, summer 2004  
Revised in Fall 2006 by Jorge Castellanos  
Revised in Spring 2009 by Hans-Christoph Steiner to compile in the data file

http://markmail.org/message/sxfauaymftshbgwz  
http://lists.puredata.info/pipermail/pd-list/2005-02/025764.html

On Jan 31, 2005, at 5:05 PM, Pei Xiang wrote:

> hey, marc,
> actually i've just written one external that handles this, [earplug\~]
> http://crca.ucsd.edu/~pxiang/research.htm it basically takes the KEMAR data
> set, and interpolates 366 locations where HRTF measurement exists in a
> spherical surface. you get azimuth control 0-360 and elevation -40 - 90.
>
> now i'm still trying to clean it up a little bit, but probably have to use by
> copy the data.txt file into the default search dir of Pd, for the moment.
>
> cheers, Pei 
>
>
> --- metafor <metafor at gmx.net> wrote:
>
>> just wondering, is there a puredata object or
>> abstraction
>> existing for the "head  related transfer function"?
>>
>> or other externals which work with spcacialisation
>> of sounds,
>> preferably for headphones.
>>
>> thanks
>> marc

Build
-----

Make sure you have a copy of Pure Data installed to your system.

Compile with:

    make

Usage
-----

To load this external in Pd, include the directory containing the following files in your Pd search paths:

* zirkhrtf~.pd_* or zirkhrtf~.d_fat: compiled external
* zirkhrtf~-help.pd: help file
* zirkhrtf_data.txt: KEMAR data set
