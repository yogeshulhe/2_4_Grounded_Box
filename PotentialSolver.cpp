#include "PotentialSolver.h"

bool PotentialSolver::solve()
{
    Field &phi = world.phi; // references to avoid writing world.phi
    Field &rho = world.rho;

    // precompute 1/(dx^2)
    double3 dh = world.getDh();
    double idx2 = 1.0 / (dh[0] * dh[0]); // 1/dx^2
    double idy2 = 1.0 / (dh[1] * dh[1]); // 1/dy^2
    double idz2 = 1.0 / (dh[2] * dh[2]); // 1/dz^2

    double L2 = 0; // norm
    bool converged = false;

    // solve potential
    for (unsigned it = 0; it < max_solver_it; it++)
    {
        for (int i = 1; i < world.ni - 1; i++)
            for (int j = 1; j < world.nj - 1; j++)
                for (int k = 1; k < world.nk - 1; k++)
                {
                    // standard internal open node
                    double phi_new = (rho[i][j][k] / Const::EPS_0 +
                                      idx2 * (phi[i - 1][j][k] + phi[i + 1][j][k]) +
                                      idy2 * (phi[i][j - 1][k] + phi[i][j + 1][k]) +
                                      idz2 * (phi[i][j][k - 1] + phi[i][j][k + 1])) /
                                     (2 * idx2 + 2 * idy2 + 2 * idz2);
                    // SOR
                    phi[i][j][k] = phi[i][j][k] + 1.4 * (phi_new - phi[i][j][k]);
                }

        // check for convergence
        if (it % 25 == 0)
        {
            double sum = 0;
            for (int i = 1; i < world.ni - 1; i++)
                for (int j = 1; j < world.nj - 1; j++)
                    for (int k = 1; k < world.nk - 1; k++)
                    {
                        double R = -phi[i][j][k] * (2 * idx2 + 2 * idy2 + 2 * idz2) +
                                   rho[i][j][k] / Const::EPS_0 +
                                   idx2 * (phi[i - 1][j][k] + phi[i + 1][j][k]) +
                                   idy2 * (phi[i][j - 1][k] + phi[i][j + 1][k]) +
                                   idz2 * (phi[i][j][k - 1] + phi[i][j][k + 1]);
                        sum += R * R;
                    }
            L2 = sqrt(sum / (world.ni * world.nj * world.nk));
            if (L2 < tolerance)
            {
                converged = true;
                break;
            }
        }
    } // iteration loop
    if (!converged)
        std::cerr << "GS failed to converge, L2=" << L2 << std::endl;
    return converged;
}