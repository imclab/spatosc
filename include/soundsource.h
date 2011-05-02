/*
 * This file is part of Spatosc.
 *
 * Copyright (c) 2010 Society for Arts and Technologies <info@sat.qc.ca>
 *
 * Spatosc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Spatosc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Spatosc.  If not, see <http://www.gnu.org/licenses/>.
 */

/** @file
 * The SoundSource class.
 */
#ifndef __SOUNDSOURCE_H__
#define __SOUNDSOURCE_H__

#include "node.h"
#include "memory.h"

namespace spatosc
{

class Connection;

/**
 * Sound source node.
 */
class SoundSource : public Node
{
    public:
        SoundSource(const std::string &nodeID, Scene &scene);
        virtual std::string getType() const { return "source"; }
        void addConnectionTo(const std::tr1::shared_ptr<Connection> &conn);
        void removeConnectionTo(const Connection *conn);

        // TODO: media extension:

        /**
         * Possible URIs for media??:
        file://soundfilename.wav
        http://www.server.com/soundfile.wav
        adc://1:1
        content://media/external/audio/media/710
        mms://some_media_stream
        rtsp://127.0.0.1:12311
        pd_plugin://audio_plugin_patch.pd
        */

        void setMediaURI(std::string uri) { mediaURI_ = uri; }

        /*
        enum PlayingState {NOT_APPLICABLE, PLAYING, PAUSED};
        enum LoopingState {NOT_LOOPING, LOOPING};

        void setLoopingState(LoopingState loop);
        // play() may need a timestamp?
        virtual void play() { currentLoopingState = PLAYING; onMediaChanged(); }
        virtual void stop() { currentPlayingState = PAUSED; onMediaChanged(); }
        virtual void seek(double normalizedIndex);
        */

        // NOTE: need onMediaChanged() that tells "media-style" translators to
        // update their respective renderers.
        //
        // To start, we will have 2 translators that make use of these variables
        // - The existing Basic renderer AND D-Mitri translator
        // (In the case of D-Mitri, we send messages to WildTracks)


    private:

        std::string mediaURI_;
        /*
        PlayingState currentPlayingState;
        LoopingState currentLoopingState;
        */


        std::vector<std::tr1::shared_ptr<Connection> > connectTO_;
        virtual void onNodeChanged(bool forcedNotify);
        virtual bool handleMessage_(const std::string &/*method*/, int argc, lo_arg ** /*argv*/, const char *types);
};

} // end namespace spatosc

#endif // __SOUNDSOURCE_H__
