#pragma once
#include <iostream>
#include <Eigen/Eigen>

using namespace std;
using namespace Eigen;

namespace PolygonalLibrary {

struct PolygonalMesh {

    /* Le celle 0D (0-dimensionali) sono i vertici: ogni vertice ha 1 codice id, 1 marker e 2 coordinate
     * Le celle 1D sono i lati: ogni lato ha un codice id, 2 vertici che connette (vengono riportati i loro id), 1 marker
     * Le celle 3D sono le facce: ogni faccia ha un codice id, N vertici e N lati
     */

    unsigned int NumberCell0D = 0;   //numero vertici
    vector<unsigned int> Cell0DId = {}; // id vertici, dim 1x NumberCell0D
    vector<Vector2d> Cell0DCoordinates = {};  //coord dei vertici, dim 2 x NumberCell0D (x,y)
    map<unsigned int, list<unsigned int>> Cell0DMarkers = {};  // markers dei vertici, dim 1 x NumberCell0D

    unsigned int NumberCell1D = 0;  // numero lati
    vector<unsigned int> Cell1DId = {}; // id lati, dimensione 1x Numbercell1D
    vector<Vector2i> Cell1DVertices = {};  // id vertici connessi da un lato, dim 2 x NumberCell1D
    map<unsigned int, list<unsigned int>> Cell1DMarkers = {};  // merkers dei lati, size 1 x NumberCell1D (marker)

    unsigned int NumberCell2D = 0; // numero facce
    vector<unsigned int> Cell2DId = {}; // id facce, dim 1 x NumberCell2D
    vector<vector<unsigned int>> Cell2DVertices = {};  // id vertici connessi da una faccia, dim 1x numero vertici coinvolti nella faccia
    vector<vector<unsigned int>> Cell2DEdges = {}; // id lati connessi da una faccia, dim 1x numero lati coinvolti nella faccia
    map<unsigned int, list<unsigned int>> Cell2DMarkers = {};
};


}
