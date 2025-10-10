#include "sphere.h"
#include "parameters.h"
#include <vector>
#include <cstdint>
#include <cstddef>
#include "CellComponent.h"


struct SphereIds {
    int neuron_id;
    int dendrite_id;
    int subbranch_id;
    int sphere_id;
    int spine_index;
};

// Exact, order-sensitive equality
inline bool operator==(const SphereIds& a, const SphereIds& b) {
    return a.neuron_id   == b.neuron_id
        && a.dendrite_id == b.dendrite_id
        && a.subbranch_id== b.subbranch_id
        && a.sphere_id   == b.sphere_id
        && a.spine_index    == b.spine_index;
}

class SphereMap {
public:
    static constexpr int N = 100;

    SphereMap();
    SphereMap(const std::vector<double>& voxel_size);
    SphereMap(double vx, double vy, double vz);

    // initialize the sphere map with a given voxel size.
    void init(std::vector<double>& vox_sizes);

    // Add a sphere to the SphereMap. If no_duplicates is true,
    // the exact same SphereIds will not be added twice in the same entry.
    bool add_sphere_to_entry(int x, int y, int z, const SphereIds& ids, bool no_duplicates = true);

    // Convenience overload with named fields
    bool add_sphere_to_entry(int x, int y, int z,
                    int neuron_id,
                    int dendrite_id,
                    int subbranch_id,
                    int sphere_id,
                    int spine_index,
                    bool no_duplicates = true);

    // Remove a sphere by exact value (order-sensitive). Returns true if removed.
    bool remove_sphere(int x, int y, int z, const SphereIds& ids);

    // Check if a sphere exists at an entry (order-sensitive).
    bool contains_sphere(int x, int y, int z, const SphereIds& ids) const;

    // Number of spheres at an entry.
    std::size_t sphere_count(int x, int y, int z) const;

    // Access all spheres at an entry.
    const std::vector<SphereIds>& spheres(int x, int y, int z) const;
    std::vector<SphereIds>&       spheres(int x, int y, int z);

    // Utilities per entry.
    void clear_entry(int x, int y, int z);
    void reserve_entry(int x, int y, int z, std::size_t n_spheres);

    // Add cell process to SphereMap.
    bool add_process(CellComponent& cell_process, int spine_index);
    
    // Add sphere object to SphereMap
    bool add_sphere(Sphere& sphere,  SphereIds& ids);
    // Add sphere object to Spheremap, with named ids 
    bool add_sphere(Sphere& sphere,  int neuron_id, int dendrite_id, int subbranch_id, int spine_index, int sphere_id);

    // Returns a vectors of sphere IDs that potentially overlap with a given sphere.
    std::vector<SphereIds> get_potentially_overlapping_spheres_ids(Sphere & sphere);
    std::vector<int> get_number_of_entries(Sphere & sphere);


private:
    struct Entry {
        std::vector<SphereIds> spheres;
    };

    static std::size_t lin(int x, int y, int z);

    const std::vector<SphereIds>&   entry_spheres(int x, int y, int z) const;
    std::vector<SphereIds>&         entry_spheres(int x, int y, int z);

    std::vector<int> get_index_of_point(Eigen::Vector3d & point);
    bool is_sphere_in_map_voxel(std::vector<int>& indices, Eigen::Vector3d& sphere_center, double & radius);

    std::vector<Entry> entries_;
    std::vector<double> voxel_size_;
    std::vector<double> step_size_; // defines step size of grid in µm in x, y, z directions
};