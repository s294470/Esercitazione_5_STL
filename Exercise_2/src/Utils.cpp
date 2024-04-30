#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
namespace PolygonalLibrary{


/**** Checks on non degeneracy of lenghts and areas ************************************************************************************************************/


bool ImportMesh (const string& filepath, PolygonalMesh& mesh)
{
    if (!ImportCell0Ds(filepath + "/Cell0Ds.csv", mesh))
    { cerr << "Something went wrong while importing Cell0Ds" << endl;
        return false; }
    else
    {
        cout << "Cell0D marker: " << endl;
        for (auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++)
        {
            cout << "key: \t" << it -> first << "\t values:";
            for(const unsigned int id: it -> second)
                cout << "\t" << id;
            cout << endl;
        }
    }
    if (!ImportCell1Ds(filepath + "/Cell1Ds.csv", mesh))
    { cerr << "Something went wrong while importing Cell1Ds" << endl;
        return false; }
    else
    {
        cout << "Cell1D marker: " << endl;
        for (auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "key: \t" << it -> first << "\t values:";
            for(const unsigned int id: it -> second)
                cout << "\t" << id;
            cout << endl;
        }
    }
    if (!ImportCell2Ds(filepath + "/Cell2Ds.csv", mesh))
    { cerr << "Something went wrong while importing Cell2Ds" << endl;
        return false;}
    else
    {
        cout << "Cell2D marker: " << endl;
        for (auto it = mesh.Cell2DMarkers.begin(); it != mesh.Cell2DMarkers.end(); it++)
        {
            cout << "key: \t" << it -> first << "\t values:";
            for(const unsigned int id: it -> second)
                cout << "\t" << id;
            cout << endl;
        }

        //Controllo sulla coerenza dei vertici
        for(unsigned int c = 0; c < mesh.NumberCell2D; c++)
        {
            vector<unsigned int> edges = mesh.Cell2DEdges[c];
            unsigned int numedges = edges.size();

            for(unsigned int e = 0; e < numedges; e++)
            {
                const unsigned int origin = mesh.Cell1DVertices[edges[e]][0];
                const unsigned int end = mesh.Cell1DVertices[edges[e]][1];

                //controllo che origin del lato sia uno dei vertici del poligono
                auto findOrigin = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), origin);
                if(findOrigin == mesh.Cell2DVertices[c].end())
                {
                    cerr<< "Wrong mesh" << endl;
                    return 2;
                }

                //controllo che end del lato sia uno dei vertici del poligono
                auto findEnd = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), end);
                if(findEnd == mesh.Cell2DVertices[c].end())
                {
                    cerr<< "Wrong mesh" << endl;
                    return 3;
                }
            }
        }

        // Controllo non degenerazione aree e lunghezze
        double tol = 10* numeric_limits<double>::epsilon();
        CheckEdgesNonDegeneracy(mesh, tol); //controllo che i lati non siano degeneri (lunghezza > della tolleranza)
        CheckAreasNonDegeneracy(mesh, tol); //controllo che l'area delle figure non sia degenere (area > della tolleranza)

    }
    return true;
}

/* IMPORT VERTICES ********************************************************************************************************************************************************************************/

bool ImportCell0Ds (const string& filename, PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);

    if (file.fail())
    { cerr << "File containing Cell0Ds could not be opened. " << endl;
        return false; }
    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        replace(line.begin(), line.end(), ';', ' ');
        listLines.push_back(line);        // PUSH_BACK si usa per inserire un elemento
    }

    listLines.pop_front();                // POP_FRONT si usa per prelevare il primo

    mesh.NumberCell0D = listLines.size();

    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }
    mesh.Cell0DId.reserve(mesh.NumberCell0D);    // riservo NumberCell0D posti per memorizzare gli id dei vertici
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);  // riservo NumberCell0D posti per memorizzare le coordinate dei vertici (ogni posto coincide a un vettore 2d)

    for (const string& line : listLines)
    {
        istringstream converter(line);              // creo un oggetto iss chiamato converter che contiene il contenuto della stringa line
        unsigned int id;
        unsigned int marker;
        Vector2d coord;

        converter >> id >> marker >> coord(0) >> coord(1);  // le 4 cose presenti in converter le metto risp. in id, marker, coord(0), coord(1)

        mesh.Cell0DId.push_back(id);            // meto le info raccolte nei posti corretti della mesh
        mesh.Cell0DCoordinates.push_back(coord);

        if (marker != 0)
        {
            // if (mesh.Cell0DMarkers.find(marker) == mesh.Cell0DMarkers.end())
            //     mesh.Cell0DMarkers.insert({marker, {id}});
            // else
            //     mesh.Cell0DMarkers[marker].push_back(id);

            auto ret = mesh.Cell0DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first) -> second.push_back(id);

            //se non ho ancora inserito il marker lo inserisce, altrimenti non cambia nulla
            //restituisce true se inserisce e false se non inserisce
            //auto  dice al programma di capire autonomamente il tipo
        }
    }
    file.close();
    return true;

}
bool ImportCell1Ds (const string& filename, PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);

    if (file.fail())
    { cerr << "File containing Cell1Ds could not be opened. " << endl;
        return false; }

    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        replace(line.begin(), line.end(), ';', ' ');
        listLines.push_back(line);
    }

    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }
    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        converter >> id >> marker >> vertices(0) >> vertices(1);

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);

        if (marker != 0)
        {
            // if (mesh.Cell0DMarkers.find(marker) == mesh.Cell0DMarkers.end())
            //     mesh.Cell0DMarkers.insert({marker, {id}});
            // else
            //     mesh.Cell0DMarkers[marker].push_back(id);

            auto ret = mesh.Cell1DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first) -> second.push_back(id);
        }
    }
    file.close();
    return true;
}
bool ImportCell2Ds (const string& filename, PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);

    if (file.fail())
    { cerr << "File containing Cell2Ds could not be opened. " << endl;
        return false; }

    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        replace(line.begin(), line.end(), ';', ' ');
        listLines.push_back(line);
    }

    listLines.pop_front();

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }
    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        unsigned int id;
        unsigned int marker;
        unsigned int numvertices;

        converter >> id >> marker >> numvertices;    // fermo lo stream al terzo elemento: esso mi dà il numero di vertici del poligono e così posso creare gli altri oggetti che mi servono
        vector<unsigned int> vertices(numvertices);

        for (unsigned int i = 0; i < numvertices; i++)
            converter >> vertices[i];

        unsigned int numedges;

        converter >> numedges;
        vector<unsigned int> edges(numedges);

        for (unsigned int j = 0; j < numedges; j++)
            converter >> edges[j];

        mesh.Cell2DId.push_back(id);
        mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DEdges.push_back(edges);

        if (marker != 0)
        {
            // if (mesh.Cell0DMarkers.find(marker) == mesh.Cell0DMarkers.end())
            //     mesh.Cell0DMarkers.insert({marker, {id}});
            // else
            //     mesh.Cell0DMarkers[marker].push_back(id);

            auto ret = mesh.Cell2DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first) -> second.push_back(id);
        }
    }
    file.close();
    return true;
}

void CheckEdgesNonDegeneracy (PolygonalMesh& mesh, double tol)
{
    for (unsigned int i = 0; i < mesh.NumberCell1D; i++ )
    {
        const unsigned int origin = mesh.Cell1DVertices[i][0];
        const unsigned int end = mesh.Cell1DVertices[i][1];
        const double originX = mesh.Cell0DCoordinates[origin][0];
        const double originY = mesh.Cell0DCoordinates[origin][1];
        const double endX = mesh.Cell0DCoordinates[end][0];
        const double endY = mesh.Cell0DCoordinates[end][1];
        double distance = sqrt(pow(originX - endX, 2) + pow(originY - endY, 2));
        if (distance < tol)
        {
            cout << "Edge " << i << " has lenght less than the tollerance" << endl;
        }
    }

}

void CheckAreasNonDegeneracy (PolygonalMesh& mesh, double tol)
{
    tol = pow(tol, 2);
    for (unsigned int f = 0; f < mesh.NumberCell2D; f++)
    {

        vector<unsigned int> vertices = mesh.Cell2DVertices[f];
        unsigned int numvertices = vertices.size();

        double area = 0;
        for ( unsigned int i=0; i< numvertices -1 ; i++  )
        {
            double x = mesh.Cell0DCoordinates[vertices[i]][0];
            double y = mesh.Cell0DCoordinates[vertices[i]][1];

            double xneighbor = mesh.Cell0DCoordinates[vertices[i+1]][0];
            double yneighbor = mesh.Cell0DCoordinates[vertices[i+1]][1];

            area += x*yneighbor - xneighbor*y;

        }

        double x = mesh.Cell0DCoordinates[vertices[numvertices-1]][0];
        double y = mesh.Cell0DCoordinates[vertices[numvertices-1]][1];

        double xneighbor = mesh.Cell0DCoordinates[vertices[0]][0];
        double yneighbor = mesh.Cell0DCoordinates[vertices[0]][1];

        area += x*yneighbor - xneighbor*y;

        area = abs(area)/2;

        cout << "Area of polygon" << f << " = " << area << endl;;
        if (area < tol)
        {
            cout << "The figure" << f << "has area less than tolerance" << endl;
        }
    }

}

}
