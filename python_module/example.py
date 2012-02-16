#!/usr/bin/env python

import spatosc
import time

def run():
    scene = spatosc.Wrapper()
    scene.createSource("source0")
    scene.createListener("listener0")
    scene.setPosition("source0", 1.0, 1.0, 1.0)
    scene.debugPrint()

if __name__ == "__main__":
    run()
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt, e:
        print("")

