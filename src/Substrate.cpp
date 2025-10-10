#include "Substrate.h"
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