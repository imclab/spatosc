
// GUI.h
//
// Copyright (c) 2010 Tristan Matthews <le.businessman@gmail.com>
//
// This file is part of Spatosc.
//
// Spatosc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Spatosc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Spatosc.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _CLUTTERGUI_H_
#define _CLUTTERGUI_H_

#include <clutter/clutter.h>

class GUI {
    private:
        ClutterActor *sourceActor_;
        ClutterActor *stage_;
        void createStage();
        void connectMouseCallbacks();
        void connectKeyCallbacks();

        // callbacks
        static gboolean pointerMotionCb(ClutterActor *actor, 
                ClutterEvent *event,
                gpointer data);

        static gboolean pointerScrollCb(ClutterActor *actor, 
                ClutterEvent *event,
                gpointer data);

        static gboolean keyPressCb(ClutterActor *actor,
                ClutterEvent *event,
                gpointer user_data);

        static void keyQuitCb(GObject *instance, 
                const gchar *action_name,
                guint key_val,
                ClutterModifierType modifiers, 
                gpointer user_data);
    public:
        /// Initialize clutter subsystem
        static void clutterInit();
        GUI();
        void run();
};

#endif // _CLUTTERGUI_H_
