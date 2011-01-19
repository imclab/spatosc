#include "application.h"

int main(int argc, const char* argv[])
{
    Application::initSubsystems();
    Application app;
    app.start();
    return 0;
}
