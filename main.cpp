#include "World.h"
#include "Field.h"
#include "Output.h"
#include "PotentialSolver.h"
#include "Species.h"
#include <iostream>

int main()
{
    World world(21, 21, 21);
    std::cout << "All ok after main world" << std::endl;
    world.setExtends({-0.1, -0.1, 0}, {0.1, 0.1, 0.2});

    // set up particle species
    std::vector<Species> species;
    // species.reserve(2); // pre-allocate space for two species
    species.push_back(Species("O+", 16 * Const::AMU, Const::QE, world));
    species.push_back(Species("e-", Const::ME, -1 * Const::QE, world));

    int np_ions = 80000;                                                      // number of simulation ions
    int np_eles = 10000;                                                      // number of simulation electrons
    species[0].loadParticlesBox(world.getX0(), world.getXm(), 1e11, np_ions); // ions
    species[1].loadParticlesBox(world.getX0(), world.getXc(), 1e11, np_eles); // electrons

    std::cout
        << "all ok after species" << std::endl;

    // use range-style loop to print particle counts
    for (Species &sp : species)
        std::cout << sp.name << " has " << sp.getNp() << " particle" << std::endl;
}