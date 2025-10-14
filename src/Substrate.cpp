#include "Substrate.h"
#include "CellComponent.h"
#include "obstacle.h"
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

Substrate::Substrate(const Substrate &sub): Obstacle(){
    cell_types = sub.cell_types;
    cell_type_to_index = sub.cell_type_to_index;
    number_of_cells_per_type = sub.number_of_cells_per_type;
    cells = sub.cells;
    spheremap = sub.spheremap;
    /*percolation = sub.percolation;
    diffusivity_i = sub.diffusivity_i;
    diffusivity_e = sub.diffusivity_e;*/
}

int Substrate::getCellTypeIndex(
    const string &cell_type, 
    const int &cell_index
){
    return cell_type_to_index[cell_type] + cell_index;
}

void Substrate::createSphereMap(){
    double xyz_min, xyz_max;
    // Find the voxel size by looking for maximum and minimum extension in all directions 
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
                        for (int i_dim = 0; i_dim < 3; i_dim ++){
                            // initialize bounds by first sphere
                            if (i_cell_type == 0 
                                && i_cell == 0 
                                && i_component_type == 0 
                                && i_component == 0
                                && i_sphere == 0 
                                && i_dim == 0
                            ){
                                xyz_min = this_sphere.center[0];
                                xyz_max = this_sphere.center[0];
                            }
                            double min_value = this_sphere.center[i_dim] - this_sphere.radius;
                            double max_value = this_sphere.center[i_dim] + this_sphere.radius;
                            if (min_value < xyz_min){
                                xyz_min = min_value;
                            }
                            if (max_value > xyz_max){
                                xyz_max = max_value;
                            }
                        }
                    }
                }
            }
        }
    }
    // get voxel size and origin (x0 from the analysis above to initialize sphere map.
    vector<double> voxel_size {xyz_max-xyz_min,xyz_max-xyz_min,xyz_max-xyz_min};
    vector<double> x0 {xyz_min,xyz_min,xyz_min};
    spheremap.init(voxel_size, x0);

    // Fill the sphere map
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