#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include <iostream>

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

int main()
{
    PolygonalMesh mesh;

    if(!ImportMesh("PolygonalMesh", mesh))
    {cerr << "Something went wrong while importing the mesh data" << endl;
        return 1;}

    return 0;
}

