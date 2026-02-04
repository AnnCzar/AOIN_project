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

    double packWithFixedAnglesWindow(
    const std::vector<float>& normalized_angles,
    int current_config,
    int window_start,
    const std::vector<std::vector<std::pair<double, double>>>& frozen_positions,
    std::vector<std::pair<double, double>>& updated_positions);

    // SA -- odkomentowac  --
    // std::pair<std::vector<std::shared_ptr<ChristmasTree>>, std::vector<double>>
    // packTreesWithFixedAnglesSA(const std::vector<double>& angles);
    // struct PackedResult {
    //     std::vector<std::shared_ptr<ChristmasTree>> trees;
    //     double square_side;
    // };
    // SA -- odkomentowac  -- koniec
    

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


    // SA -- odkomentowac
     // double tryMoveCloser(
     //    const std::shared_ptr<ChristmasTree>&,
     //    const std::vector<std::shared_ptr<ChristmasTree>>&,
     //    const SpatialHash& spatial_hash);
    // SA -- odkomentowac  -- koniec


    // DE -- odkomentowac
    double tryMoveCloser(const std::shared_ptr<ChristmasTree>& new_tree,
                 const std::vector<std::shared_ptr<ChristmasTree>>& placed);

    // DE -- odkomentowac  -- koniec

};

#endif