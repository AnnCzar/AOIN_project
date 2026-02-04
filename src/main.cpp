#include "ChristmasTree.h"
#include <iostream>
#include <iomanip>
#include "GreedyPacker.h"
#include "CSVWriter.h"
#include "Pso.h"
#include <chrono>
#include <chrono>
#include <fstream>
#include "Tabu.h"


int main(){


    /// Tabu - wyniki dla n=10,50,100,200; FFE = 3k


        int runsPerTest = 10;
        int numberOfIterations = 200;
        int tabuListLength = 8;
        float ballRadius = 1.8; //10%
        int neighborhoodSize = 15;
        GreedyPacker packer;
        int seed = 9;
        double sigma = 15;
        int numberOfTrees =50;
        int k = 5;
        //k = 10, tLL=12 dla 100
        //k = 5, tLL=8 dla 50 (zobacz 10 też)
        //k = 1, tLL=4 dla 10 

    Tabu tabu(numberOfIterations, tabuListLength, neighborhoodSize, ballRadius, packer, seed);
    TabuFullHistory result = tabu.algorithmWithScoresKFinal(numberOfTrees, sigma, k);

    std::ofstream treeFile("../my_results/finalResults/finalTrees10Coords.csv");
    if (treeFile.is_open()) {
        treeFile << "tree_x,tree_y,tree_angle\n";
        for (const auto& t : result.trees) {
            treeFile << t.x << "," << t.y << "," << t.angle << "\n";
        }
        treeFile.close();
    }

    std::ofstream scoreFile("../my_results/finalResults/finalTrees10Plot.csv");
    if (scoreFile.is_open()) {
        scoreFile << "iteration,global_best,current_score\n";
        for (const auto& s : result.scores) {
            scoreFile << s.iteration << "," << s.globalBestScore << "," << s.iterationBestScore << "\n";
        }
        scoreFile.close();
    }
    
    return 0;
}



