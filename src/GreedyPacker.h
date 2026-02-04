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
    std::pair<std::vector<std::shared_ptr<ChristmasTree>>, std::vector<double>> 
    packTreesWithAngles(int num_trees, std::vector<float>& angles);

    double packWithFixedAngles(const std::vector<double>& normalized_angles);
    double packWithFixedAnglesWindow(
    const std::vector<float>& normalized_angles,
    int current_config,
    int window_start,
    const std::vector<std::vector<std::pair<double, double>>>& frozen_positions,
    std::vector<std::pair<double, double>>& updated_positions);


    std::pair<std::vector<std::shared_ptr<ChristmasTree>>, std::vector<double>>
    packTreesWithFixedAnglesSA(const std::vector<double>& angles);
    struct PackedResult {
        std::vector<std::shared_ptr<ChristmasTree>> trees;
        double square_side;
    };
    

private:
    double calculateGlobalSquareSide(
        const std::vector<std::shared_ptr<ChristmasTree>>& placed,

        const std::shared_ptr<ChristmasTree>& new_tree);

    double tryMoveCloserOptimizedDE(
        const std::shared_ptr<ChristmasTree>& tree,
        const std::vector<std::shared_ptr<ChristmasTree>>& placed,
        const SpatialHash& spatial_hash);


        std::pair<double, double> findBestPosition(
        const std::shared_ptr<ChristmasTree>&,
        const std::vector<std::shared_ptr<ChristmasTree>>&);
    double findBestAngle(
        const std::shared_ptr<ChristmasTree>&,
        const std::vector<std::shared_ptr<ChristmasTree>>&);
     double tryMoveCloser(
        const std::shared_ptr<ChristmasTree>&,
        const std::vector<std::shared_ptr<ChristmasTree>>&,
        const SpatialHash& spatial_hash); 
                                

};

#endif