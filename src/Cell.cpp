#include "Cell.h"
#include "obstacle.h"

using namespace std;

Cell::Cell()
{}

Cell::~Cell()
{}

Cell::Cell(const Cell &cell): Obstacle(){
    component_types = cell.component_types;
    component_type_to_index = cell.component_type_to_index;
    number_of_components_per_type = cell.number_of_components_per_type;
    components = cell.components;
    type = cell.type;
    /*percolation = cell.percolation;
    diffusivity_i = cell.diffusivity_i;
    diffusivity_e = cell.diffusivity_e;*/
}

int Cell::getComponentTypeIndex(
    const string &component_type, 
    const int &component_index
){
    return component_type_to_index[component_type] + component_index;
}

