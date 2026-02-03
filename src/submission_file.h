#ifndef SUBMISSION_FILE_H
#define SUBMISSION_FILE_H

#include "ChristmasTree.h"
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <iomanip>

class submissionFile {
public:

    static void saveTreesToCSV(
        const std::vector<std::vector<std::shared_ptr<ChristmasTree>>>& all_groups,
        
        const std::string& filename = "sumbission.csv");
           
    };

#endif
