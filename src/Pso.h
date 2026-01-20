#ifndef PSO_H
#define PSO_H

#include "ChristmasTree.h"
#include "PsoParticle.h"
#include "GreedyPacker.h"
#include <vector>
#include <memory>
#include <random>


class Pso{

public:
    Pso(double inertia_weight, double inertia_weight_max,
            double inertia_weight_min, double acc_c_1, double acc_c_2, 
            int number_of_particles, int number_of_iterations, GreedyPacker packer, int seed);


    std::vector<float> algorithm(int num_trees);
    std::vector<float> algorithmImproved(int num_trees, double P);
    std::vector<std::vector<float>> runForAllTreeCounts(int maxTrees);


    int number_of_particles = 20; 
    std::vector<float> globalBest;
    std::vector<float> globalSecondBest;
    float globalBestScore;
    float globalSecondBestScore;
    double inertia_weight;
    double inertia_weight_max = 0.9;
    double inertia_weight_min = 0.4;
    double acc_c_1 = 1.49445;
    double acc_c_2 = 1.49445;
    double acc_c_3 = 1.9;
    int number_of_iterations = 100;
    
    void updateParticlePosition(PsoParticle& particle, std::vector<float>& position);
    void updateParticleVelocity(PsoParticle& particle, std::vector<float>& position);
    void updateParticlePersonalBest(PsoParticle& particle, std::vector<float>& position);
    void updateGlobalBest(std::vector<float>& position, float globalBestScore);


    double evaluateFitness(int num_trees, std::vector<float>& position);

    std::vector<float> calculateNewPosition(PsoParticle& particle);
    std::vector<float> calculateNewVelocity(PsoParticle& particle);
    std::vector<float> calculateNewVelocityImproved(PsoParticle& particle);
    void updateInertiaWeight(int iteration);

private:
   PsoParticle initializeParticle(int num_trees);
   std::mt19937 rng;
   GreedyPacker packer;


};

#endif