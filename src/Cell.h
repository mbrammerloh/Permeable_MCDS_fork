//!  Cell class =============================================================/
/*!
*   \details   Class that defines a cell
*   \author    Malte Brammerloh
*   \date      Oct 2025
*   \version   0.1
=================================================================================================*/

#ifndef CELL_H
#define CELL_H

#include "CellComponent.h"
#include <string>
#include <vector>
#include <map>

using namespace std;

class Cell: public Obstacle
{
    public:
        vector<string> component_types;
        map<string, int> component_type_to_index;
        vector <int> number_of_components_per_type;
        vector<CellComponent> components;
        string type;

        Cell();
        ~Cell();

        int getComponentTypeIndex(
            const string &component_type, 
            const int &component_index
        );
};

#endif