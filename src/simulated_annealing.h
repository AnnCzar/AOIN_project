#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include "structures.h"
#include "evaluation.h"
#include "GreedyPacker.h"
#include <vector>
#include <memory>
#include <string>

std::vector<SubmissionRow> convert_to_submission(
    const TreeSolution& solution, 
    const std::string& group_name);

double evaluate_tree_solution(const TreeSolution& solution, 
                              Evaluation& eval, 
                              const std::string& group_name);
                       
TreeSolution local_search(const TreeSolution& solution, int num_trees, 
                         Evaluation& eval, std::vector<double>& deltas, 
                         const std::string& filename);


TreeSolution local_search_greedy(const TreeSolution& solution, int num_trees, 
                         Evaluation& eval, std::vector<double>& deltas, 
                         const std::string& filename);                         

TreeSolution simulated_annealing(const TreeSolution& start_solution, 
                                int num_trees, 
                                double T0, 
                                double alpha, 
                                int max_iterations, 
                                const std::string& filename,
                                int neighborhood_size = 5);
                                   
                                
vector<TreeSolution> createNeighborhood(const TreeSolution& current, 
                                       double sigma, 
                                       int neighborhood_size,
                                       int greedy_frequency,
                                       int iteration,
                                       GreedyPacker& packer);

TreeSolution getBestNeighbor(vector<TreeSolution>& neighborhood, 
                            Evaluation& eval,
                            const string& filename);
                                
#endif