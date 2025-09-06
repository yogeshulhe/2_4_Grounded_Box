#include <math.h>
#include "Species.h"
#include "World.h"

void Species::loadParticlesBox(double3 x1, double3 x2, double num_den, int num_mp)
{
    double box_vol = (x2[0] - x1[0]) * (x2[1] - x1[1]) * (x2[2] - x1[2]); // box volume
    double num_real = num_den * box_vol;                                  // number of real particles
    double mpw = num_real / num_mp;                                       // macroparticle weight

    // Preallocate memory for particles
    particles.reserve(num_mp);

    /*load particles on an equally spaced grid*/
    for (int p = 0; p < num_mp; p++)
    {
        // sample random position
        double3 pos;
        pos[0] = x1[0] + rnd() * (x2[0] - x1[0]);
        pos[1] = x1[1] + rnd() * (x2[1] - x1[1]);
        pos[2] = x1[2] + rnd() * (x2[2] - x1[2]);

        // set initial velocity
        double3 vel{0, 0, 0};       // stationary particle
        addParticle(pos, vel, mpw); // add a new particle to the array
    }
}

// adds new entry to the particle vector
void Species::addParticle(double3 pos, double3 vel, double mpw)
{
    particles.emplace_back(pos, vel, mpw);
}