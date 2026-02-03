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
        // const std::vector<std::shared_ptr<ChristmasTree>>& trees,
        // const std::string& tree_count_prefix,
        const std::string& filename = "sumbission.csv");

        // DE
        void saveSolutionToCSV( const std::string& filename, const std::vector<float>& angles_normalized, const std::vector<std::vector<std::pair<double, double>>>& positions, int num_configs, const std::vector<std::vector<double>>& best_angles_per_config, const std::vector<std::vector<std::pair<double, double>>>& best_positions_per_config);

        // DE
        void saveIterationScores(const std::string& filename, const std::vector<std::vector<double>>& iteration_scores, int num_configs);

        // DE 
        // void saveConfigScoresWithSum( const std::string& filename, const std::vector<double>& config_scores);
static void saveConfigScoresWithSum(
        const std::string& filename,
        const std::vector<double>& config_scores,

        const std::string& score_column_name = "score"  // Domyślna wartość
    );void saveIterationScoresExtended(
        const std::string& filename,
        const std::vector<std::vector<double>>& best_scores,
        const std::vector<std::vector<double>>& worst_scores,
        const std::vector<std::vector<double>>& avg_scores,
        int num_configs
    );

   
    };
    


#endif
