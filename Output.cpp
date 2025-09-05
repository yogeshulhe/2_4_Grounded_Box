#include "Output.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include "Field.h"

// saves output in VTK format
void Output::fields(World &world)
{
    std::stringstream name; // build file name
    name << "fields.vti";

    // open output file
    std::ofstream out(name.str());
    if (!out.is_open())
    {
        std::cerr << "Could not open " << name.str() << std::endl;
        return;
    }

    // ImageData is a VTK format for structured Cartesian meshes
    out << "<VTKFile type=\"ImageData\">\n";
    double3 x0 = world.getX0();
    double3 dh = world.getDh();
    out << "<ImageData Origin=\"" << x0[0] << " " << x0[1] << " " << x0[2] << "\" ";
    out << "Spacing=\"" << dh[0] << " " << dh[1] << " " << dh[2] << "\" ";
    out << "WholeExtent=\"0 " << world.ni - 1 << " 0 " << world.nj - 1 << " 0 " << world.nk - 1 << "\">\n";

    // output data stored on nodes (point data)
    out << "<PointData>\n";

    // // node volumes, scalar
    // out << "<DataArray Name=\"NodeVol\" NumberOfComponents=\"1\" format=\"ascii\" type=\"Float64\">\n";
    // out << world.node_vol; // use the overloaded << operator
    // out << "</DataArray>\n";

    // potential scalar
    out << "<DataArray Name=\"phi\" NumberOfComponents=\"1\" format=\"ascii\" type=\"Float64\">\n";
    out << world.phi; // use the overloaded << operator
    out << "</DataArray>\n";

    // charge density, scalar
    /* ... */ // output world.rho

    // electric field, 3 component vector
    out << "<DataArray Name=\"ef\" NumberOfComponents=\"3\" format=\"ascii\" type=\"Float64\">\n";
    out << world.ef;
    out << "</DataArray>\n";

    //  close the tags
    out << "</PointData>\n";
    out << "</ImageData>\n";
    out << "</VTKFile>\n";
} // file closed here as "out" goes out of the scope