#include <AL/al.h>
#include <AL/alut.h>
#include <clutter/clutter.h>
#include <cmath>
#include <iostream>

struct SourceData {
    SourceData(ClutterActor *actor) : sourceActor(actor)
    {}

    ~SourceData()
    {
        alDeleteSources(1, &soundSource);
    }

    ALfloat soundSourcePos[3];
    ALuint soundSource;
    ClutterActor *sourceActor;
};

void paint_circle(ClutterActor *actor)
{
    gfloat radius = std::min(clutter_actor_get_width(actor), 
            clutter_actor_get_height(actor)) / 2.0f;
    cogl_set_source_color4ub(0x0, 0x0, 0x0, 0xff);
    cogl_path_arc(radius, radius, radius, radius, 0, 360);
    cogl_path_fill();
}

ClutterActor *create_circle(gfloat radius)
{
    const ClutterColor transp = {0};
    ClutterActor *circle = clutter_rectangle_new_with_color(&transp);
    clutter_actor_set_size(circle, radius * 2, radius * 2);
    g_signal_connect(circle, "paint", G_CALLBACK(paint_circle), NULL);
    return circle;
}


// scrolling causes the sound source to move in the z direction
gboolean pointer_scroll_cb(ClutterActor *actor, ClutterEvent *event,
        gpointer data)
{
    SourceData *context = static_cast<SourceData*>(data);

    /* determine the direction the mouse was scrolled */
    ClutterScrollDirection direction;
    direction = clutter_event_get_scroll_direction (event);
    gfloat circleWidth;
    gfloat circleHeight;

    switch (direction)
    {
        case CLUTTER_SCROLL_UP:
            // increase circle radius
            clutter_actor_get_size(context->sourceActor, &circleWidth,
                    &circleHeight);
            clutter_actor_set_size(context->sourceActor, circleWidth + 1,
                    circleHeight + 1);
            // increase sound source's position in z
            context->soundSourcePos[2] += 0.1;
            alSourcefv(context->soundSource, AL_POSITION, 
                    context->soundSourcePos);
            break;

        case CLUTTER_SCROLL_DOWN:
            // decrease circle radius
            clutter_actor_get_size(context->sourceActor, &circleWidth,
                    &circleHeight);
            clutter_actor_set_size(context->sourceActor, circleWidth - 1,
                    circleHeight - 1);
            // decrease sound source's position in z
            context->soundSourcePos[2] -= 0.1;
            alSourcefv(context->soundSource, AL_POSITION,
                    context->soundSourcePos);
            break;
        default:
            break;
    }

    return TRUE; /* event has been handled */
}

gboolean pointer_motion_cb(ClutterActor *actor, ClutterEvent *event,
        gpointer data)
{ 
    SourceData *context = static_cast<SourceData*>(data);

    /* get the coordinates where the pointer crossed into the actor */
    gfloat stage_x, stage_y, stage_width, stage_height,
           distance_from_source_x, distance_from_source_y;

    stage_width = clutter_actor_get_width(actor);
    stage_height = clutter_actor_get_height(actor);
    clutter_event_get_coords(event, &stage_x, &stage_y);
    distance_from_source_x = stage_x / stage_width;
    distance_from_source_y = stage_y / stage_height;

    context->soundSourcePos[0] = distance_from_source_x;
    context->soundSourcePos[1] = distance_from_source_y;
    alSourcefv(context->soundSource, AL_POSITION, context->soundSourcePos);

    clutter_actor_set_position(CLUTTER_ACTOR(context->sourceActor), stage_x, 
            stage_y);

    return TRUE;
}

int main(int argc, const char* argv[])
{
    alutInit(0, 0);
    clutter_init(0, 0);

    SourceData data(create_circle(20));

    alGetError(); // clear the error bit

    // position of source sound
    for (int i = 0; i != 3; ++i)
        data.soundSourcePos[i] = 0.0;

    // velocity of source sound
    ALfloat sourceVel[] = {0.0, 0.0, 0.0};

    // position of listener
    ALfloat listenerPos[] = {0.0, 0.0, 0.0};

    // listener velocity
    ALfloat listenerVel[] = {0.0, 0.0, 0.0};

    // orientation of the listener (first 3 elements are "at", second 3 are "up"
    ALfloat listenerOri[] = {0.0, 0.0, -1.0, 0.0, 1.0, 0.0};

    // if argv[1] is present, it's a filename
    ALuint buffer = alutCreateBufferFromFile(argc > 1 ? argv[1] :
            "/usr/share/sounds/alsa/Noise.wav");

    // sources are points emitting sound

    alGenSources(1, &data.soundSource);
    alSourcei(data.soundSource, AL_BUFFER, buffer);
    alSourcef(data.soundSource, AL_PITCH, 1.0f);
    alSourcef(data.soundSource, AL_GAIN, 1.0f);
    alSourcefv(data.soundSource, AL_POSITION, data.soundSourcePos);
    alSourcefv(data.soundSource, AL_VELOCITY, sourceVel);
    alSourcei(data.soundSource, AL_LOOPING, AL_TRUE);

    if (alGetError() != AL_NO_ERROR)
        return 1;

    // set listener values
    alListenerfv(AL_POSITION, listenerPos);
    alListenerfv(AL_VELOCITY, listenerVel);
    alListenerfv(AL_ORIENTATION, listenerOri);

    // begin the source playing
    alSourcePlay(data.soundSource);

    // create the clutter gui
    ClutterActor *stage = clutter_stage_get_default();
    clutter_stage_set_title(CLUTTER_STAGE(stage), "Moving Noise");

    // put listener in middle
    ClutterActor *listenerActor = create_circle(20);
    clutter_actor_set_position(listenerActor, clutter_actor_get_width(stage) * 0.5, 
            clutter_actor_get_height(stage) * 0.5);
    clutter_container_add(CLUTTER_CONTAINER(stage), data.sourceActor, listenerActor, NULL);
    clutter_actor_show(stage);
    // connect mouse event signals
    g_signal_connect(stage, "motion-event", G_CALLBACK(pointer_motion_cb), &data);
    g_signal_connect(stage, "scroll-event", G_CALLBACK(pointer_scroll_cb), &data);

    // main loop
    clutter_main();

    clutter_actor_destroy(stage);

    alDeleteBuffers(1, &buffer);

    alutExit();
    return 0;
}
