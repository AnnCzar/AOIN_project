#ifndef OPERATORS_H
#define OPERATORS_H
#include <vector>
#include <random>

std::vector<double> rotate_tree(const std::vector<double>& angles, double sigma=0.5);
std::vector<double> rotate_tree_selective(const std::vector<double>& angles, double sigma, double pm);
                              
#endif