#ifndef World_h
#define World_h

#include "Field.h"
#include <random>

class World
{

public:
    World(int ni, int nj, int nk); // constructor

    // sets the mesh span, also recompute cell spacing
    void setExtends(const double3 &_x0, const double3 &_xm);

    const int3 nn;        // number of nodes
    const int ni, nj, nk; // number of nodes in individual variable

    Field phi;
    Field rho;
    Field3 ef;
    Field node_vol;

    double3 getX0() const { return x0; }
    double3 getDh() const { return dh; }
    double3 getXc() const { return xc; }
    double3 getXm() const { return xm; }

protected:
    double3 x0; // mesh origin
    double3 dh; // cell spacing
    double3 xm; // mesh max bound
    double3 xc; // domain centroid
};

namespace Const
{
    const double EPS_0 = 8.8541878e-12; // C/V/m, vac. permittivity
    const double QE = 1.602176565e-19;  // C, electron charge
    const double AMU = 1.660538921e-27; // kg, atomic mass unit
    const double ME = 9.10938215e-31;   // kg, electron mass
    const double K = 1.380648e-23;      // J/K, Boltzmann constant
    const double PI = 3.141592653;      // Pi
    const double EvToK = QE / K;        // 1eV in K 11604
}

// object for sampling random numbers
class Rnd
{
public:
    // constructor: set initial random seed and distribution limits
    Rnd() : mt_gen{std::random_device()()}, rnd_dist{0, 1.0} {}

    double operator()() { return rnd_dist(mt_gen); }

protected:
    std::mt19937 mt_gen;                             // random number generation        // #iclude<random>
    std::uniform_real_distribution<double> rnd_dist; // distribution
};

extern Rnd rnd;

#endif