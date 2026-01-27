#ifndef TABU_H
#define TABU_H

#include "ChristmasTree.h"
#include "GreedyPacker.h"
#include <vector>
#include <memory>
#include <random>



struct TabuMetrics {
    float globalBestScore;
    float iterationBestScore;
    float globalWorstScore;

    TabuMetrics(double globalBestScore, double iterationBestScore, double globalWorstScore) 
        : globalBestScore(globalBestScore), iterationBestScore(iterationBestScore), globalWorstScore(globalWorstScore) {}
};


class Tabu{

public:
    Tabu(int numberOfIterations, int tabuListLength, int neighborhoodSize, float ballRadius, GreedyPacker& packer, int seed);



    int tabuListLength; 
    std::vector<float> bestSolution;
    float bestScore;
    float ballRadius;
    float globalBestScore;
    float globalWorstScore;
    int number_of_iterations;
    int neighborhoodSize;

    std::vector<std::vector<float>> createNeighborhood(std::vector<float>& currentSolution, float sigma);
    std::pair<std::vector<float>,float> getBestNeighbor(std::vector<std::vector<float>>& neighborhood, std::vector<std::vector<float>>& tabuList,
                                                                float bestScore, int numberOfTrees);

    bool checkAspirationCriteria();
    bool doesTabuContain(std::vector<std::vector<float>>& tabuList, std::vector<float>& solution);

    std::pair<std::vector<float>,float> algorithm(int numberOfTrees, float sigma);
    std::vector<TabuMetrics> algorithmWithScores(int numberOfTrees, float sigma);


    float evaluateFitness(std::vector<float>& position);
    std::vector<float> rotate_tree(const std::vector<float>& angles, float sigma);


private:
   std::mt19937 rng;
   GreedyPacker packer;


};

#endif