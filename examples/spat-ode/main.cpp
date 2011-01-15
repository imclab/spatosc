/** @file
 * ODE spatosc example
 * 
 * This example is in the public domain.
 */

#include <spatosc.h>
#include <ode/ode.h>
#include <ctime>
#include <string>
#include <tr1/memory>

using std::tr1::shared_ptr;

struct Box
{
    dBodyID body;
    //dMass mass;
    //std::string audio_node;
};

struct ExampleApp
{
    dWorldID world;
    dSpaceID space;
    std::vector<shared_ptr<Box> > boxes;
};

/**
 * Creates some stuff in our ODE and spatosc world.
 */
static void create_stuff(ExampleApp &app)
{
    // Set random seed
    dRandSetSeed(std::time(0));
    // set gravity
    dWorldSetGravity(app.world, 0.0, -9.8, 0.0);

    for (int i = 0; i < 3; i++)
    {
        shared_ptr<Box> box(new Box);
        app.boxes.push_back(box);
        box->body = dBodyCreate(app.world);
        //dMassSetSphereTotal(&box->mass, i * 1.0, 1.0);
        //dBodySetMass(box->body, &app.boxes[i]->mass);
        dBodySetLinearVel(box->body, (i - 1) * -1.0, 0.0, 0.0);
    }
}

/**
 * Runs our example application.
 */
static void run_app(ExampleApp &app)
{
    // TODO
}

int main(int argc, char *argv[])
{
    dInitODE2(0);
    ExampleApp app;
    app.world = dWorldCreate();

    create_stuff(app);
    run_app(app);

    dWorldDestroy(app.world);
    dCloseODE();
    return 0;
}

