#include "CSVWriter.h"
#include <iostream>

void CSVWriter::saveTreesToCSV(
    const std::vector<std::shared_ptr<ChristmasTree>>& trees,
    const std::vector<double>& sides_square,
    const std::string& filename) {
    
    std::ofstream file(filename);
    
    // header w pliku
    file << "tree_id,center_x,center_y,angle,square_side\n";


    // dane choinek
    for (size_t i = 0; i < trees.size(); i++) {
        file << i << ","
             << std::fixed << std::setprecision(4)
             << trees[i]->getX() << ","
             << trees[i]->getY() << ","
             << trees[i]->getAngle() << ","
             << sides_square[i] << "\n";
    }
    
    file.close();
    
    std::cout << "Zapisano " << trees.size() 
              << " choinek zapisanych do " << filename << std::endl;
}

