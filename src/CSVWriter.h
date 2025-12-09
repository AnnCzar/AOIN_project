#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include "ChristmasTree.h"
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <iomanip>

class CSVWriter {
public:

    static void saveTreesToCSV(
        const std::vector<std::shared_ptr<ChristmasTree>>& trees,
        const std::string& filename = "results.csv");
    
    };

#endif
