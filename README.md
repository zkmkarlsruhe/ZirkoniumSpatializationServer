#Zirkonum Spatailization Server

### Overview

This software is the rendering server for 3D sound spatialization that ZKM | Institute for Music and Acoustics employs for its 3D surround audio system, Klangdom.

![Klangdom](resources/Klangdom.png)

The software is programmed in [PureData](https://puredata.info), an open source visual programming language by Miller Puckette.

This engine is embeded in the [Zirkonium Trajectory Editor](http://zkm.de/zirkonium), a toolkit for spatial composition and performance, employing libPd.

### Dependencies

To open the files, you will need [PureData](https://puredata.info). 
In order to realize VBAP (Vector-based amplitude panning) the software employs an custom-built object based on the external Pd object implemented by Ville Pukki available from his [website](http://legacy.spa.aalto.fi/research/cat/vbap/).

### Contribution

If you find a bug, please file it as an issue.

