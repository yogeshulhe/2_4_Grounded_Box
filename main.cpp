#include "World.h"
#include "Field.h"
#include "Output.h"
#include <iostream>

int main()
{
    World world(21, 21, 21);
    std::cout << "All ok after main world" << std::endl;
    world.setExtends({-0.1, -0.1, 0}, {0.1, 0.1, 0.2});

    Output::fields(world);
    return 0;
}