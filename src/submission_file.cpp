#include "submission_file.h"
#include <iostream>

void submissionFile::saveTreesToCSV(
    const std::vector<std::vector<std::shared_ptr<ChristmasTree>>>& all_groups,
    const std::string& filename) {
    
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Can't open file: " << filename << std::endl;
        return;
    }
    
    file << "id,x,y,deg\n";


    for (size_t n = 0; n < all_groups.size(); n++) {
        const auto& group = all_groups[n];

        std::stringstream ss;
        ss << std::setw(3) << std::setfill('0') << (n + 1);
        std::string prefix = ss.str();



        for (size_t i = 0; i < group.size(); i++) {

            const auto& tree = group[i];

            file << prefix << "_" << i << ",";

            file << "s" << std::fixed << std::setprecision(6) << tree->getX() << ",";
            file << "s" << std::fixed << std::setprecision(6) << tree->getY() << ",";
            file << "s" << std::fixed << std::setprecision(6) << tree->getAngle() << "\n";
        }
    }
    
    file.close();
    
    std::cout << "Zapisano dla konfiguracji:" << all_groups.size() 
              << "do " << filename << std::endl;
}

