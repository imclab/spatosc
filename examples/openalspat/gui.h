/**
 * This file is part of spatosc.
 *
 * Copyright (c) 2010 Society for Arts and Technology <info@sat.qc.ca>
 *
 * spatosc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * spatosc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with spatosc.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _CLUTTERGUI_H_
#define _CLUTTERGUI_H_

#include <clutter/clutter.h>

class Application;

class GUI 
{
    private:
        Application &owner_;
        float radius_;
        ClutterActor *sourceActor_;
        gfloat default_stage_width_;
        gfloat default_stage_height_;
        ClutterActor *stage_;
        void createStage();
        void connectMouseCallbacks();
        void connectKeyCallbacks();
        void moveSourceToOrigin();

        // callbacks
        static gboolean pointerMotionCb(ClutterActor *actor, 
                ClutterEvent *event,
                gpointer data);

        static gboolean pointerScrollCb(ClutterActor *actor, 
                ClutterEvent *event,
                gpointer data);

        static gboolean keyPressCb(ClutterActor *actor,
                ClutterEvent *event,
                gpointer data);

        static void keyQuitCb(GObject *instance, 
                const gchar *action_name,
                guint key_val,
                ClutterModifierType modifiers, 
                gpointer data);
#if CLUTTER_CHECK_VERSION(1, 4, 0)
        static void on_drag_motion(ClutterDragAction *action, ClutterActor *actor,
                gfloat delta_x, gfloat delta_y, gpointer data);
#endif

    public:
        /// Initialize clutter subsystem
        static void clutterInit();
        GUI(Application &owner);
        void run();
};

#endif // _CLUTTERGUI_H_
