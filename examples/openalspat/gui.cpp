
// GUI.cpp
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

#include "gui.h"
#include <algorithm>

namespace 
{
    void paintCircle(ClutterActor *actor)
    {
        gfloat radius = std::min(clutter_actor_get_width(actor), 
                clutter_actor_get_height(actor)) * 0.5;
        cogl_set_source_color4ub(0x0, 0x0, 0x0, 0xff);
        cogl_path_arc(radius, radius, radius, radius, 0, 360);
        cogl_path_fill();
    }

    /// FIXME: use clutter_cairo, it looks much nicer
    ClutterActor *createCircle(gfloat radius)
    {
        const ClutterColor transp = {0};
        /// HACK: create a rectangle, then override its paint method to draw a circle
        ClutterActor *circle = clutter_rectangle_new_with_color(&transp);
        clutter_actor_set_size(circle, radius * 2, radius * 2);
        g_signal_connect(circle, "paint", G_CALLBACK(paintCircle), NULL);
        return circle;
    }
} // end anonymous namespace

void GUI::clutterInit()
{
    clutter_init(0, 0);
}

GUI::GUI() : 
    sourceActor_(createCircle(20))
{
    createStage();
    connectMouseCallbacks();
    connectKeyCallbacks();
}

void GUI::connectMouseCallbacks()
{
    // connect mouse event signals
    g_signal_connect(stage_, "motion-event", G_CALLBACK(pointerMotionCb), this);
    g_signal_connect(stage_, "scroll-event", G_CALLBACK(pointerScrollCb), this);
}

void GUI::connectKeyCallbacks()
{
    ClutterBindingPool *bindingPool = clutter_binding_pool_get_for_class(CLUTTER_STAGE_GET_CLASS(stage_));

    clutter_binding_pool_install_action(bindingPool,
            "quit",      /* identifier */
            CLUTTER_KEY_Q, /* up key pressed */
            CLUTTER_SHIFT_MASK,              /* no modifiers pressed */
            G_CALLBACK (keyQuitCb),
            NULL,           /* no user data passed */
            NULL);
    clutter_binding_pool_install_action(bindingPool,
            "quit",      /* identifier */
            CLUTTER_KEY_Escape, /* up key pressed */
            CLUTTER_RELEASE_MASK,              /* no modifiers pressed */
            G_CALLBACK (keyQuitCb),
            NULL,           /* no user data passed */
            NULL);
    g_signal_connect(stage_,
            "key-press-event",
            G_CALLBACK(keyPressCb),
            NULL);
}

void GUI::keyQuitCb(GObject *instance, 
        const gchar *action_name,
        guint key_val,
        ClutterModifierType modifiers, 
        gpointer user_data)
{
    g_print("Q\n");
    clutter_main_quit();
}

void GUI::createStage()
{
    // create the clutter gui
    stage_ = clutter_stage_get_default();
    clutter_stage_set_title(CLUTTER_STAGE(stage_), "Moving Noise");

    clutter_container_add(CLUTTER_CONTAINER(stage_), sourceActor_, NULL);
    clutter_actor_show(stage_);
}

gboolean GUI::keyPressCb(ClutterActor *actor,
        ClutterEvent *event,
        gpointer user_data)
{
    g_print("%s\n", __FUNCTION__);
    ClutterBindingPool *pool;

    pool = clutter_binding_pool_find(G_OBJECT_TYPE_NAME(actor));

    return clutter_binding_pool_activate(pool,
            clutter_event_get_key_symbol(event),
            clutter_event_get_state(event),
            G_OBJECT (actor));
}

// main loop
void GUI::run()
{
    clutter_main();
    clutter_actor_destroy(stage_);
}

// scrolling causes the sound source to move in the z direction
gboolean GUI::pointerScrollCb(ClutterActor *actor, ClutterEvent *event,
        gpointer data)
{
    GUI *context = static_cast<GUI*>(data);

    /* determine the direction the mouse was scrolled */
    ClutterScrollDirection direction;
    direction = clutter_event_get_scroll_direction (event);
    gfloat circleWidth;
    gfloat circleHeight;
    clutter_actor_get_size(context->sourceActor_, &circleWidth,
            &circleHeight);

    switch (direction)
    {
        case CLUTTER_SCROLL_UP:
            // increase circle radius
            clutter_actor_set_size(context->sourceActor_, circleWidth + 1,
                    circleHeight + 1);
            // increase sound source's position in z
            /*context->sourcePos_[2] += 0.1;
            alSourcefv(context->source_, AL_POSITION, 
                    context->sourcePos_);
                    */
            break;

        case CLUTTER_SCROLL_DOWN:
            // decrease circle radius
            clutter_actor_set_size(context->sourceActor_, circleWidth - 1,
                    circleHeight - 1);
            // decrease sound source's position in z
            /*context->soundSourcePos[2] -= 0.1;
            alSourcefv(context->soundSource_, AL_POSITION,
                    context->sourcePos_);*/
            break;
        default:
            break;
    }

    return TRUE; /* event has been handled */
}

gboolean GUI::pointerMotionCb(ClutterActor *actor, ClutterEvent *event,
        gpointer data)
{ 
    GUI *context = static_cast<GUI*>(data);

    /* get the coordinates where the pointer crossed into the actor */
    gfloat stageX, stageY, stageWidth, stageHeight,
           distanceFromSourceX, distanceFromSourceY;

    stageWidth = clutter_actor_get_width(actor);
    stageHeight = clutter_actor_get_height(actor);
    clutter_event_get_coords(event, &stageX, &stageY);
    distanceFromSourceX = stageX / stageWidth;
    distanceFromSourceY = stageY / stageHeight;

    /*
    context->soundSourcePos[0] = distanceFromSourceX;
    context->soundSourcePos[1] = distanceFromSourceY;
    alSourcefv(context->soundSource, AL_POSITION, context->soundSourcePos);
    */

    clutter_actor_set_position(CLUTTER_ACTOR(context->sourceActor_), stageX, 
            stageY);

    return TRUE;
}
