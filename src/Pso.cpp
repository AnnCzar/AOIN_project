#include "Pso.h"
#include "GreedyPacker.h"
#include <cmath>
#include <iostream>
#include <iomanip>


Pso::Pso(double inertia_weight, double inertia_weight_max, double inertia_weight_min,
         double acc_c_1, double acc_c_2, int number_of_particles,
        int number_of_iterations, GreedyPacker packer, int seed) : inertia_weight(inertia_weight),
        inertia_weight_max(inertia_weight_max),inertia_weight_min(inertia_weight_min),
        acc_c_1(acc_c_1), acc_c_2(acc_c_2),
number_of_particles(number_of_particles), 
number_of_iterations(number_of_iterations),
packer(packer),
rng(seed){}


void Pso::updateGlobalBest(std::vector<float>& globalBest, float globalBestScore){
    this->globalBest = globalBest;
    this->globalBestScore = globalBestScore;
}



PsoParticle Pso::initializeParticle(int num_trees){ //seed?


    std::uniform_real_distribution<float> angles(0, 360);
    std::uniform_real_distribution<float> velocities(-100.0,100.0);



    std::vector<float> position;
    std::vector<float> velocity;
    for(int i=0; i<num_trees; i++){
        position.push_back(angles(rng)); //kąty 0-360
        velocity.push_back(velocities(rng)); //tu jaki range

    }
    PsoParticle particle(position,velocity);

    return particle;
}


double Pso::evaluateFitness(int num_trees, std::vector<float>& position) {
    auto [trees, squares] = packer.packTreesWithAngles(num_trees, position);

    return squares.back();
}

std::vector<float> Pso::calculateNewPosition(PsoParticle& particle){

    int n = particle.getPosition().size();

    std::vector<float> newPos(n);
    for(int i=0; i<n; i++){
        newPos[i] = particle.getPosition()[i] + particle.getVelocity()[i];
        newPos[i] = std::fmod(newPos[i],360);
        if(newPos[i]<=0){newPos[i]+=360;}
    }
    return newPos;
}

std::vector<float> Pso::calculateNewVelocity(PsoParticle& particle){

    int n = particle.getPosition().size();

    std::uniform_real_distribution<float> range(0.0f, 1.0f);
    float r1 = range(rng);
    float r2 = range(rng);

    std::vector<float> newVel(n);
    for(int i=0; i<n; i++){
        newVel[i] =  inertia_weight*particle.getVelocity()[i] + 
        acc_c_1*r1*(particle.getPersonalBest()[i]-particle.getPosition()[i]) + 
        acc_c_2*r2*(globalBest[i] - particle.getPosition()[i]);
    }
    return newVel;
}

std::vector<float> Pso::calculateNewVelocityImproved(PsoParticle& particle){


    int n = particle.getPosition().size();

    std::uniform_real_distribution<float> range(0.0f, 1.0f);
    float r1 = range(rng);
    float r2 = range(rng);
    float r3 = range(rng);

    std::vector<float> newVel(n);
    for(int i=0; i<n; i++){
        newVel[i] =  inertia_weight*particle.getVelocity()[i] + 
        acc_c_1*r1*(particle.getPersonalBest()[i]-particle.getPosition()[i]) + 
        acc_c_2*r2*(globalBest[i] - particle.getPosition()[i]) +
        acc_c_3*r3*(globalSecondBest[i] - particle.getPosition()[i]);
    }
    return newVel;

}


void Pso::updateInertiaWeight(int iteration){
    double newInertia = inertia_weight_max - (inertia_weight_max-inertia_weight_min) * (iteration/(double)number_of_iterations);
    this->inertia_weight = newInertia;
}


std::vector<float> Pso::algorithm(int num_trees) {
    std::vector<PsoParticle> swarm;
    
    globalBestScore=std::numeric_limits<float>::infinity();

    //losowa inicjalizacja cząstek i globalBest
    for(int i=0; i<number_of_particles; i++){
        swarm.push_back(initializeParticle(num_trees));
        double score = evaluateFitness(num_trees, swarm[i].getPosition());
        if(score<=globalBestScore){
            globalBestScore=score;
            globalBest=swarm[i].getPosition();
        }
    }

    for(int it=0; it<number_of_iterations; it++){
        
    //pętla po wszystkich cząstkach        
        for(int i=0; i<number_of_particles; i++){

            std::vector<float> position = swarm[i].getPosition(); //z sąsiedztwa?
            double score = evaluateFitness(num_trees, position);

            if(score<=swarm[i].getPersonalBestScore()){
                swarm[i].updatePersonalBest(position,score);
            }

            if(swarm[i].getPersonalBestScore()<globalBestScore){
                globalBest = swarm[i].getPersonalBest();
                globalBestScore = swarm[i].getPersonalBestScore();
            }

        }


        //zaktualizuj pozycje i velocity
        for(int i=0; i<number_of_particles; i++){
            swarm[i].updateVelocity(calculateNewVelocity(swarm[i]));
            swarm[i].updatePosition(calculateNewPosition(swarm[i]));
        }

        //zaktualizuj inertia
        updateInertiaWeight(it);
        
    }

    return globalBest;
    
    
}




std::vector<float> Pso::algorithmImproved(int num_trees, double Prob) {
    std::vector<PsoParticle> swarm;
    
    globalBestScore=std::numeric_limits<float>::infinity();
    globalSecondBestScore=std::numeric_limits<float>::infinity();
    std::uniform_real_distribution<float> range(0.0f, 1.0f);



    //losowa inicjalizacja cząstek i globalBest
    for(int i=0; i<number_of_particles; i++){
        swarm.push_back(initializeParticle(num_trees));
        double score = evaluateFitness(num_trees, swarm[i].getPosition());
        if(score<=globalBestScore){
            globalSecondBestScore = globalBestScore;
            globalSecondBest = globalBest; //pierwszy spada na drugie miejsce
            globalBestScore=score;
            globalBest=swarm[i].getPosition();
        }
        else if(score <=globalSecondBestScore && score > globalBestScore){
            globalSecondBestScore = globalBestScore;
            globalSecondBest = globalBest;
        }
    }


    for(int it=0; it<number_of_iterations; it++){
        
    //pętla po wszystkich cząstkach        
        for(int i=0; i<number_of_particles; i++){

            std::vector<float> position = swarm[i].getPosition(); //z sąsiedztwa?
            double score = evaluateFitness(num_trees, position);
            
            if(score<=swarm[i].getPersonalBestScore()){
                swarm[i].updatePersonalBest(position,score);
            }

            if(swarm[i].getPersonalBestScore()<globalBestScore){
                globalSecondBest = globalBest;
                globalSecondBestScore = globalBestScore;
                globalBest = swarm[i].getPersonalBest();
                globalBestScore = swarm[i].getPersonalBestScore();
            }

            if(swarm[i].getPersonalBestScore()<globalSecondBestScore && swarm[i].getPersonalBestScore()>globalBestScore){
                globalSecondBest = swarm[i].getPersonalBest();
                globalSecondBestScore = swarm[i].getPersonalBestScore();
            }

        }


        float r1 = range(rng);

    //zaktualizuj pozycje i velocity
        if(r1>Prob){
            for(int i=0; i<number_of_particles; i++){
                swarm[i].updateVelocity(calculateNewVelocity(swarm[i]));
                swarm[i].updatePosition(calculateNewPosition(swarm[i]));
            }
        }else{
            for(int i=0; i<number_of_particles; i++){
                swarm[i].updateVelocity(calculateNewVelocityImproved(swarm[i]));
                swarm[i].updatePosition(calculateNewPosition(swarm[i]));
            }
        }
       

        //zaktualizuj inertia
        updateInertiaWeight(it);
        
    }

    return globalBest;
    
    
}


std::vector<std::vector<float>> Pso::runForAllTreeCounts(int maxTrees){

    std::vector<std::vector<float>> results;
    for(int k=1; k<=maxTrees; k++){
        results.push_back(algorithm(k));
        std::cout << k << std::endl;
    }

    return results;
}
