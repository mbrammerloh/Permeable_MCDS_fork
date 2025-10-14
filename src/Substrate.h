//!  Substrate Obstacle Derived Class =============================================================/
/*!
*   \details   Substrate class derived from an Obstacle
*   \author    Malte Brammerloh
*   \date      Oct 2025
*   \version   0.1
=================================================================================================*/

#ifndef SUBSTRATE_H
#define SUBSTRATE_H

#include "Cell.h"
#include "obstacle.h"
#include <SphereMap.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Substrate: public Obstacle
{
    public:
        vector<string> cell_types;
        map<string, int> cell_type_to_index;
        vector<int> number_of_cells_per_type;
        vector<Cell> cells;
        SphereMap spheremap;

        /*!
        *  \brief Default constructor. Does nothing
        */
        Substrate();
        ~Substrate();
        Substrate(const Substrate &sub);

        int getCellTypeIndex(
            const string &cell_type, 
            const int &cell_index
        );

        string cellTypeFromIndex(const int & index);

        void createSphereMap();
};

#endif