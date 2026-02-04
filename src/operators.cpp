#include <random>
#include <algorithm>
#include <unordered_set>
#include <vector>

using namespace std;

std::vector<double> rotate_tree(const std::vector<double>& angles, double sigma) {
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution d{0.0, sigma};

    std::vector<double> new_angles;
    new_angles.reserve(angles.size());

    for (double angle : angles) {
        double perturbation = d(gen);
        double new_angle = std::fmod(angle + perturbation, 360.0);
        if (new_angle < 0.0) {
            new_angle += 360.0;
        }
        new_angles.push_back(new_angle);

    }
    return new_angles;
}

// tu z katem z losowego rozkladu
// std::vector<double> rotate_tree(const std::vector<double>& angles, double sigma) {
//     std::random_device rd{};
//     std::mt19937 gen{rd()};
//     std::uniform_real_distribution <double>dist(-sigma, sigma);

//     std::vector<double> new_angles;
//     new_angles.reserve(angles.size());

//     for (double angle : angles) {
//         double perturbation = dist(gen);
//         double new_angle = std::fmod(angle + perturbation, 360.0);
//         if (new_angle < 0.0) {
//             new_angle += 360.0;
//         }
//         new_angles.push_back(new_angle);

//     }
//     return new_angles;
// }


std::vector<double> rotate_tree_selective(const std::vector<double>& angles, double sigma, double pm) {
    std::random_device rd{};
    std::mt19937 gen{rd()};
    // std::uniform_real_distribution <double>dist(-sigma, sigma);
    std::normal_distribution d{0.0, sigma};
    std::uniform_real_distribution<double> prob_dist(0.0, 1.0);

    std::vector<double> new_angles = angles;

    for (size_t i = 0; i < new_angles.size(); ++i) {
        if (prob_dist(gen) < pm) {
            double perturbation = d(gen);
            new_angles[i] = std::fmod(new_angles[i] + perturbation + 360.0, 360.0);
        }
    }    return new_angles;
}
