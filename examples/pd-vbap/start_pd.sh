#!/bin/bash
# apt-get install pd pd-vbap pd-oscrxtx
pd \
    -path /usr/lib/pd/extra/vbap \
    -helppath /usr/lib/pd/extra/vbap \
    -path /usr/lib/pd/extra/oscrxtx \
    -helppath /usr/lib/pd/extra/oscrxtx \
    -outchannels 8 \
    -alsa \
    spatosc.vbap.8~-help.pd

