#include "World.h"
#include "Field.h"
#include "Output.h"
#include "PotentialSolver.h"
#include <iostream>

int main()
{
    World world(21, 21, 21);
    std::cout << "All ok after main world" << std::endl;
    world.setExtends({-0.1, -0.1, 0}, {0.1, 0.1, 0.2});

    // set phi[i=0] = 1 for testing
    for (int j = 0; j < world.nj; j++)
        for (int k = 0; k < world.nk; k++)
            world.phi[0][j][k] = 1; // phi[i=0] = 1

    // set phi[k=0] = 2 for testing
    for (int i = 0; i < world.ni; i++)
        for (int j = 0; j < world.nj; j++)
            world.phi[i][j][0] = 2; // phi[k=0] = 2

    std::cout << "All ok after just before PotentialSolver" << std::endl;
    // Initialize and solve potential
    PotentialSolver solver(world, 5000, 1e-4);

    std::cout << "All ok after just after PotentialSolver" << std::endl;

    solver.solve();

    // save results
    Output::fields(world);
    return 0;
}