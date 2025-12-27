#ifndef GREEDY_PACKER_H
#define GREEDY_PACKER_H

#include "ChristmasTree.h"
#include <vector>
#include <memory>



class GreedyPacker {
public:

    std::pair<std::vector<std::shared_ptr<ChristmasTree>>, std::vector<double>> 
    packTrees(int num_trees);

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