#include "PsoParticle.h"
#include <cstdlib>   
#include <limits>


PsoParticle::PsoParticle(const std::vector<float>& position, const std::vector<float>& velocity)
: position(position), velocity(velocity), personalBest(position),
      personalBestScore(std::numeric_limits<float>::infinity())
{


}

void PsoParticle::updatePersonalBest(const std::vector<float>& newPosition, float newScore) {   
    if (newScore < personalBestScore) {
        this->personalBest = newPosition;
        this->personalBestScore = newScore;
    }    
}

void PsoParticle::updateVelocity(const std::vector<float>& newVelocity){
        this->velocity = newVelocity;
}

void PsoParticle::updatePosition(const std::vector<float>& newPosition){
        this->position = newPosition;
}

std::vector<float>& PsoParticle::getPosition(){
        return this->position;
}

std::vector<float>& PsoParticle::getVelocity(){
        return this->velocity;
}

std::vector<float>& PsoParticle::getPersonalBest(){
        return this->personalBest;
}

float PsoParticle::getPersonalBestScore(){
        return this->personalBestScore;
}