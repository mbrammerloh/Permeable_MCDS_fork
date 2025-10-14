#include "sphere.h"
#include "parameters.h"
#include <vector>
#include <cstdint>
#include <cstddef>
#include "CellComponent.h"

using namespace std;


struct SphereIds {
    int cell_type;// int according to the cell types in the vector stored in the substrate
    int cell_id;
    int component_type; // int, similar to cell_type
    int component_id;
    int sphere_id; // vector index of spheres
};

// Exact, order-sensitive equality
inline bool operator==(const SphereIds& a, const SphereIds& b) {
    return a.cell_type   == b.cell_type
        && a.cell_id == b.cell_id
        && a.component_type== b.component_type
        && a.component_id   == b.component_id
        && a.sphere_id    == b.sphere_id;
}

class SphereMap {
public:
    static constexpr int N = 5;
    vector<double> x0;

    SphereMap();
    SphereMap(const SphereMap &spm);
    SphereMap(
        const vector<double> &voxel_size, 
        const vector<double> &x0
    );
    SphereMap(
        double vx, double vy, double vz,
        double x0, double y0, double z0
    );

    // initialize the sphere map with a given voxel size.
    void init(
        vector<double>& vox_sizes,
        vector<double>& x0_origin
    );

    // Add a sphere to the SphereMap. If no_duplicates is true,
    // the exact same SphereIds will not be added twice in the same entry.
    bool add_sphere_to_entry(int x, int y, int z, const SphereIds& ids, bool no_duplicates = true);

    // Convenience overload with named fields
    bool add_sphere_to_entry(int x, int y, int z,
                    int cell_type,
                    int cell_id,
                    int component_type,
                    int component_id,
                    int sphere_id,
                    bool no_duplicates = true);

    // Remove a sphere by exact value (order-sensitive). Returns true if removed.
    bool remove_sphere(int x, int y, int z, const SphereIds& ids);

    // Check if a sphere exists at an entry (order-sensitive).
    bool contains_sphere(int x, int y, int z, const SphereIds& ids) const;

    // Number of spheres at an entry.
    size_t sphere_count(int x, int y, int z) const;

    // Access all spheres at an entry.
    const vector<SphereIds>& spheres(int x, int y, int z) const;
    vector<SphereIds>&       spheres(int x, int y, int z);

    // Utilities per entry.
    void clear_entry(int x, int y, int z);
    void reserve_entry(int x, int y, int z, size_t n_spheres);
    
    // Add sphere object to SphereMap
    bool add_sphere(Sphere& sphere,  SphereIds& ids);
    // Add sphere object to Spheremap, with named ids 
    bool add_sphere(
        Sphere& sphere,  
        int cell_type, 
        int cell_id, 
        int component_type, 
        int component_id, 
        int sphere_id);

    // Returns a vectors of sphere IDs that potentially overlap with a given sphere.
    vector<SphereIds> get_potentially_overlapping_spheres_ids(Sphere & sphere);
    vector<int> get_number_of_entries(Sphere & sphere);


private:
    struct Entry {
        vector<SphereIds> spheres;
    };

    static size_t lin(int x, int y, int z);

    const vector<SphereIds>&   entry_spheres(int x, int y, int z) const;
    vector<SphereIds>&         entry_spheres(int x, int y, int z);

    vector<int> get_index_of_point(Eigen::Vector3d & point);
    bool is_sphere_in_map_voxel(vector<int>& indices, Eigen::Vector3d& sphere_center, double & radius);

    vector<Entry> entries_;
    vector<double> voxel_size_;
    vector<double> step_size_; // defines step size of grid in µm in x, y, z directions
};