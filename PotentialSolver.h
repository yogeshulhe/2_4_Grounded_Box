#ifndef potentialSolver_H
#define potentialSolver_H

#include "World.h"

class PotentialSolver
{
public:
    // constructor, sets members to given inputs
    PotentialSolver(World &world, int max_it, double tol) : world(world), max_solver_it(max_it), tolerance(tol) {}

    // solves potential using Gauss-siedal and SOR
    bool solve();

    // computes electric field = - gradient(phi)
    void computeEF();

protected:
    World &world;
    unsigned max_solver_it; // maximum number of solver iteratins
    double tolerance;       // solver tolerance
};

#endif