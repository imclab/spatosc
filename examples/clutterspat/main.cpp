/**
 * Clutter spatosc example
 *
 * This example is in the public domain.
 */
#include <clutter/clutter.h>
#include <spatosc.h>
#include <algorithm>
#include <tr1/memory>

static const float WINDOW_WIDTH = 500;
static const float WINDOW_HEIGHT = 500;
static const char *WINDOW_TITLE = "Press arrow keys to move the sound source";

/**
 * Info for our little application.
 */
struct ExampleApplication
{
    ClutterActor *foo_actor;
    spatosc::SoundSource *foo_sound;
};

/**
 * We override the paint method of a rectangle to actually paint a circle.
 *
 * We use its original color, but with an alpha of 100%.
 */
void paint_circle(ClutterActor *actor)
{
    gfloat radius = std::min(clutter_actor_get_width(actor), 
            clutter_actor_get_height(actor)) / 2.0f;
    ClutterColor color;
    clutter_rectangle_get_color(CLUTTER_RECTANGLE(actor), &color);
    cogl_set_source_color4ub(color.red, color.green, color.blue, 0xff);
    cogl_path_arc(radius, radius, radius, radius, 0, 360);
    cogl_path_fill();
}

/**
 * Handler for key events.
 */
static void key_event_cb(ClutterActor *actor, ClutterKeyEvent *event,
        gpointer data)
{
    ExampleApplication *app = static_cast<ExampleApplication *>(data);
    switch (event->keyval)
    {
        case CLUTTER_Escape:
            clutter_main_quit();
            break;
        case CLUTTER_Up:
            clutter_actor_move_by(app->foo_actor, 0.0f, -1.0f);
            break;
        case CLUTTER_Down:
            clutter_actor_move_by(app->foo_actor, 0.0f, 1.0f);
            break;
        case CLUTTER_Right:
            clutter_actor_move_by(app->foo_actor, 1.0f, 0.0f);
            break;
        case CLUTTER_Left:
            clutter_actor_move_by(app->foo_actor, -1.0f, 0.0f);
            break;
        default:
            break;
    }
    app->foo_sound->setPosition(clutter_actor_get_x(app->foo_actor),
            clutter_actor_get_y(app->foo_actor), 0);
}

/**
 * Scrolling causes the sound source to move in the z direction.
 */
gboolean pointer_scroll_cb(ClutterActor *actor, ClutterEvent *event,
        gpointer data)
{
    ExampleApplication *app = static_cast<ExampleApplication *>(data);

    ClutterScrollDirection direction;
    direction = clutter_event_get_scroll_direction(event);
    gfloat actor_width;
    gfloat actor_height;
    clutter_actor_get_size(app->foo_actor, &actor_width,
            &actor_height);

    switch (direction)
    {
        case CLUTTER_SCROLL_UP:
            // increase circle radius
            clutter_actor_set_size(app->foo_actor, actor_width * 1.1,
                    actor_height * 1.1);
            // TODO increase sound source's position in z
            break;

        case CLUTTER_SCROLL_DOWN:
            // decrease circle radius
            clutter_actor_set_size(app->foo_actor, actor_width * 0.9f,
                    actor_height * 0.9);
            // TODO: decrease sound source's position in z
            break;

        default:
            break;
    }

    return TRUE; /* event has been handled */
}

#if CLUTTER_CHECK_VERSION(1, 4, 0)
static void on_drag_motion( ClutterDragAction *action, ClutterActor *actor,
    gfloat delta_x, gfloat delta_y, gpointer data)
{
    ExampleApplication *app = static_cast<ExampleApplication *>(data);
    (void) app; // Unused
    float x_pos = clutter_actor_get_x(actor) + delta_x;
    float y_pos = clutter_actor_get_y(actor) + delta_y;
    bool stop_it = false;

    if (x_pos >= WINDOW_WIDTH)
    {
        stop_it = true;
        clutter_actor_set_x(actor, WINDOW_WIDTH);
    }
    else if (x_pos <= 0.0f)
    {
        stop_it = true;
        clutter_actor_set_x(actor, 0.0f);
    }
    else if (y_pos >= WINDOW_HEIGHT)
    {
        stop_it = true;
        clutter_actor_set_y(actor, WINDOW_HEIGHT);
    }
    else if (y_pos <= 0.0f)
    {
        stop_it = true;
        clutter_actor_set_y(actor, 0.0);
    }

    // in Clutter 2.0 we will be able to simply return FALSE instead of calling g_signal_stop_emission_by_name
    if (stop_it)
        g_signal_stop_emission_by_name(action, "drag-motion");
}

#endif

int main(int argc, char *argv[])
{
    using namespace spatosc;
    using std::tr1::shared_ptr;
    Scene scene;
    ClutterActor *stage = NULL;
    ClutterColor black = { 0x00, 0x00, 0x00, 0xff };
    ClutterColor orange = { 0xff, 0xcc, 0x33, 0x00 }; /* transparent orange */
    ExampleApplication app;

    clutter_init(&argc, &argv);
    stage = clutter_stage_get_default();
    clutter_stage_set_color(CLUTTER_STAGE(stage), &black);
    clutter_stage_set_title(CLUTTER_STAGE(stage), WINDOW_TITLE);
    clutter_actor_set_size(stage, WINDOW_WIDTH, WINDOW_HEIGHT);

    app.foo_actor = clutter_rectangle_new_with_color(&orange);
    g_signal_connect(app.foo_actor, "paint", G_CALLBACK(paint_circle), NULL);
    clutter_container_add_actor(CLUTTER_CONTAINER(stage), app.foo_actor);
    clutter_actor_set_anchor_point_from_gravity(app.foo_actor,
            CLUTTER_GRAVITY_CENTER);
    clutter_actor_set_size(app.foo_actor, 50.0f, 50.0f);
    clutter_actor_set_position(app.foo_actor, WINDOW_WIDTH / 2.0f,
            WINDOW_HEIGHT / 2.0f);
    
    // Make it draggable
#if CLUTTER_CHECK_VERSION(1, 4, 0)
    ClutterAction *drag_action = clutter_drag_action_new();
    g_signal_connect(drag_action, "drag-motion", G_CALLBACK(on_drag_motion), 
            static_cast<gpointer>(&app));
    clutter_actor_set_reactive(app.foo_actor, TRUE);
    clutter_actor_add_action(app.foo_actor, drag_action);
#endif

    app.foo_sound = scene.getOrCreateSoundSource("foo_sound");
    app.foo_sound->setChannelID(1);
    shared_ptr<Translator> translator(new DmitriTranslator("192.168.2.26"));
    scene.setTranslator(translator);
    app.foo_sound->setPosition(clutter_actor_get_x(app.foo_actor),
            clutter_actor_get_y(app.foo_actor), 0);
    scene.debugPrint();

    g_signal_connect(stage, "key-press-event", G_CALLBACK(key_event_cb),
            static_cast<gpointer>(&app));
    g_signal_connect(stage, "scroll-event", G_CALLBACK(pointer_scroll_cb), 
            static_cast<gpointer>(&app));
            
    clutter_actor_show(stage);

    clutter_main();
    return 0;
}
