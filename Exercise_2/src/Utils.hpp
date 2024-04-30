#pragma once
#include <iostream>
#include "PolygonalMesh.hpp"


using namespace std;

/* - Le prime 4 funzioni importano la mesh e le varie componenti (vertici, lati, facce) e danno cpme risultato vero o falso in base alla riuscita dell'operazione.
 * - Le ultime 2 testano che tutti i lati non siano degeneri a punti e che i poligoni non siano degeneri a lati o punti. Danno come risultato avvisi in output.
 */

namespace PolygonalLibrary {

bool ImportMesh (const string& filepath, PolygonalMesh& mesh);
bool ImportCell0Ds (const string& filename, PolygonalMesh& mesh);
bool ImportCell1Ds (const string& filename, PolygonalMesh& mesh);
bool ImportCell2Ds (const string& filename, PolygonalMesh& mesh);
void CheckEdgesNonDegeneracy (PolygonalMesh& mesh, double tol);
void CheckAreasNonDegeneracy (PolygonalMesh& mesh, double tol);
}


