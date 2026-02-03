#ifndef SUBMISSION_FILE_GREEDY_H
#define SUBMISSION_FILE_GREEDY_H

#include "ChristmasTree.h"
#include <vector>
#include <memory>
#include <string>

class submissionFileGreedy {
public:
    void saveTreesToCSVGreedy(
        const std::vector<std::shared_ptr<ChristmasTree>>& trees,
        const std::string& filename = "sumbission_greedy.csv"
    );
};

#endif 
