#include "ChristmasTree.h"
#include <iostream>
#include <iomanip>
#include "GreedyPacker.h"
#include "CSVWriter.h"
#include "DE.h"
#include "submission_file.h"
#include <fstream>
#include <chrono> 
#include <sstream>
#include <iostream>


int main(int argc, char* argv[]) {



    //-----------------DE --------------------------------

    // Rozpoczęcie pomiaru całkowitego czasu
    auto program_start = std::chrono::high_resolution_clock::now();


    // DE parametry-----
    int population_size = 30;     
    int num_trees = 10;           // liczba choinek // num_config
    int max_iterations = 100;     
    double F = 0.7;                // 0.5-1.0
    double CR = 0.85;               // 0.7-0.95



    std::string rerun;
    rerun = argv[1];


    const std::string greedy_file = "greedy_solution.csv"; // plik greedy wejsciowy


    std::stringstream config_name;
    config_name << "pop" << population_size 
                << "_trees" << num_trees 
                << "_iter" << max_iterations 
                << "_F" << std::fixed << std::setprecision(1) << F 
                << "_CR" << std::fixed << std::setprecision(1) << CR;
    std::string params_suffix = config_name.str();
    const std::string output_dir = "../data/de_output/";

    // ładowanie danych z greedy
    Evaluation eval;
    std::vector<SubmissionRow> greedy_data = eval.loadSubmissionFile(greedy_file);
    std::map<int, std::vector<SubmissionRow>> grouped_by_config;

    for (const auto& row : greedy_data) {
            // wyciąganie nr konfiguracji
            std::string config_str = row.id.substr(0, 3);
            int config_num = std::stoi(config_str) - 1;
            
            grouped_by_config[config_num].push_back(row);
        }

    // przygotowanie współrzednych i kątów z greedy dla DE
    std::vector<std::vector<std::pair<double, double>>> frozen_positions;
    std::vector<std::vector<float>> greedy_angles;
        
    frozen_positions.resize(num_trees);
    greedy_angles.resize(num_trees);


    for (int config = 0; config < num_trees; ++config) {
        if (grouped_by_config.find(config) != grouped_by_config.end()) {
            const auto& trees = grouped_by_config[config];
                
            for (const auto& tree : trees) {
                // pozycja x,y
                frozen_positions[config].emplace_back(tree.x, tree.y);
                    
                // znormalizowany kąt
                double normalized_angle = tree.deg / 360.0;
                greedy_angles[config].push_back(normalized_angle);
            }
            
        }
    }




    auto de_start = std::chrono::high_resolution_clock::now(); // pomiar czasu

    // std::cout << "\n------------------------ DE-----------------------" << std::endl;

    DE de (population_size, num_trees, max_iterations, F, CR);
    std::cout << "DE zainicjalizowane" << std::endl;
    de.setFrozenPositions(frozen_positions);


    // odpalanie de
    std::vector<float> best_solution = de.run(num_trees, greedy_angles);
    
    auto de_end = std::chrono::high_resolution_clock::now();
    auto de_duration = std::chrono::duration_cast<std::chrono::seconds>(de_end - de_start);
    auto de_duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(de_end - de_start);
    
    std::cout << "\nCzas optymalizacji DE: " << de_duration.count() << " s (" 
              << de_duration_ms.count() << " ms)" << std::endl;
        
    // konwert katow
    std::vector<double> best_angles_degrees(best_solution.size());
    for (size_t i = 0; i < best_solution.size(); ++i) {
        best_angles_degrees[i] = best_solution[i] * 360.0;
    }
    const std::vector<std::vector<double>>& best_angles = de.getBestAnglesPerConfig();
    const std::vector<std::vector<std::pair<double, double>>>& best_positions = de.getBestPositionsPerConfig();
    const std::vector<double>& best_fitness_per_config = de.getBestFitnessPerConfig();
    const std::vector<std::vector<double>>& iteration_worst_scores = de.getIterationWorstScores();
    const std::vector<std::vector<double>>& iteration_avg_scores = de.getIterationAvgScores();

    // sciagnie scores z de    
    const std::vector<double>& config_scores = de.getConfigScores();

    
    // oblcizanie total score
    double total_score = 0.0;
    for (int i = 0; i < best_fitness_per_config.size(); ++i) {
        total_score += best_fitness_per_config[i];
    }

    // zapis wynikow
    submissionFile submFile;
    
    // rozwiazanie parametrow (id, x, y, deg)
    submFile.saveSolutionToCSV(
        output_dir + "de_solution_" + params_suffix + "_final_" + rerun + ".csv",
        best_solution,
        de.getFrozenPositions(),
        num_trees,
        best_angles,             
        best_positions           
    );
    
    const std::vector<std::vector<double>>& iteration_scores = de.getIterationScores();

    submFile.saveIterationScoresExtended(
        output_dir + "de_iteration_stats_" + params_suffix + "_final_" + rerun + ".csv",
        iteration_scores,       // best
        iteration_worst_scores, // worst
        iteration_avg_scores,   // avg
        num_trees
    );


    submFile.saveConfigScoresWithSum(
        output_dir + "de_config_scores_" + params_suffix + "_final_" + rerun + ".csv",
        best_fitness_per_config
        
    );
    
    std::cout << "\n Porownaie z greedy" << std::endl;
    
    double greedy_total_score = 0.0;
    DE de_greedy(population_size, num_trees, 0, F, CR);
    de_greedy.setFrozenPositions(frozen_positions);
    
    for (int config = 0; config < num_trees; ++config) {
        std::vector<float> greedy_angles_flat(num_trees, 0.0);
        for (int i = 0; i <= config; ++i) {
            if (i < greedy_angles[config].size()) {
                greedy_angles_flat[i] = greedy_angles[config][i];
            }
        }
        
        double greedy_score = de_greedy.evaluateFitnessForConfig(greedy_angles_flat, config);
        greedy_total_score += greedy_score;
    }

    std::cout << "Greedy total score = " << std::fixed << std::setprecision(6) << greedy_total_score << std::endl;
    std::cout << "DE total score = " << std::fixed << std::setprecision(6) << total_score << std::endl;
    std::cout << "Poprawa: " << std::fixed << std::setprecision(6)<< (greedy_total_score - total_score) << " (" << std::fixed << std::setprecision(2)<< ((greedy_total_score - total_score) / greedy_total_score * 100.0) << "%)" << std::endl;
    
    auto program_end = std::chrono::high_resolution_clock::now();
    auto total_duration = std::chrono::duration_cast<std::chrono::seconds>(program_end - program_start);
    
    std::cout << "\n ----CZAS-----" << std::endl;

    std::cout << "DE " << de_duration.count() << " s " << std::endl;

    //-----------------DE  koniec --------------------------------

    return 0;
}
