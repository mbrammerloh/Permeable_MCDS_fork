#include "SphereMap.h"
#include "sphere.h"
#include "CellComponent.h"

#include <algorithm>
#include <cassert>
#include <vector>
#include "Eigen/Dense"



SphereMap::SphereMap()
    : entries_(static_cast<std::size_t>(N) * N * N),
      voxel_size_{1.0, 1.0, 1.0},
      step_size_{1.0/N, 1.0/N, 1.0/N} {}

SphereMap::SphereMap(const std::vector<double>& voxel_size)
    : entries_(static_cast<std::size_t>(N) * N * N),
      voxel_size_(voxel_size),
      step_size_{voxel_size[0]/N, voxel_size[1]/N, voxel_size[2]/N} {
    assert(voxel_size_.size() == 3);
}

SphereMap::SphereMap(double vx, double vy, double vz)
    : entries_(static_cast<std::size_t>(N) * N * N),
      voxel_size_{vx, vy, vz},
      step_size_{vx/N, vy/N, vz/N} {}

void SphereMap::init(std::vector<double>& vox_sizes){
    for (int i = 0; i < 3; i++){
        voxel_size_[i] = vox_sizes[i];
        step_size_[i] = vox_sizes[i]/N; 
    }
}

bool SphereMap::add_sphere_to_entry(int x, int y, int z, const SphereIds& ids, bool no_duplicates) {
    auto& v = entry_spheres(x, y, z);
    if (no_duplicates) {
        if (std::find(v.begin(), v.end(), ids) != v.end()) return false;
    }
    v.push_back(ids);
    return true;
}

bool SphereMap::add_sphere_to_entry(int x, int y, int z,
                           int cell_type,
                           int cell_id,
                           int component_type,
                           int component_id,
                           int sphere_id,
                           bool no_duplicates) {
    SphereIds ids{cell_type, cell_id, component_type, component_id, sphere_id};
    return add_sphere_to_entry(x, y, z, ids, no_duplicates);
}
/*
bool SphereMap::remove_sphere(int x, int y, int z, const SphereIds& ids) {
    auto& v = entry_spheres(x, y, z);
    auto it = std::find(v.begin(), v.end(), ids);
    if (it == v.end()) return false;
    *it = std::move(v.back());  // swap-pop (does not preserve vector order)
    v.pop_back();
    return true;
}

bool SphereMap::contains_sphere(int x, int y, int z, const SphereIds& ids) const {
    const auto& v = entry_spheres(x, y, z);
    return std::find(v.begin(), v.end(), ids) != v.end();
}

std::size_t SphereMap::sphere_count(int x, int y, int z) const {
    return entry_spheres(x, y, z).size();
}*/

const std::vector<SphereIds>& SphereMap::spheres(int x, int y, int z) const {
    return entry_spheres(x, y, z);
}

std::vector<SphereIds>& SphereMap::spheres(int x, int y, int z) {
    return entry_spheres(x, y, z);
}
 
/*
void SphereMap::clear_entry(int x, int y, int z) {
    entry_spheres(x, y, z).clear();
}

void SphereMap::reserve_entry(int x, int y, int z, std::size_t n_spheres) {
    entry_spheres(x, y, z).reserve(n_spheres);
}*/

std::size_t SphereMap::lin(int x, int y, int z) {
    assert(0 <= x && x < N);
    assert(0 <= y && y < N);
    assert(0 <= z && z < N);
    return static_cast<std::size_t>(x)
         + static_cast<std::size_t>(N) * (static_cast<std::size_t>(y)
                                          + static_cast<std::size_t>(N) * static_cast<std::size_t>(z));
}

const std::vector<SphereIds>& SphereMap::entry_spheres(int x, int y, int z) const {
    return entries_[lin(x, y, z)].spheres;
}

std::vector<SphereIds>& SphereMap::entry_spheres(int x, int y, int z) {
    return entries_[lin(x, y, z)].spheres;
}

bool SphereMap::add_sphere(Sphere& sphere,  SphereIds& ids){
    bool success = true;

    std::vector<int> number_of_entries = get_number_of_entries(sphere);

    const std::vector<int> index_center = get_index_of_point(sphere.center);

    for (           int i = index_center[0]-number_of_entries[0]; i <= index_center[0]+number_of_entries[0]; i++){
        for (       int j = index_center[1]-number_of_entries[1]; j <= index_center[1]+number_of_entries[1]; j++){
            for (   int k = index_center[2]-number_of_entries[2]; k <= index_center[2]+number_of_entries[2]; k++){  
                if (i>=0 && i < N && j>=0 && j < N && k>=0 && k < N){
                    std::vector<int> indices = {i,j,k};
                    if (is_sphere_in_map_voxel(indices,sphere.center,sphere.radius)){
                        success = success && add_sphere_to_entry(i,j,k, ids);
                    }
                }
            }
        }
    }
    return success;
}

bool SphereMap::add_sphere(
    Sphere& sphere,  
    int cell_type, 
    int cell_id, 
    int component_type, 
    int component_id, 
    int sphere_id
){
    SphereIds sph_ids;
    sph_ids.cell_type = cell_type;
    sph_ids.cell_id = cell_id;
    sph_ids.component_type = component_type;
    sph_ids.component_id = component_id;
    sph_ids.sphere_id = sphere_id;
    return add_sphere(sphere,sph_ids);
}


std::vector<int> SphereMap::get_number_of_entries(Sphere & sphere){
    std::vector<int> number_of_entries;
    for (int i = 0; i < 3; i++){
        number_of_entries.push_back(int(std::round(sphere.radius / step_size_[i]))+1); // the number of entries to test in each direction from the sphere center, +1 for safety / limiting cases
    }
    return number_of_entries;
}

std::vector<SphereIds> SphereMap::get_potentially_overlapping_spheres_ids(Sphere & sphere){
    std::vector<SphereIds> sphere_ids;

    std::vector<int> number_of_entries = get_number_of_entries(sphere);
    const std::vector<int> index_center = get_index_of_point(sphere.center);

    for (           int i = index_center[0]-number_of_entries[0]; i <= index_center[0]+number_of_entries[0]; i++){
        for (       int j = index_center[1]-number_of_entries[1]; j <= index_center[1]+number_of_entries[1]; j++){
            for (   int k = index_center[2]-number_of_entries[2]; k <= index_center[2]+number_of_entries[2]; k++){  
                if (i>=0 && i < N && j>=0 && j < N && k>=0 && k < N){
                    auto these_sphere_ids = entry_spheres(i, j, k);
                    for (uint l = 0; l < these_sphere_ids.size(); l++){
                        // Check that SphereID was not yet added
                        if (!(std::find(sphere_ids.begin(), sphere_ids.end(), these_sphere_ids[l]) != sphere_ids.end())) {
                            sphere_ids.push_back(these_sphere_ids[l]);
                        }
                    }
                }
            }
        }
    }
    return sphere_ids;
}

inline float squared(float v) { return v * v; }

bool SphereMap::is_sphere_in_map_voxel(std::vector<int> & indices, Eigen::Vector3d& sphere_center, double & radius)
{
    Eigen::Vector3d cube_corner0;
    Eigen::Vector3d cube_corner1;

    for (int i = 0; i < 3; i++){
        cube_corner0[i] = indices[i] * step_size_[i];
        cube_corner1[i] = (indices[i]+1) * step_size_[i];
    }

    float dist_squared = radius*radius;
    /* assume C1 and C2 are element-wise sorted, if not, do that now */
    if      (sphere_center[0] < cube_corner0[0]) dist_squared -= squared(sphere_center[0] - cube_corner0[0]);
    else if (sphere_center[0] > cube_corner1[0]) dist_squared -= squared(sphere_center[0] - cube_corner1[0]);

    if      (sphere_center[1] < cube_corner0[1]) dist_squared -= squared(sphere_center[1] - cube_corner0[1]);
    else if (sphere_center[1] > cube_corner1[1]) dist_squared -= squared(sphere_center[1] - cube_corner1[1]);

    if      (sphere_center[2] < cube_corner0[2]) dist_squared -= squared(sphere_center[2] - cube_corner0[2]);
    else if (sphere_center[2] > cube_corner1[2]) dist_squared -= squared(sphere_center[2] - cube_corner1[2]);

    return dist_squared > 0;
}


std::vector<int> SphereMap::get_index_of_point(Eigen::Vector3d & point)
{
    std::vector<int> indices;
    for (int i = 0; i < 3; i++){
        indices.push_back(point[i] / step_size_[i]);
    }
    return indices;
}

