#ifndef DE_H
#define DE_H


#include <vector>
#include <memory>
#include <random>
#include <utility> 
#include "ChristmasTree.h"
#include "evaluation.h"
#include "GreedyPacker.h" 
class SpatialHash;

class DE {

private:
    
    //parametry de
    int population_size;
    int num_trees; // liczba choinek
    int max_iterations;
    float F; // współczynnik mutacji
    float CR; // współczynnik krzyżowania
    
    std::vector<std::vector<float>> population;
    std::vector<float> fitness;
    std::vector<float> best_individual;
    std::vector<float> optimized_angles; // bede zapisywac kolejne zoptymalizowane kąty
    std::vector<bool> frozen_genes;        // Które kąty są zoptymalizowane
    std::vector<double> config_scores;

    float best_fitness;

    std::mt19937 rng;
    std::uniform_real_distribution<float> uniform_dist;


    std::vector<std::vector<std::pair<double, double>>> frozen_positions; // do przechowywania wspórzednych z greedy
    std::vector<std::vector<double>> iteration_scores;
    std::vector<std::vector<double>> iteration_worst_scores; 
    std::vector<std::vector<double>> iteration_avg_scores;    
    int num_configs;

    std::map<int, std::vector<std::pair<double, double>>> temporary_backup_positions;

    GreedyPacker greedy;

    std::vector<std::vector<double>> best_angles_per_config;  
    std::vector<std::vector<std::pair<double, double>>> best_positions_per_config;  
    std::vector<double> best_fitness_per_config;                    

    int iterations_without_improvement;
    double last_best_fitness;
    const double improvement_threshold = 1e-6;

    std::vector<float> mutant_buffer;
    std::vector<float> child_buffer;
    std::vector<ChristmasTree> trees_cache;




public:

    DE(int population_size, int num_trees, int max_iterations, 
       double F, double CR);
    
    void initializePopulationWithWindow(int current_config, int window_start, int window_size, const std::vector<std::vector<float>>& greedy_angles);
    int getWindowSize(int current_config);
    float  evaluateFitnessForConfig(const std::vector<float >& angles, int config);
    void iterationWithWindow(int current_config, int window_start, int window_size);
    std::vector<float>& mutateWindow(int parent_id, int window_start, int window_end);
    std::vector<float>& crossoverWindow(const std::vector<float>& parent,const std::vector<float>& mutant,int window_start,int window_end);


    std::vector<float> run(int num_configs,  const std::vector<std::vector<float >>& greedy_angles = std::vector<std::vector<float>>());


    // Gettery
    double getBestFitness() const { return best_fitness; }
    const std::vector<float>& getBestIndividual() const { return best_individual;    }
    const std::vector<float>& getOptimizedAngles() const { return optimized_angles; }
    const std::vector<double>& getConfigScores() const { return config_scores; }
    const std::vector<std::vector<std::pair<double, double>>>& getFrozenPositions() const { return frozen_positions; }
    const std::vector<std::vector<double>>& getIterationScores() const {return iteration_scores;}
    const std::vector<std::vector<double>>& getBestAnglesPerConfig() const {return best_angles_per_config;}
    
    const std::vector<std::vector<std::pair<double, double>>>& getBestPositionsPerConfig() const {return best_positions_per_config;}
    
    const std::vector<double>& getBestFitnessPerConfig() const {return best_fitness_per_config;}
    const std::vector<std::vector<double>>& getIterationWorstScores() const {return iteration_worst_scores;}

    const std::vector<std::vector<double>>& getIterationAvgScores() const {return iteration_avg_scores;}


    void setFrozenPositions(const std::vector<std::vector<std::pair<double, double>>>& positions);

    // funckje dodatkowe
    double denormalizeAngle(double normalized) const {
        return normalized * 360.0;
    }
    
    void repackIndividual(std::vector<float>& individual_angles,int current_config,int window_start);
    void restoreOldPositions(int current_config);


};
#endif