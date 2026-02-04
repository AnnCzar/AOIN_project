#include "submission_file_greedy.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <iostream>


void submissionFileGreedy::saveTreesToCSVGreedy(
    const std::vector<std::shared_ptr<ChristmasTree>>& trees,  
    const std::string& filename) {
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Can't open file: " << filename << std::endl;
        return;
    }
    
    file << "id,x,y,deg\n";
    
    for (size_t n = 1; n <= trees.size(); n++) {
        std::stringstream ss;
        ss << std::setw(3) << std::setfill('0') << n;
        std::string prefix = ss.str();
        
        for (size_t i = 0; i < n; i++) {
            const auto& tree = trees[i];
            file << prefix << "_" << i << ",";
            file << "s" << std::fixed << std::setprecision(6) << tree->getX() << ",";
            file << "s" << std::fixed << std::setprecision(6) << tree->getY() << ",";
            file << "s" << std::fixed << std::setprecision(6) << tree->getAngle() << "\n";
        }
    }
    
    file.close();
    
    std::cout << "Zapisano konfiguracje 1-" << trees.size() 
              << " do " << filename << std::endl;
}