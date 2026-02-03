#ifndef SPATIAL_HASH_H
#define SPATIAL_HASH_H

#include <vector>
#include <unordered_map>
#include <memory>
#include "ChristmasTree.h"

class SpatialHash {
public:
     SpatialHash(double cell_size = 15.0) : cell_size_(cell_size) {}
    
    void insert(const std::shared_ptr<ChristmasTree>& tree, int index) {
        int cell_x = (int)std::floor(tree->getX() / cell_size_);
        int cell_y = (int)std::floor(tree->getY() / cell_size_);
        
        uint64_t cell_key = make_key(cell_x, cell_y);
        cells_[cell_key].push_back(index);
        
        tree_cells_[index] = {cell_x, cell_y};
    }
    
    std::vector<int> get_potential_collisions(const std::shared_ptr<ChristmasTree>& tree) const {
        std::vector<int> candidates;
        
        int cell_x = (int)std::floor(tree->getX() / cell_size_);
        int cell_y = (int)std::floor(tree->getY() / cell_size_);
        
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                uint64_t cell_key = make_key(cell_x + dx, cell_y + dy);
                
                auto it = cells_.find(cell_key);
                if (it != cells_.end()) {
                    candidates.insert(candidates.end(), it->second.begin(), it->second.end());
                }
            }
        }
        
        return candidates;
    }
    
    void clear() {
        cells_.clear();
        tree_cells_.clear();
    }
    
    size_t num_cells() const { return cells_.size(); }
    
    
    // srednia liczba choinek na komórkę 
    double avg_trees_per_cell() const {
        if (cells_.empty()) return 0.0;
        
        size_t total = 0;
        for (const auto& [key, trees] : cells_) {
            total += trees.size();
        }
        return (double)total / cells_.size();
    }
    
private:
    double cell_size_;  

    std::unordered_map<uint64_t, std::vector<int>> cells_;
    
    std::unordered_map<int, std::pair<int, int>> tree_cells_;
    // unikalny klucz dla komorki
    uint64_t make_key(int x, int y) const {
        uint32_t ux = (uint32_t)(x + 1000);
        uint32_t uy = (uint32_t)(y + 1000);
        
        return ((uint64_t)ux << 32) | uy;
    }
};

#endif 