#include <iostream>
#include "spatosc.h"

static const bool VERBOSE = false;
using namespace spatosc;

static bool vectorsMatch(Vector3 a, Vector3 b)
{
    if (a.x != b.x || a.y != b.y || a.z != b.z)
    {
        std::cout << "Expected :(" << a.x << ", " << a.y << ", " << a.z
            << ") but got (" << b.x << ", " << b.y << ", " << b.z << ")" << std::endl;
        return false;
    }
    return true;
}

bool check_aed_to_xyz()
{
    if (! vectorsMatch(Vector3(0.0, -1.0, 0.0), sphericalToCartesian(
            Vector3(M_PI, 0.0, 1.0))))
        return false;
    return true;
}

int main(int /*argc*/, char ** /*argv*/)
{
    if (! check_aed_to_xyz())
        return 1;
    return 0;
}

