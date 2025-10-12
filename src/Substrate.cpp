#include "Substrate.h"
#include "CellComponent.h"
#include <limits>
#include <cmath>
#include <limits>
#include <algorithm> // for std::min initializer_list
#include <unordered_set>
#include <unordered_map>


using namespace std;

Substrate::Substrate()
{}

Substrate::~Substrate()
{}

int Substrate::getCellTypeIndex(
    const string &cell_type, 
    const int &cell_index
){
    return cell_type_to_index[cell_type] + cell_index;
}

void Substrate::fillSphereMap(){
    for (int i_cell_type = 0; i_cell_type < int(cells.size()); i_cell_type ++){
        for (int i_cell = 0; i_cell < int(number_of_cells_per_type[i_cell_type]); i_cell++){
            int index_cell = cell_type_to_index[cell_types[i_cell_type]] + i_cell; 
            Cell & this_cell = cells[index_cell];
            for (int i_component_type = 0; i_component_type < int(this_cell.component_types.size()); i_component_type ++){
                for (int i_component = 0; i_component < this_cell.number_of_components_per_type[i_component_type]; i_component ++) {
                    int index_component = this_cell.component_type_to_index[this_cell.component_types[i_component_type]] + i_component; 
                    CellComponent & this_component = this_cell.components[index_component];
                    for (int i_sphere = 0; i_sphere < int(this_component.spheres.size()); i_sphere ++){
                        Sphere & this_sphere = this_component.spheres[i_sphere];
                        spheremap.add_sphere(
                            this_sphere,
                            i_cell_type,
                            index_cell,
                            i_component_type,
                            index_component,
                            i_sphere
                        );
                    }
                }
            }
        }
    }
}