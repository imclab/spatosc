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

#include "gui.h"
#include "application.h"
#include "audio_scene.h"
#include <algorithm>

namespace 
{
    /**
     * Draws a grid.
     *
     * The parent must have a size.
     * Note that it adds it to its parent.
     */
    ClutterActor *create_grid(ClutterContainer *parent, gfloat interval_x, gfloat interval_y, ClutterColor *color)
    {
        // group
        ClutterActor *group = clutter_group_new();
        clutter_container_add_actor(CLUTTER_CONTAINER(parent), CLUTTER_ACTOR(group));
        gfloat parent_w = clutter_actor_get_width(CLUTTER_ACTOR(parent));
        gfloat parent_h = clutter_actor_get_height(CLUTTER_ACTOR(parent));

        gfloat x, y;
        ClutterActor *rect = NULL;
        // vertical lines:
        for (x = 0.0; x < parent_w; x += interval_x)
        {
            rect = clutter_rectangle_new_with_color(color);
            clutter_container_add_actor(CLUTTER_CONTAINER(group), CLUTTER_ACTOR(rect));
            clutter_actor_set_height(rect, parent_h);
            clutter_actor_set_width(rect, 1.0);
            clutter_actor_set_position(rect, x, 0.0);
        }
        // horizontal lines:
        for (y = 0.0; y < parent_h; y += interval_y)
        {
            rect = clutter_rectangle_new_with_color(color);
            clutter_container_add_actor(CLUTTER_CONTAINER(group), CLUTTER_ACTOR(rect));
            clutter_actor_set_height(rect, 1.0);
            clutter_actor_set_width(rect, parent_w);
            clutter_actor_set_position(rect, 0.0, y);
        }
        return group;
    }

    /**
     * Draws a cross for the origin of a cartesian diagram. (in the center)
     *
     * The parent must have a size.
     * Note that it adds it to its parent.
     */
    ClutterActor *create_origin_axis(ClutterContainer *parent, ClutterColor *color)
    {
        // group
        ClutterActor *group = clutter_group_new();
        clutter_container_add_actor(CLUTTER_CONTAINER(parent), CLUTTER_ACTOR(group));
        gfloat parent_w = clutter_actor_get_width(CLUTTER_ACTOR(parent));
        gfloat parent_h = clutter_actor_get_height(CLUTTER_ACTOR(parent));
        g_print("w: %f h: %f\n", parent_w, parent_h);

        ClutterActor *rect = NULL;
        // vertical line:
        rect = clutter_rectangle_new_with_color(color);
        clutter_container_add_actor(CLUTTER_CONTAINER(group), CLUTTER_ACTOR(rect));
        clutter_actor_set_height(rect, parent_h);
        clutter_actor_set_width(rect, 1.0);
        clutter_actor_set_position(rect, parent_w / 2.0, 0.0);
        // horizontal line:
        rect = clutter_rectangle_new_with_color(color);
        clutter_container_add_actor(CLUTTER_CONTAINER(group), CLUTTER_ACTOR(rect));
        clutter_actor_set_height(rect, 1.0);
        clutter_actor_set_width(rect, parent_w);
        clutter_actor_set_position(rect, 0.0, parent_h / 2.0);
        return group;
    }

    void paintCircle(ClutterActor *actor)
    {
        gfloat radius = std::min(clutter_actor_get_width(actor), 
                clutter_actor_get_height(actor)) * 0.5;
        cogl_set_source_color4ub(0xff, 0xcc, 0x33, 0xcc);
        cogl_path_arc(radius, radius, radius, radius, 0, 360);
        cogl_path_fill();
    }

    /// FIXME: use clutter_cairo, it looks much nicer
    ClutterActor *createCircle(gfloat radius)
    {
        const ClutterColor transp = {0};
        /// HACK: create a rectangle, then override its paint method to draw a circle
        ClutterActor *circle = clutter_rectangle_new_with_color(&transp);
        clutter_actor_set_anchor_point_from_gravity(circle,
                CLUTTER_GRAVITY_CENTER);
        clutter_actor_set_size(circle, radius * 2, radius * 2);
        g_signal_connect(circle, "paint", G_CALLBACK(paintCircle), NULL);
        return circle;
    }
} // end anonymous namespace

void GUI::clutterInit()
{
    clutter_init(0, 0);
}

GUI::GUI(Application &owner) : 
    owner_(owner),
    radius_(20.0),
    sourceActor_(createCircle(radius_)),
    default_stage_width_(480.0f), 
    default_stage_height_(480.0f)
{
    createStage();
    connectMouseCallbacks();
    connectKeyCallbacks();
    moveSourceToOrigin();
}

void GUI::moveSourceToOrigin()
{
    float windowWidth = clutter_actor_get_width(stage_);
    float windowHeight = clutter_actor_get_height(stage_);
    // move sourceActor to middle
    clutter_actor_set_position(sourceActor_, windowWidth * 0.5f,
            windowHeight * 0.5f);
    clutter_actor_set_size(sourceActor_, 2 * radius_, 2 * radius_);
    owner_.getAudio().moveSourceToOrigin();
}

#if CLUTTER_CHECK_VERSION(1, 4, 0)
void GUI::on_drag_motion(ClutterDragAction *action, ClutterActor *actor,
    gfloat delta_x, gfloat delta_y, gpointer data)
{
    GUI *context = static_cast<GUI*>(data);
    float xPos = clutter_actor_get_x(actor) + delta_x;
    float yPos = clutter_actor_get_y(actor) + delta_y;
    bool stopDrag = false;
    float windowWidth = clutter_actor_get_width(context->stage_);
    float windowHeight = clutter_actor_get_height(context->stage_);

    if (xPos >= windowWidth)
    {
        stopDrag = true;
        clutter_actor_set_x(actor, windowWidth);
    }
    else if (xPos <= 0.0f)
    {
        stopDrag = true;
        clutter_actor_set_x(actor, 0.0f);
    }
    else if (yPos >= windowHeight)
    {
        stopDrag = true;
        clutter_actor_set_y(actor, windowHeight);
    }
    else if (yPos <= 0.0f)
    {
        stopDrag = true;
        clutter_actor_set_y(actor, 0.0);
    }

    if (delta_x > 0.0)
        context->owner_.getAudio().moveSourceRight();
    else if (delta_x < 0.0)
        context->owner_.getAudio().moveSourceLeft();

    if (delta_y > 0.0)
        context->owner_.getAudio().moveSourceUp();
    else if (delta_y < 0.0)
        context->owner_.getAudio().moveSourceDown();

    // in Clutter 2.0 we will be able to simply return FALSE instead of calling g_signal_stop_emission_by_name
    if (stopDrag)
        g_signal_stop_emission_by_name(action, "drag-motion");
}
#endif

void GUI::connectMouseCallbacks()
{
    // connect mouse event signals
   // g_signal_connect(stage_, "motion-event", G_CALLBACK(pointerMotionCb), this);
    g_signal_connect(stage_, "scroll-event", G_CALLBACK(pointerScrollCb), this);
    
    // Make it draggable
#if CLUTTER_CHECK_VERSION(1, 4, 0)
    ClutterAction *dragAction = clutter_drag_action_new();
    g_signal_connect(dragAction, "drag-motion", G_CALLBACK(on_drag_motion), 
            this);
    clutter_actor_set_reactive(sourceActor_, TRUE);
    clutter_actor_add_action(sourceActor_, dragAction);
#endif
}

void GUI::connectKeyCallbacks()
{
    g_signal_connect(stage_,
            "key-press-event",
            G_CALLBACK(keyPressCb),
            this);
}

void GUI::createStage()
{
    // create the clutter gui
    stage_ = clutter_stage_get_default();
    clutter_actor_set_size(stage_, default_stage_width_, default_stage_height_);
    clutter_stage_set_title(CLUTTER_STAGE(stage_), "Moving Noise");
    ClutterColor black = { 0x00, 0x00, 0x00, 0xff };
    clutter_stage_set_color(CLUTTER_STAGE(stage_), &black);
    clutter_container_add(CLUTTER_CONTAINER(stage_), sourceActor_, NULL);
    ClutterColor grid_color = { 0xff, 0xff, 0xff, 0x66 };
    create_grid(CLUTTER_CONTAINER(stage_), 10.0f, 10.0f, &grid_color);
    ClutterColor origin_color = { 0xff, 0xff, 0xff, 0xff };
    create_origin_axis(CLUTTER_CONTAINER(stage_), &origin_color);
    clutter_actor_show(stage_);
}

gboolean GUI::keyPressCb(ClutterActor *actor,
        ClutterEvent *event,
        gpointer data)
{
    GUI *context = static_cast<GUI*>(data);
    guint keyval = clutter_event_get_key_symbol(event);

    //ClutterModifierType state = clutter_event_get_state(event);
    //gboolean shift_pressed = (state & CLUTTER_SHIFT_MASK ? TRUE : FALSE);
    //gboolean ctrl_pressed = (state & CLUTTER_CONTROL_MASK ? TRUE : FALSE);
    gboolean handled = FALSE;

    switch (keyval)
    {
        case CLUTTER_KEY_q:
        case CLUTTER_KEY_Q:
        case CLUTTER_KEY_Escape:
            clutter_main_quit();
            return TRUE;

        case CLUTTER_KEY_Up:
            context->owner_.getAudio().moveSourceUp();
            clutter_actor_move_by(context->sourceActor_, 0.0f, -1.0f);
            return TRUE;

        case CLUTTER_KEY_Down:
            context->owner_.getAudio().moveSourceDown();
            clutter_actor_move_by(context->sourceActor_, 0.0f, 1.0f);
            return TRUE;

        case CLUTTER_KEY_Left:
            context->owner_.getAudio().moveSourceLeft();
            clutter_actor_move_by(context->sourceActor_, -1.0f, 0.0f);
            return TRUE;

        case CLUTTER_KEY_Right:
            context->owner_.getAudio().moveSourceRight();
            clutter_actor_move_by(context->sourceActor_, 1.0f, 0.0f);
            return TRUE;

        case CLUTTER_KEY_o:
        case CLUTTER_KEY_O:
            context->moveSourceToOrigin();
            return TRUE;

        default:
            break;
    }

    /* The event was not handled, and the emission should continue */
    return handled;
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
            context->owner_.getAudio().moveSourceRaise();
            break;

        case CLUTTER_SCROLL_DOWN:
            // decrease circle radius
            clutter_actor_set_size(context->sourceActor_, circleWidth - 1,
                    circleHeight - 1);
            context->owner_.getAudio().moveSourceLower();
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
