#Zirkonum Spatailization Server

### Overview

This software is the rendering server for 3D sound spatialization that ZKM | Institute for Music and Acoustics employs for its 3D surround audio system, Klangdom.

![Klangdom](resources/Klangdom.png)

The software is programmed in [PureData](https://puredata.info), an open source visual programming language by Miller Puckette.

This engine is embeded in the [Zirkonium Trajectory Editor](http://zkm.de/zirkonium), a toolkit for spatial composition and performance, employing libPd.

### Dependencies

To open the files, you will need [PureData](https://puredata.info). 
In order to realize VBAP (Vector-based amplitude panning) the software employs an custom-built object based on the external Pd object implemented by Ville Pukki available from his [website](http://legacy.spa.aalto.fi/research/cat/vbap/).

### Control Messages

* PropertiesOfAudioFile _filepath_
* Transport
  - play
  - poll
  - setCurrentTime _time offset_ (seconds?)
  - refreshRate _fps_
* AudioFileInspection (for creating waveform representation)
  - loadToBuffer _max buffer size_ _bytes to skip_ _resize_
  - freeBuffer
* SampleRate _sample rate_
* ID (1 - n, 0 is test id)
  - _id_ input _channel_
  - _id_ position _x_ _y_ _z_
  - _id_ span _0 - 1_ (?)
  - _id_ muted _0 or 1_
  - _id_ algorithm _VBAP or HOA_
  - _id_ optim _HOA optim_ (?)
  - _id_ directOut _channel: out#_ (out#?)
  - _id_ oscAddress (not connected?)
  - _id_ activate _0 or 1_
* Speaker
  - _id_ delay _delay ms_
  - _id_ phaseCorrection _0 or 1_
  - _id_ gain _db_ (range?)
  - _id_ outputChannel _channel_
  - _id_ activate _0 or 1_
  - _id_ bounce _filepath_
* SpeakerDef
  - (dimen?) 2 _x_ _y_ (or ae?)
  - (dimen?) 3 _x_ _y_ _z_ (aed?)
* vbapMapping
* hrtf
* Report
  - enableIDLevelReport _0 or 1_
  - enableSpeakerLevelReport _0 or 1_
* SoundFile
  - open _filepath_
  - play
  - stop
* input gain _dB_ (-100 to 12)
* output gain _dB_ (-100 to 12)
* vbap speed _speed ms_ 
* Verbose _0 or 1_

### Messaging

* #zirk-in: server receive (input)
* #zirk-out: server send (ouput)
* internal sends:
  - #zirk_input
  - #zirk_output
  - #zirk_vbap
  - #zirk_speaker

### Contribution

If you find a bug, please file it as an issue.
