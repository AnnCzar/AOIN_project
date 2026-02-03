#ifndef GREEDY_PACKER_H
#define GREEDY_PACKER_H

#include "ChristmasTree.h"
#include <vector>
#include <memory>

#include "SpatialHash.h"

class GreedyPacker {
public:

    std::pair<std::vector<std::shared_ptr<ChristmasTree>>, std::vector<double>> 
    packTrees(int num_trees);
     double packWithFixedAngles(const std::vector<double>& normalized_angles);
    double packWithFixedAnglesWindow(
    const std::vector<float>& normalized_angles,
    int current_config,
    int window_start,
    const std::vector<std::vector<std::pair<double, double>>>& frozen_positions,
    std::vector<std::pair<double, double>>& updated_positions);

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

    double tryMoveCloserOptimizedDE(
        const std::shared_ptr<ChristmasTree>& tree,
        const std::vector<std::shared_ptr<ChristmasTree>>& placed,
        const SpatialHash& spatial_hash);



    

   
};

#endif