#include "submission_file.h"
#include <iostream>
#include <sstream>  
void submissionFile::saveTreesToCSV(
    const std::vector<std::vector<std::shared_ptr<ChristmasTree>>>& all_groups,
    const std::string& filename) {
    
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Can't open file: " << filename << std::endl;
        return;
    }
    
    // header w pliku
    file << "id,x,y,deg\n";


    // dane choinek
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


/**
 * Zapis rozwiązania z DE 
 */
void submissionFile::saveSolutionToCSV( const std::string& filename, const std::vector<float>& best_angles, 
    const std::vector<std::vector<std::pair<double, double>>>& best_positions, int num_configs, const std::vector<std::vector<double>>& best_angles_per_config, const std::vector<std::vector<std::pair<double, double>>>& best_positions_per_config)
    {

    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
        return;
    }
    
    file << "id,x,y,deg\n";
    
    // po koleji konfiguracje
    for (int config = 0; config < num_configs; ++config) {
        int num_trees = config + 1;
        
        std::stringstream ss;
        ss << std::setw(3) << std::setfill('0') << (config + 1);
        std::string prefix = ss.str();
        
        for (int i = 0; i < num_trees; ++i) {

            file << prefix << "_" << i << ",";
            
            // bez s
            // file << std::fixed << std::setprecision(6) 
            //      << positions[config][i].first << ",";
            // file << std::fixed << std::setprecision(6) 
            //      << positions[config][i].second << ",";
            file << std::fixed << std::setprecision(6) 
                 << best_positions_per_config[config][i].first << ",";
            file << std::fixed << std::setprecision(6) 
                 << best_positions_per_config[config][i].second << ",";
            // double angle_deg = angles_normalized[i] * 360.0;
            // file << std::fixed << std::setprecision(6) 
            //      << angle_deg << "\n";
            double angle_deg = best_angles_per_config[config][i] * 360.0;
            file << std::fixed << std::setprecision(6) 
                 << angle_deg << "\n";
        }
    }
    
    file.close();
}



/**
 * zapisywanie najlepszego scora na iteracje dla danej konfiguracji - DE 
 */
void submissionFile::saveIterationScores(const std::string& filename, 
    const std::vector<std::vector<double>>& iteration_scores, int num_configs) 
    {
    
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
        return;
    }
    
    file << "iteration";
    for (int config = 0; config < num_configs; ++config) 
    {
        file << ",config_" << (config + 1);
    }

    file << "\n";
    
    // wiersz - iteracja; kolumna - konfiguracja
    if (!iteration_scores.empty()) {
        int max_iterations = iteration_scores[0].size();
        
        for (int iter = 0; iter < max_iterations; ++iter) {
            file << iter;
            for (int config = 0; config < num_configs; ++config) {
                file << "," << std::fixed << std::setprecision(6) 
                     << iteration_scores[config][iter];
            }
            file << "\n";
        }
    }
    
    file.close();
}

// /**
//  * zaspis score dla każdej konfiguracji + suma pozostałych
//  */
// void submissionFile::saveConfigScoresWithSum( const std::string& filename, const std::vector<double>& config_scores) {
    
//     std::ofstream file(filename);
    
//     if (!file.is_open()) {
//         std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
//         return;
//     }
//     file << "config,score,sum_score\n";
    
//     double sum = 0.0;
    
//     for (int i = 0; i < config_scores.size(); ++i) {
//         sum += config_scores[i];
        
//         file << (i + 1) << ","
//              << std::fixed << std::setprecision(6) << config_scores[i] << ","
//              << std::fixed << std::setprecision(6) << sum << "\n";
//     }
//     file.close();

// }

/**
 * Zapis score dla każdej konfiguracji + suma
 * UWAGA: Może być używane zarówno dla config_scores jak i best_fitness_per_config
 */
void submissionFile::saveConfigScoresWithSum(
    const std::string& filename, 
    const std::vector<double>& config_scores,
    const std::string& score_column_name)  // ✅ NOWY PARAMETR - domyślnie "score"
{
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
        return;
    }
    
    
    file << "config," << score_column_name << ",sum_" << score_column_name << "\n";
    
    double sum = 0.0;
    
    for (int i = 0; i < config_scores.size(); ++i) {
        sum += config_scores[i];
        
        file << (i + 1) << ","
             << std::fixed << std::setprecision(6) << config_scores[i] << ","
             << std::fixed << std::setprecision(6) << sum << "\n";
    }
    
    file.close();
    std::cout << " Zapisano " << score_column_name << " do: " << filename << std::endl;
}
void submissionFile::saveIterationScoresExtended(
    const std::string& filename,
    const std::vector<std::vector<double>>& best_scores,
    const std::vector<std::vector<double>>& worst_scores,
    const std::vector<std::vector<double>>& avg_scores,
    int num_configs)
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
        return;
    }

    // Nagłówek CSV
    file << "config,iteration,best_fitness,worst_fitness,avg_fitness\n";

    // Dane dla każdej konfiguracji i iteracji
    for (int config = 0; config < num_configs; ++config) {
        for (size_t iter = 0; iter < best_scores[config].size(); ++iter) {
            file << (config + 1) << ","
                << (iter + 1) << ","
                << std::fixed << std::setprecision(6)
                << best_scores[config][iter] << ","
                << worst_scores[config][iter] << ","
                << avg_scores[config][iter] << "\n";
        }
    }

    file.close();
    std::cout << "Zapisano rozszerzone statystyki iteracji do: " << filename << std::endl;
}
