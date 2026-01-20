#ifndef PSOPARTICLE_H
#define PSOPARTICLE_H

#include "ChristmasTree.h"
#include <vector>
#include <memory>


class PsoParticle{
public:

    PsoParticle(const std::vector<float>& position, const std::vector<float>& velocity);
    void updatePersonalBest(const std::vector<float>& newPosition, float newScore);
    void updateVelocity(const std::vector<float>& newVelocity);
    void updatePosition(const std::vector<float>& newPosition);

    std::vector<float>& getPosition();
    std::vector<float>& getVelocity();
    std::vector<float>& getPersonalBest();
    float getPersonalBestScore();
private:

    std::vector<float> position;
    std::vector<float> velocity;
    std::vector<float> personalBest;
    float personalBestScore;

};

#endif