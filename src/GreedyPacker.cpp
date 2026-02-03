#define _USE_MATH_DEFINES
#include "GreedyPacker.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include "SpatialHash.h"


std::pair<std::vector<std::shared_ptr<ChristmasTree>>, std::vector<double>>
GreedyPacker::packTrees(int num_trees) {
    std::vector<std::shared_ptr<ChristmasTree>> trees;
    std::vector<double> sides_square;
    
    trees.reserve(num_trees);
    sides_square.reserve(num_trees);

    if (num_trees == 0) return {trees, sides_square};
    
    std::cout << "\nUmieszczanie choinki 0 w (0, 0) z katem 0" << std::endl;
    auto tree0 = std::make_shared<ChristmasTree>(0.0, 0.0, 0.0);
    trees.push_back(tree0);
    
    SpatialHash spatial_hash(15.0);
    spatial_hash.insert(tree0, 0);
    
    double current_side0 = calculateGlobalSquareSide(trees, tree0);
    sides_square.push_back(current_side0);
    
    const auto* env = tree0->getEnvelope();
    std::cout << "Pierwsza choinka umieszczona. Ramka ograniczajaca: ["
              << std::fixed << std::setprecision(2)
              << env->getMinX() << ", " << env->getMinY() << "] - ["
              << env->getMaxX() << ", " << env->getMaxY() << "]"
              << std::endl;

    if (num_trees == 1) return {trees, sides_square};

    for (int i = 1; i < num_trees; i++) {
        auto new_tree = std::make_shared<ChristmasTree>(0.0, 0.0, 0.0);
        
        auto [best_x, best_y] = findBestPosition(new_tree, trees);
        new_tree->setPosition(best_x, best_y);

        double best_angle = findBestAngle(new_tree, trees);
        new_tree->setAngle(best_angle);
        
        trees.push_back(new_tree);
        spatial_hash.insert(new_tree, i);
        
        double current_side = calculateGlobalSquareSide(trees, new_tree);
        sides_square.push_back(current_side);
    }
    
    return {trees, sides_square};
}


std::pair<std::vector<std::shared_ptr<ChristmasTree>>, std::vector<double>>
GreedyPacker::packTreesWithFixedAnglesSA(const std::vector<double>& angles) {
    std::vector<std::shared_ptr<ChristmasTree>> trees;
    std::vector<double> sides_square;
    
    trees.reserve(angles.size());
    sides_square.reserve(angles.size());

    if (angles.empty()) return {trees, sides_square};
    
    auto tree0 = std::make_shared<ChristmasTree>(0.0, 0.0, angles[0]);
    trees.push_back(tree0);

    SpatialHash spatial_hash(15.0);
    spatial_hash.insert(tree0, 0);
    
    double current_side0 = calculateGlobalSquareSide(trees, tree0);
    sides_square.push_back(current_side0);

    if (angles.size() == 1) return {trees, sides_square};

    for (size_t i = 1; i < angles.size(); i++) {
        auto new_tree = std::make_shared<ChristmasTree>(0.0, 0.0, angles[i]);
        
        auto [best_x, best_y] = findBestPosition(new_tree, trees);
        new_tree->setPosition(best_x, best_y);
        
        tryMoveCloser(new_tree, trees, spatial_hash);
        
        auto candidates = spatial_hash.get_potential_collisions(new_tree);
        
        bool has_collision = false;
        for (int idx : candidates) {
            if (new_tree->intersects(*trees[idx])) {
                has_collision = true;
        
                double safe_angle = findBestAngle(new_tree, trees);
                new_tree->setAngle(safe_angle);
                break;
            }
        }
        
        trees.push_back(new_tree);
        spatial_hash.insert(new_tree, i);
        
        double current_side = calculateGlobalSquareSide(trees, new_tree);
        sides_square.push_back(current_side);
    }
    
    return {trees, sides_square};
}

std::pair<double, double>
GreedyPacker::findBestPosition(
    const std::shared_ptr<ChristmasTree>& new_tree,
    const std::vector<std::shared_ptr<ChristmasTree>>& placed) {


    SpatialHash spatial_hash(15.0);
    for (int i = 0; i < placed.size(); ++i) {
        spatial_hash.insert(placed[i], i);
    }

    const int NUM_DIRECTIONS = 45;      
    const double START_DISTANCE = 15.0; 
    const double STEP = 0.2;           
    const double MIN_DIST = 0.1;      

    double best_distance = 1e9;  

    double best_square = 1e9;  
    double best_center = 1e9; 
    double best_x = 0.0, best_y = 0.0;


    for (int dir = 0; dir < NUM_DIRECTIONS; dir++) {
        double angle_rad = (2.0 * M_PI * dir) / NUM_DIRECTIONS;
        double dx = std::cos(angle_rad);
        double dy = std::sin(angle_rad);

        for (double dist = START_DISTANCE; dist > MIN_DIST; dist -= STEP) {
            double x = dx * dist;
            double y = dy * dist;

            new_tree->setPosition(x, y);

            auto candidates = spatial_hash.get_potential_collisions(new_tree);

            bool collision = false;
            for (int idx : candidates) {
                if (new_tree->intersects(*placed[idx])) {
                    collision = true;
                    break;
                }
            }
             if (!collision) {
                double current_square = calculateGlobalSquareSide(placed, new_tree);
                double current_center = std::hypot(x, y);

                if (current_square < best_square - 0.001) {
                    best_square = current_square;
                    best_center = current_center;
                    best_x = x;
                    best_y = y;
                }
                else if (std::abs(current_square - best_square) < 0.001) {
                    if (current_center < best_center) {
                        best_center = current_center;
                        best_x = x;
                        best_y = y;
                    }
                }
            }

        }
    }
    return { best_x, best_y };

}

double GreedyPacker::findBestAngle(
    const std::shared_ptr<ChristmasTree>& new_tree,
    const std::vector<std::shared_ptr<ChristmasTree>>& placed) {
    
    SpatialHash spatial_hash(15.0);
    for (int i = 0; i < placed.size(); ++i) {
        spatial_hash.insert(placed[i], i);
    }

    const int step = 5;
    double best_angle = 0.0;
    double current_x = new_tree->getX();
    double current_y = new_tree->getY();
    double best_square = 1e9;

    for (int rotation = 0; rotation < 360; rotation += step) {
        new_tree->setAngle(rotation);
        new_tree->setPosition(current_x, current_y);

        auto candidates = spatial_hash.get_potential_collisions(new_tree);
        
        bool collision = false;
        for (int idx : candidates) {
            if (new_tree->intersects(*placed[idx])) {
                collision = true;
                break;
            }
        }
        
        if (!collision) {
            double saved_x = new_tree->getX();
            double saved_y = new_tree->getY();
            
            tryMoveCloser(new_tree, placed, spatial_hash);
            double current_square = calculateGlobalSquareSide(placed, new_tree);

            if (current_square < best_square) {
                best_square = current_square;
                best_angle = rotation;
            }
            
            new_tree->setPosition(saved_x, saved_y);
        }
    }
    
    new_tree->setAngle(best_angle);
    new_tree->setPosition(current_x, current_y);
    SpatialHash final_hash(15.0);
    for (int i = 0; i < placed.size(); ++i) {
        final_hash.insert(placed[i], i);
    }

    tryMoveCloser(new_tree, placed, spatial_hash);
    
    return best_angle;
}

double GreedyPacker::tryMoveCloser(
    const std::shared_ptr<ChristmasTree>& tree,
    const std::vector<std::shared_ptr<ChristmasTree>>& placed,
    const SpatialHash& spatial_hash) {
    
    double current_x = tree->getX();
    double current_y = tree->getY();
    double current_distance = std::hypot(current_x, current_y);
    
    if (current_distance < 0.01) {
        return current_distance;
    }
    
    double to_center_x = -current_x / current_distance;
    double to_center_y = -current_y / current_distance;
    
    const double PUSH_STEP = 0.1;
    double best_distance = current_distance;
    double best_x = current_x;
    double best_y = current_y;
    
    for (double d = PUSH_STEP; d < current_distance; d += PUSH_STEP) {
        double new_x = current_x + to_center_x * d;
        double new_y = current_y + to_center_y * d;
        
        tree->setPosition(new_x, new_y);
        
        auto candidates = spatial_hash.get_potential_collisions(tree);
        
        bool collision = false;
        for (int idx : candidates) {
            if (tree->intersects(*placed[idx])) {
                collision = true;
                break;
            }
        }
        
        if (collision) {
            tree->setPosition(best_x, best_y);
            return best_distance;
        } else {
            best_x = new_x;
            best_y = new_y;
            best_distance = calculateGlobalSquareSide(placed, tree);
        }
    }
    
    return best_distance;
}

double GreedyPacker::calculateGlobalSquareSide(
    const std::vector<std::shared_ptr<ChristmasTree>>& placed,
    const std::shared_ptr<ChristmasTree>& new_tree) {

    double minX = 1e9;
    double maxX = -1e9;
    double minY = 1e9;
    double maxY = -1e9;
    
    for (const auto& t : placed) {
        const auto* env = t->getEnvelope();
        if (env->getMinX() < minX) minX = env->getMinX();
        if (env->getMaxX() > maxX) maxX = env->getMaxX();
        if (env->getMinY() < minY) minY = env->getMinY();
        if (env->getMaxY() > maxY) maxY = env->getMaxY();
    }

    const auto* env = new_tree->getEnvelope();
    if (env->getMinX() < minX) minX = env->getMinX();
    if (env->getMaxX() > maxX) maxX = env->getMaxX();
    if (env->getMinY() < minY) minY = env->getMinY();
    if (env->getMaxY() > maxY) maxY = env->getMaxY();

    double sf = new_tree->getScaleFactor();
    double width = maxX - minX;
    double height = maxY - minY;

    return std::max(width, height) / sf;
}