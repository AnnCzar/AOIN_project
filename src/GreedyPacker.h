#ifndef GREEDY_PACKER_H
#define GREEDY_PACKER_H

#include "ChristmasTree.h"
#include <vector>
#include <memory>
class SpatialHash;



class GreedyPacker {
public:

    std::pair<std::vector<std::shared_ptr<ChristmasTree>>, std::vector<double>> 
    packTrees(int num_trees);
    std::pair<std::vector<std::shared_ptr<ChristmasTree>>, std::vector<double>>
    packTreesWithFixedAngles(const std::vector<float>& angles);
    struct PackedResult {
        std::vector<std::shared_ptr<ChristmasTree>> trees;
        double square_side;
    };
    std::pair<double, double> findBestPositionOptimized(
        const std::shared_ptr<ChristmasTree>&,
        const std::vector<std::shared_ptr<ChristmasTree>>&,
        const SpatialHash& spatial_hash);
    double findBestAngleOptimized(
        const std::shared_ptr<ChristmasTree>&,
        const std::vector<std::shared_ptr<ChristmasTree>>&,
        const SpatialHash& spatial_hash);
    std::pair<std::vector<std::shared_ptr<ChristmasTree>>, std::vector<double>> packTreesDifferentStart(int num_trees, double sigma);
     double tryMoveCloserOptimized(
        const std::shared_ptr<ChristmasTree>&,
        const std::vector<std::shared_ptr<ChristmasTree>>&,
        const SpatialHash& spatial_hash); 
    
    // PackedResult packTreesWithAngles(const std::vector<double>& angles);

private:

    std::pair<double, double> 
    findBestPosition(const std::shared_ptr<ChristmasTree>& new_tree,
                     const std::vector<std::shared_ptr<ChristmasTree>>& placed);
    double findBestAngle(const std::shared_ptr<ChristmasTree>& new_tree,
                     const std::vector<std::shared_ptr<ChristmasTree>>& placed);
    double tryMoveCloser(const std::shared_ptr<ChristmasTree>& new_tree,
                     const std::vector<std::shared_ptr<ChristmasTree>>& placed);  
                                      
    double calculateGlobalSquareSide(
        const std::vector<std::shared_ptr<ChristmasTree>>& placed,
        const std::shared_ptr<ChristmasTree>& new_tree);
};

#endif