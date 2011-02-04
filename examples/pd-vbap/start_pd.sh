#!/bin/bash
# apt-get install pd pd-vbap pd-oscrxtx
pd \
    -path /usr/lib/pd/extra/vbap \
    -helppath /usr/lib/pd/extra/vbap \
    -path /usr/lib/pd/extra/oscrxtx \
    -helppath /usr/lib/pd/extra/oscrxtx \
    -outchannels 8 \
    -r 48000 \
    -jack \
    spatosc.vbap.8~-help.pd

