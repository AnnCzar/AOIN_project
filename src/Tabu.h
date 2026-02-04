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

struct TabuMetrics2 {
    int iteration;
    float globalBestScore;
    float iterationBestScore;

    TabuMetrics2(int iteration,double globalBestScore, double iterationBestScore) 
        : iteration(iteration), globalBestScore(globalBestScore), iterationBestScore(iterationBestScore) {}
};

struct TabuResult {
    std::vector<float> bestAngles;
    std::vector<double> history;
    float bestFinalScore;
};

struct TreeData {
    double x;
    double y;
    double angle;
};


struct TabuFullHistory {
    std::vector<TreeData> trees;
    std::vector<TabuMetrics2> scores;
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
    std::vector<std::vector<float>> createNeighborhood2(std::vector<float>& currentSolution, float sigma, int k);

    std::pair<std::vector<float>,float> getBestNeighbor(std::vector<std::vector<float>>& neighborhood, std::vector<std::vector<float>>& tabuList,
                                                                float bestScore, int numberOfTrees);

    bool checkAspirationCriteria();
    bool doesTabuContain(std::vector<std::vector<float>>& tabuList, std::vector<float>& solution);

    TabuResult algorithm(int numberOfTrees, float sigma);
    std::vector<TabuMetrics> algorithmWithScores(int numberOfTrees, float sigma);

    TabuResult algorithmK(int numberOfTrees, float sigma, int k);
    std::vector<TabuMetrics> algorithmWithScoresK(int numberOfTrees, float sigma, int k);
    TabuFullHistory algorithmWithScoresKFinal(int numberOfTrees, float sigma, int k);



    float evaluateFitness(std::vector<float>& position);
    std::vector<float> rotate_tree(const std::vector<float>& angles, float sigma);
    std::vector<float> rotate_k_trees(const std::vector<float>& angles, float sigma, int k);


private:
   std::mt19937 rng;
   GreedyPacker packer;


};

#endif