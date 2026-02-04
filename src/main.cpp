#include "ChristmasTree.h"
#include <iostream>
#include <iomanip>
#include "GreedyPacker.h"
#include "Pso.h"
#include <fstream>

int main(){


    /// PSO - wyniki dla n=10; FFE = 3k


        int numberOfTrees = 10;
        int runsPerTest = 10;
        int numberOfIterations = 100;
        int numberOfParticles = 30;
        int acc_c_1 = 1.49445;
        int acc_c_2 = 1.49445;
        int acc_c_3 = 1.9;
        double min_inertia = 0.4;
        double max_inertia = 0.9;
        double inertia_weight = 0.7;
        int seed = 1;
        GreedyPacker packer;
        double P = 0.5;




        std::vector<float> bestStandardResults;
        std::vector<float> bestImpResults;
    for (int i = 0; i < runsPerTest; i++) {

        Pso pso(inertia_weight, max_inertia, min_inertia,
            acc_c_1,  acc_c_2,  numberOfParticles,
            numberOfIterations, packer,  i);
        std::vector<float> standardResult = pso.algorithm(numberOfTrees);
        std::vector<float> impResult = pso.algorithmImproved(numberOfTrees, P);

        float resultStandardScore = 0.0;
        for(int j=0; j<standardResult.size(); j++){
            resultStandardScore += (standardResult.at(j)*standardResult.at(j))/(j+1);
        }
        bestStandardResults.push_back(resultStandardScore);
       
       
        float resultImprovedScore = 0.0;
        for(int j=0; j<impResult.size(); j++){
            resultImprovedScore += (impResult.at(j)*impResult.at(j))/(j+1);
        }
        bestImpResults.push_back(resultImprovedScore);
    }   


    std::ofstream stdFile("pso10treesBestScores.csv");
    if (stdFile.is_open()) {
        stdFile << "Run,BestScore\n";
        for (int i = 0; i < bestStandardResults.size(); i++) {
            stdFile << i << "," << bestStandardResults[i] << "\n";
        }
        stdFile.close();
    }

    std::ofstream impFile("psoImp10treesBestScores.csv");
    if (impFile.is_open()) {
        impFile << "Run,BestScore\n"; 
        for (int i = 0; i < bestImpResults.size(); i++) {
            impFile << i << "," << bestImpResults[i] << "\n";
        }
        impFile.close();
    }

    return 0;

}