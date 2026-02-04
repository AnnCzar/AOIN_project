#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>
#include <string>

struct TreeSolution {
    std::vector<double> positions_x;
    std::vector<double> positions_y;
    std::vector<double> angles;
    double fitness;

    TreeSolution() : fitness(1e9) {}
};


struct SubmissionRow {
    std::string id;
    double x;
    double y;
    double deg;

    SubmissionRow() : x(0.0), y(0.0), deg(0.0) {}
};


struct Individual {
    std::vector<int> permutation;
    double fitness;
};


#endif 