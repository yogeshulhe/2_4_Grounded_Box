#include "World.h"

// constructor
World::World(int ni, int nj, int nk) : ni{ni}, nj{nj}, nk{nk}, nn{ni, nj, nk}, phi(ni, nj, nk), rho(ni, nj, nk), ef(ni, nj, nk), node_vol(ni, nj, nk) { std::cout << "All ok in world constructor" << std::endl; }

// set mesh extends and computes the cell spacing
void World::setExtends(const double3 &_x0, const double3 &_xm)
{
    x0 = _x0;
    xm = _xm;
    // compute spacing by dividing length by the number of cells
    for (int i = 0; i < 3; i++)
        dh[i] = (xm(i) - x0(i)) / (nn(i) - 1);

    // compute centroid
    for (int i = 0; i < 3; i++)
        xc[i] = 0.5 * (x0[i] + xm[i]);
}