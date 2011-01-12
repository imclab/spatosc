/**
 * Clutter spatosc example
 */
#include <clutter/clutter.h>
#include <vAudioManager.h>
#include <vPlugin_dmitri.h>

static const unsigned int WINDOW_WIDTH = 500;
static const unsigned int WINDOW_HEIGHT = 500;

struct ExampleApplication
{
    ClutterActor *foo_actor;
    vSoundSource *foo_sound;
};

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
    vAudioManager::Instance().debugPrint();
}

int main(int argc, char *argv[])
{
    ClutterActor *stage = 0;
    ClutterColor black = { 0x00, 0x00, 0x00, 0xff };
    ClutterColor orange = { 0xff, 0xcc, 0x33, 0xff };
    ExampleApplication app;

    clutter_init(&argc, &argv);
    stage = clutter_stage_get_default();
    clutter_stage_set_color(CLUTTER_STAGE(stage), &black);
    clutter_stage_set_title(CLUTTER_STAGE(stage), "Clutter spatosc example");
    clutter_actor_set_size(stage, WINDOW_WIDTH, WINDOW_HEIGHT);

    app.foo_actor = clutter_rectangle_new_with_color(&orange);
    clutter_container_add_actor(CLUTTER_CONTAINER(stage), app.foo_actor);
    clutter_actor_set_anchor_point_from_gravity(app.foo_actor,
            CLUTTER_GRAVITY_CENTER);
    clutter_actor_set_size(app.foo_actor, 50.0f, 50.0f);
    clutter_actor_set_position(app.foo_actor, WINDOW_WIDTH / 2.0f,
            WINDOW_HEIGHT / 2.0f);

    app.foo_sound = vAudioManager::Instance().getOrCreateSoundSource("foo_sound");
    app.foo_sound->setChannelID(1);
    vAudioManager::Instance().setPlugin(new vPlugin_dmitri("192.168.2.26"));
    vAudioManager::Instance().debugPrint();

    g_signal_connect(stage, "key-press-event", G_CALLBACK(key_event_cb),
            static_cast<gpointer>(&app));
    clutter_actor_show(stage);

    clutter_main();
    return 0;
}
