#include "Cell.h"

using namespace std;

Cell::Cell()
{}

Cell::~Cell()
{}

int Cell::getComponentTypeIndex(
    const string &component_type, 
    const int &component_index
){
    return component_type_to_index[component_type] + component_index;
}

