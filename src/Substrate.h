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
        vector<Cell> cells;
        SphereMap spheremap;

        /*!
        *  \brief Default constructor. Does nothing
        */
        Substrate();
        ~Substrate();

        int getCellTypeIndex(
            const string &cell_type, 
            const int &cell_index
        );
};

#endif