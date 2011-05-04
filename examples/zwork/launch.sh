 #!/bin/bash

# optional arg for spin sceneID
OS=`uname -s` 



DIR_PATH=${0%launch.sh}

#PATCHES_PATH=$DIR_PATH../patches



PDFLAGS=" -path  $DIR_PATH../patches"



if [ -z $1 ] ; then
	SPINSERV=spinserver
	SPINVIEW=spinviewer
	else
	SPINSERV="spinserver --scene-id $1"
	SPINVIEW="spinviewer --scene-id $1"
	
    PDMESS="GLOBAL_SPINWIDGETS_CONFIG sceneID $1"
	
fi




if [ "$OS" = "Darwin" ]; then

	OS_VER=`sw_vers -productVersion`
	OS_VER=${OS_VER:0:4}

		PDE=/Applications/Pd-extended.app/Contents/Resources/bin/pd

	echo using $PD
	
	else
	
		PDE=pdextended
fi



killall Pd-extended; killall spinserver; killall spinviewer;  



#$SPINSERV &
#$SPINVIEW --screen 1 &

echo launch audio renderer

$PDE -send "spatOSC-port 18032" -send "renderer-port 18222" -open spatosc_basic-renderer.pd  spatOSC-client.pd &

#$PDE -send "spatOSC-port 18032" -open spatosc_basic-renderer.pd &
#$PDE -send "spatOSC-port 18032" -open spatOSC-client.pd &

#$PDE -open renderer.pd  spin.renderer.pd spinEdit.pd &
