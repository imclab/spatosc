#!/usr/bin/env python

"""
Rotates a sound source around.
"""
# FIXME: The scene doesn't seem to flush the messages!

import spatosc
import time

def run():
    scene = spatosc.Wrapper()
    scene.setVerbose(True)
    scene.addTranslatorWithAddress("translator0", "BasicTranslator", "osc.udp://127.0.0.1:10000");
    scene.createSource("source0")
    scene.createListener("listener0")
    # scene.setSynchronous(True) # FIXME: does this work at all?
    scene.debugPrint()
    try:
        angle = 0.0
        while True:
            time.sleep(1.0)
            angle = ((angle + 10.0) % 360.0)
            scene.setPositionAED("source0", angle * 0.017453292519943, 0.0, 10.0)
            scene.debugPrint()
            print("scene.setPositionAED(\"%s\", %f, %f, %f)" % ("source0", angle, 0.0, 10.0))
            scene.flushMessages()
    except KeyboardInterrupt, e:
        print("")

if __name__ == "__main__":
    run()

