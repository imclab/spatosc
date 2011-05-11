Basic Audio Renderer, written in PD, by Z. Settel
(spatOSC compliant audio renderer)

uses spatOSC translator:  "basic"



Requirements:  

PD-extended 
 http://puredata.info/community/projects/software/pd-extended

and PDSHEEFA
http://code.sat.qc.ca/trac/pdsheefa



Communication with the audio renderer is done using OSC messaging on specified ports.

By default, the renderer accepts OSC messages from:
	1) the spatOSC library on port: 18032  
	2) control processes on port: 18111  for control/config messaging

These ports may be specified if the audio renderer is launched from the command line. For example:

pd-extended  -send "spatOSC-port 18032" -send "renderer-port 18111" ....

--------------------


spatOSC messages for the basic audio renderer 
and this particular renderer's control/config message set are documented at:

https://github.com/sat-metalab/spatosc/wiki/spatOSC-Basic-Renderer-Spec



May 2011