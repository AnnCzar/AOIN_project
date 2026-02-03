#include "evaluation.h"
#include <vector>
#include <map>
#include <sstream>
#include <stdexcept>
#include "structures.h"
#include <algorithm>
#include "ChristmasTree.h"

#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/GeometryCollection.h>
#include <geos/geom/Envelope.h>
#include <geos/index/strtree/STRtree.h>
#include <geos/operation/union/UnaryUnionOp.h>

double Evaluation::parseValue(const std::string& value) {
    if (value.empty() || value[0] != 's') {
        throw std::runtime_error("Value without 's' prefix found: " + value);
    }
    return std::stod(value.substr(1));
}

std::string Evaluation::extractGroup(const std::string& id) {
    size_t underscore_pos = id.find('_');
    if (underscore_pos != std::string::npos) {
        return id.substr(0, underscore_pos);
    }
    return id;
}


std::vector<SubmissionRow> Evaluation :: loadSubmissionFile(const std::string filename) {
    std::vector<SubmissionRow> submission;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::string line;
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, x_str, y_str, deg_str;
        
        if (std::getline(ss, id, ',') &&
            std::getline(ss, x_str, ',') &&
            std::getline(ss, y_str, ',') &&
            std::getline(ss, deg_str)) {
            
            SubmissionRow row;
            row.id = id;
            row.x = parseValue(x_str);
            row.y = parseValue(y_str);
            row.deg = parseValue(deg_str);
            
            submission.push_back(row);
        }
    }
    
    file.close();
    return submission;
}

double Evaluation::evaluation_score(const std::vector<SubmissionRow>& submission) {   
    const double limit = 100.0;
    const double PENALTY_PER_COLLISION = 1000.0;  
    
    for (const auto& row : submission) {
        if (row.x < -limit || row.x > limit || row.y < -limit || row.y > limit) {
            throw std::runtime_error("x and/or y values outside the bounds of -100 to 100");
        }
    }
    
    std::map<std::string, std::vector<SubmissionRow>> groups;
    for (const auto& row : submission) {
        std::string group = extractGroup(row.id);
        groups[group].push_back(row);
    }
    
    double total_score = 0.0;
    int total_collisions = 0;
    
    for (const auto& [group_name, df_group] : groups) {
        int num_trees = df_group.size();
        std::vector<ChristmasTree> placed_trees;
        
        for (const auto& row : df_group) {
            placed_trees.emplace_back(row.x, row.y, row.deg);
        }
        double scale_factor = placed_trees[0].getScaleFactor();
        
        std::vector<geos::geom::Geometry*> all_polygons;
        for (auto& tree : placed_trees) {
            all_polygons.push_back(tree.getPolygon());
        }
        
        geos::index::strtree::STRtree r_tree; 
        for (size_t i = 0; i < all_polygons.size(); ++i) {
            r_tree.insert(all_polygons[i]->getEnvelopeInternal(), (void*)i);
        }
        
        int group_collisions = 0;
        
        for (size_t i = 0; i < all_polygons.size(); ++i) {
            std::vector<void*> results;
            r_tree.query(all_polygons[i]->getEnvelopeInternal(), results);
            
            for (void* result : results) {
                size_t j = (size_t)result;
                if (i >= j) continue; 
                
                if (all_polygons[i]->intersects(all_polygons[j]) && 
                    !all_polygons[i]->touches(all_polygons[j])) {
                    group_collisions++;
                    
                }
            }
        }
        
        total_collisions += group_collisions;
        
        const geos::geom::GeometryFactory* factory = geos::geom::GeometryFactory::getDefaultInstance();
        std::vector<const geos::geom::Geometry*> geoms;
        for (auto* poly : all_polygons) {
            geoms.push_back(poly);
        }
        
        std::unique_ptr<geos::geom::GeometryCollection> geom_collection(
            factory->createGeometryCollection(geoms)
        );
        
        std::unique_ptr<geos::geom::Geometry> union_geom(geom_collection->Union());
        const geos::geom::Envelope* bounds = union_geom->getEnvelopeInternal();
        
        double side_length_scaled = std::max(bounds->getWidth(), bounds->getHeight());
        
        double group_score = (side_length_scaled * side_length_scaled) / 
                            (scale_factor * scale_factor) / 
                            static_cast<double>(num_trees);
        
        double collision_penalty = group_collisions * PENALTY_PER_COLLISION;
        
        total_score += group_score + collision_penalty;
    }
    
    // if (total_collisions > 0) {
    //     std::cerr << "WARNING: Total collisions in submission: " << total_collisions << std::endl;
    //     std::cerr << "WARNING: Total penalty added: " << (total_collisions * PENALTY_PER_COLLISION) << std::endl;
    // }
    
    return total_score;
}

