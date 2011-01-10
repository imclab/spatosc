#include <vAudioManager.h>
#include <vPlugin_dmitri.h>
#include <iostream>

int main(int /*argc*/, char ** /*argv*/)
{
	std::cout << "\nRunning spatoscTest ...\n" << std::endl;

	// The spatosc library provides an API structured around one singleton class
	// called vAudioManager. This class maintains lists of listeners and sound
	// sources, and provides methods to get and create instances of these items.


	// Each scene needs to have at least one vListener:
	//vListener *listener = vAudioManager::Instance().getOrCreateListener("listener");

	// A variable number of vSoundSource instances can then be generated. Note
	// that a bus number must be assigned to each source in order to render in
	// D-Mitri. Setting the bus to -1 will effectively disable computation for
	// that node:
	vSoundSource *foo = vAudioManager::Instance().getOrCreateSoundSource("foo");
	foo->setBus(1);
	vSoundSource *kak = vAudioManager::Instance().getOrCreateSoundSource("kak");
	kak->setBus(2);

	// In order to send OSC, some output plugin must be specified. In this case,
	// we choose D-Mitri, and provide the IP address of the server on the
	// control network. Note that D-Mitri uses 2 interfaces, a control network
	// (typically IPv4) and an audio network (AVB):
	vAudioManager::Instance().setPlugin(new vPlugin_dmitri("192.168.2.26"));

	// The vAudioManager class can print out everything to the console:
	vAudioManager::Instance().debugPrint();

	// Now we just move nodes around and updates should be sent to D-Mitri:

	foo->setPosition(0,10,0);
	kak->setPosition(5,5,0);

	sleep(1);

	foo->setPosition(0,5,0);
	kak->setPosition(-5,5,0);

	vAudioManager::Instance().debugPrint();

    return 0;
}
