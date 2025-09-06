#ifndef Species_H
#define Species_H

#include <string>
#include "World.h"

struct Particle
{
    double3 pos; // position
    double3 vel; // velocity
    double mpw;  // macroparticle weight

    Particle(double3 x, double3 v, double mpw) : pos{x[0], x[1], x[2]}, vel{v[0], v[1], v[2]}, mpw{mpw} {}
};

class Species
{
public:
    // constructor
    Species(std::string name, double mass, double charge, World &world) : name(name), mass(mass), charge(charge), den(world.ni, world.nj, world.nk), world(world) {}

    // returns the number of particles
    size_t getNp() const { return particles.size(); }

    // move all particles using electric field ef[]
    void advance();

    // compute number density
    void computeNumberDensity();

    // adds a new particle
    void addParticle(double3 pos, double3 vel, double mpwt); // mpwt means macro particle weight

    // loads num_mp particles in x1-x2 box with num_den density
    void loadParticlesBox(double3 x1, double3 x2, double num_den, int num_mp);

    const std::string name; // species name
    const double mass;      // particle mass in kg
    const double charge;    // particle charge in Coulomb

    Field den;                       // number density
    std::vector<Particle> particles; // array for storing particles

protected:
    World &world; // reference to the world object
};

#endif