#include "Tabu.h"
#include "GreedyPacker.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <random>


Tabu::Tabu(int number_of_iterations, int tabuListLength, int neighborhoodSize, float ballRadius, GreedyPacker& packer, int seed) : number_of_iterations(number_of_iterations),
    tabuListLength(tabuListLength), neighborhoodSize(neighborhoodSize), ballRadius(ballRadius),packer(packer),rng(seed){}


float Tabu::evaluateFitness(std::vector<float>& position) {
    auto [trees, squares] = packer.packTreesWithFixedAngles(position);

    return squares.back();
}


std::vector<float> Tabu::rotate_tree(const std::vector<float>& angles, float sigma) {
 
    std::normal_distribution<float> d{0.0,sigma};

    std::vector<float> new_angles;
    new_angles.reserve(angles.size());

    for (float angle : angles) {
        float perturbation = d(rng);
        float new_angle = std::fmod(angle + perturbation, 360.0);
        if (new_angle < 0.0) {
            new_angle += 360.0;
        }
        new_angles.push_back(new_angle);

    }
    return new_angles;
}


std::vector<std::vector<float>> Tabu::createNeighborhood(std::vector<float>& currentSolution, float sigma){

    std::vector<std::vector<float>> neighborhood;

    for(int i=0; i<neighborhoodSize; i++){
        neighborhood.push_back(rotate_tree(currentSolution,sigma));
    }

    return neighborhood;
}

std::pair<std::vector<float>,float> Tabu::getBestNeighbor(std::vector<std::vector<float>>& neighborhood, std::vector<std::vector<float>>& tabuList, 
                                                            float bestScore, int numberOfTrees){

    std::vector<float> bestNeighbor;
    float bestNeighborScore = std::numeric_limits<float>::infinity();

    for(int i=0; i<neighborhoodSize; i++){

        float contenderScore = evaluateFitness(neighborhood.at(i));
        if(contenderScore<bestNeighborScore){
            if(!doesTabuContain(tabuList,neighborhood.at(i))){
                bestNeighbor = neighborhood.at(i);
                bestNeighborScore = contenderScore;
            }else{                                  //Kryterium aspiracji - nie spełnia tabu ale jest lepsze od global besta
                if(contenderScore<bestScore){
                    bestNeighbor = neighborhood.at(i);
                    bestNeighborScore = contenderScore;
                }
            }
        }
    }

    return {bestNeighbor,bestNeighborScore};

}

bool Tabu::doesTabuContain(std::vector<std::vector<float>>& tabuList, std::vector<float>& solution){

    
    for(const std::vector<float>& tabuSolution : tabuList){
        bool violatedAll = true;

        for(int i=0; i<tabuSolution.size(); i++){
            float distance = abs(tabuSolution.at(i) - solution.at(i));

            if(distance > 180.0){
                distance = 360.0 - distance;
            }

            if(distance < ballRadius){
                continue;
            }else{
                violatedAll=false;
                break;
            }
        }
        if(violatedAll){
            return true;
        }


    }
    return false;
}


std::pair<std::vector<float>,float> Tabu::algorithm(int numberOfTrees, float sigma){


    std::uniform_real_distribution<float> angles(0, 360);
    std::vector<std::vector<float>> tabuList;


    std::vector<float> bestSolution;
    std::vector<float> currSolution;

    //inicjalizacja - losowe
    for(int i=0; i<numberOfTrees; i++){
        currSolution.push_back(angles(rng));
    }
    float currScore = evaluateFitness(currSolution);
    bestSolution = currSolution;
    float bestScore = currScore;

    //główna pętla; koniec - liczba iteracji
    for(int it=0; it<number_of_iterations; it++){

        std::vector<std::vector<float>> neighborhood = createNeighborhood(currSolution,sigma);
        
        
        auto result = getBestNeighbor(neighborhood, tabuList, bestScore, numberOfTrees);
        auto bestNeighbor = result.first;
        auto bestNeighborScore = result.second;

        currSolution = bestNeighbor;
        if(bestNeighborScore < bestScore){
            bestSolution = bestNeighbor;
            bestScore = bestNeighborScore;
        }

        tabuList.push_back(bestNeighbor);
        if(tabuList.size()>tabuListLength){
            tabuList.erase(tabuList.begin());
        }


    }

    return {bestSolution,bestScore};
}



std::vector<TabuMetrics> Tabu::algorithmWithScores(int numberOfTrees, float sigma){

    std::vector<TabuMetrics> history;
    std::uniform_real_distribution<float> angles(0, 360);
    std::vector<std::vector<float>> tabuList;


    std::vector<float> bestSolution;
    std::vector<float> currSolution;

    globalBestScore = std::numeric_limits<float>::infinity();
    globalWorstScore = -std::numeric_limits<double>::infinity();


    //inicjalizacja - losowe
    for(int i=0; i<numberOfTrees; i++){
        currSolution.push_back(angles(rng));
    }
    float currScore = evaluateFitness(currSolution);
    bestSolution = currSolution;
    float bestScore = currScore;
    
    globalBestScore = currScore;
    globalWorstScore = currScore;

    //główna pętla; koniec - liczba iteracji
    for(int it=0; it<number_of_iterations; it++){

        std::vector<std::vector<float>> neighborhood = createNeighborhood(currSolution,sigma);
        
        
        auto result = getBestNeighbor(neighborhood, tabuList, bestScore, numberOfTrees);
        auto bestNeighbor = result.first;
        auto bestNeighborScore = result.second;

        currSolution = bestNeighbor;
        if(bestNeighborScore < bestScore){
            bestSolution = bestNeighbor;
            bestScore = bestNeighborScore;
            globalBestScore = bestNeighborScore;
        }

        if (bestNeighborScore > globalWorstScore) {
            globalWorstScore = bestNeighborScore;
        }

        tabuList.push_back(bestNeighbor);
        if(tabuList.size()>tabuListLength){
            tabuList.erase(tabuList.begin());
        }

        history.emplace_back(globalBestScore, bestNeighborScore, globalWorstScore);

        std::cout <<"Tabu's iteration: " << it << std::endl;
    }

    return history;
}