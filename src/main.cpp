#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <iomanip>
#include "GreedyPacker.h"

#include "CSVWriter.h"

#include "DE.h"
#include "submission_file.h"
#include <fstream>
#include <chrono> 
#include <sstream>
#include <iostream>
#include "ChristmasTree.h"
#include "simulated_annealing.h"
#include "structures.h"
#include "evaluation.h"
#include "Pso.h"
#include "Tabu.h"






    //-----------------DE --------------------------------
// int main() {
//
//
//     auto program_start = std::chrono::high_resolution_clock::now();
//
//
//     // DE parametry-----
//     int population_size = 30;
//     int num_trees = 10;           // liczba choinek // num_config
//     int max_iterations = 100;
//     double F = 0.7;                // 0.5-1.0
//     double CR = 0.85;               // 0.7-0.95
//
//
//
//     std::string rerun;
//     rerun = 1;
//
//
//     const std::string greedy_file = "greedy_solution.csv"; // plik greedy wejsciowy
//
//
//     std::stringstream config_name;
//     config_name << "pop" << population_size
//                 << "_trees" << num_trees
//                 << "_iter" << max_iterations
//                 << "_F" << std::fixed << std::setprecision(1) << F
//                 << "_CR" << std::fixed << std::setprecision(1) << CR;
//     std::string params_suffix = config_name.str();
//     const std::string output_dir = "../data/de_output/";
//
//
//     // ładowanie danych z greedy
//     Evaluation eval;
//     std::vector<SubmissionRow> greedy_data = eval.loadSubmissionFile(greedy_file);
//     std::map<int, std::vector<SubmissionRow>> grouped_by_config;
//
//     for (const auto& row : greedy_data) {
//             // wyciąganie nr konfiguracji
//             std::string config_str = row.id.substr(0, 3);
//             int config_num = std::stoi(config_str) - 1;
//
//             grouped_by_config[config_num].push_back(row);
//         }
//
//     // przygotowanie współrzednych i kątów z greedy dla DE
//     std::vector<std::vector<std::pair<double, double>>> frozen_positions;
//     std::vector<std::vector<float>> greedy_angles;
//
//     frozen_positions.resize(num_trees);
//     greedy_angles.resize(num_trees);
//
//
//     for (int config = 0; config < num_trees; ++config) {
//         if (grouped_by_config.find(config) != grouped_by_config.end()) {
//             const auto& trees = grouped_by_config[config];
//
//             for (const auto& tree : trees) {
//                 // pozycja x,y
//                 frozen_positions[config].emplace_back(tree.x, tree.y);
//
//                 // znormalizowany kąt
//                 double normalized_angle = tree.deg / 360.0;
//                 greedy_angles[config].push_back(normalized_angle);
//             }
//
//         }
//     }
//
//
//
//
//     auto de_start = std::chrono::high_resolution_clock::now(); // pomiar czasu
//
//     // std::cout << "\n------------------------ DE-----------------------" << std::endl;
//
//     DE de (population_size, num_trees, max_iterations, F, CR);
//     std::cout << "DE zainicjalizowane" << std::endl;
//     de.setFrozenPositions(frozen_positions);
//
//
//     // odpalanie de
//     std::vector<float> best_solution = de.run(num_trees, greedy_angles);
//
//     auto de_end = std::chrono::high_resolution_clock::now();
//     auto de_duration = std::chrono::duration_cast<std::chrono::seconds>(de_end - de_start);
//     auto de_duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(de_end - de_start);
//
//     std::cout << "\nCzas optymalizacji DE: " << de_duration.count() << " s ("
//               << de_duration_ms.count() << " ms)" << std::endl;
//
//     // konwert katow
//     std::vector<double> best_angles_degrees(best_solution.size());
//     for (size_t i = 0; i < best_solution.size(); ++i) {
//         best_angles_degrees[i] = best_solution[i] * 360.0;
//     }
//     const std::vector<std::vector<double>>& best_angles = de.getBestAnglesPerConfig();
//     const std::vector<std::vector<std::pair<double, double>>>& best_positions = de.getBestPositionsPerConfig();
//     const std::vector<double>& best_fitness_per_config = de.getBestFitnessPerConfig();
//     const std::vector<std::vector<double>>& iteration_worst_scores = de.getIterationWorstScores();
//     const std::vector<std::vector<double>>& iteration_avg_scores = de.getIterationAvgScores();
//
//     // sciagnie scores z de
//     const std::vector<double>& config_scores = de.getConfigScores();
//
//
//     // oblcizanie total score
//     double total_score = 0.0;
//     for (int i = 0; i < best_fitness_per_config.size(); ++i) {
//         total_score += best_fitness_per_config[i];
//     }
//
//     // zapis wynikow
//     submissionFile submFile;
//
//     // rozwiazanie parametrow (id, x, y, deg)
//     submFile.saveSolutionToCSV(
//         output_dir + "de_solution_" + params_suffix + "_final_" + rerun + ".csv",
//         best_solution,
//         de.getFrozenPositions(),
//         num_trees,
//         best_angles,
//         best_positions
//     );
//
//     const std::vector<std::vector<double>>& iteration_scores = de.getIterationScores();
//
//     submFile.saveIterationScoresExtended(
//         output_dir + "de_iteration_stats_" + params_suffix + "_final_" + rerun + ".csv",
//         iteration_scores,       // best
//         iteration_worst_scores, // worst
//         iteration_avg_scores,   // avg
//         num_trees
//     );
//
//
//     submFile.saveConfigScoresWithSum(
//         output_dir + "de_config_scores_" + params_suffix + "_final_" + rerun + ".csv",
//         best_fitness_per_config
//
//     );
//
//     std::cout << "\n Porownaie z greedy" << std::endl;
//
//     double greedy_total_score = 0.0;
//     DE de_greedy(population_size, num_trees, 0, F, CR);
//     de_greedy.setFrozenPositions(frozen_positions);
//
//     for (int config = 0; config < num_trees; ++config) {
//         std::vector<float> greedy_angles_flat(num_trees, 0.0);
//         for (int i = 0; i <= config; ++i) {
//             if (i < greedy_angles[config].size()) {
//                 greedy_angles_flat[i] = greedy_angles[config][i];
//             }
//         }
//
//         double greedy_score = de_greedy.evaluateFitnessForConfig(greedy_angles_flat, config);
//         greedy_total_score += greedy_score;
//     }
//
//     std::cout << "Greedy total score = " << std::fixed << std::setprecision(6) << greedy_total_score << std::endl;
//     std::cout << "DE total score = " << std::fixed << std::setprecision(6) << total_score << std::endl;
//     std::cout << "Poprawa: " << std::fixed << std::setprecision(6)<< (greedy_total_score - total_score) << " (" << std::fixed << std::setprecision(2)<< ((greedy_total_score - total_score) / greedy_total_score * 100.0) << "%)" << std::endl;
//
//     auto program_end = std::chrono::high_resolution_clock::now();
//     auto total_duration = std::chrono::duration_cast<std::chrono::seconds>(program_end - program_start);
//
//     std::cout << "\n ----CZAS-----" << std::endl;
//
//     std::cout << "DE " << de_duration.count() << " s " << std::endl;

//-----------------DE  koniec --------------------------------


  
  
  
  
//-----------------SA  --------------------------------

// using namespace std;
//
// TreeSolution trees_to_solution(const vector<shared_ptr<ChristmasTree>>& trees,
//                                const string& group_name) {
//     TreeSolution solution;
//
//     for (const auto& tree : trees) {
//         solution.positions_x.push_back(tree->getX());
//         solution.positions_y.push_back(tree->getY());
//         solution.angles.push_back(tree->getAngle());
//     }
//
//     Evaluation eval;
//     solution.fitness = evaluate_tree_solution(solution, eval, group_name);
//
//     return solution;
// }
//
// string get_timestamp() {
//     auto now = chrono::system_clock::now();
//     auto time_t_now = chrono::system_clock::to_time_t(now);
//     stringstream ss;
//     ss << put_time(localtime(&time_t_now), "%Y%m%d_%H%M%S");
//     return ss.str();
// }
//
// vector<shared_ptr<ChristmasTree>> solution_to_trees(const TreeSolution& solution) {
//     vector<shared_ptr<ChristmasTree>> trees;
//
//     for (size_t i = 0; i < solution.angles.size(); ++i) {
//         auto tree = make_shared<ChristmasTree>(
//             solution.positions_x[i],
//             solution.positions_y[i],
//             solution.angles[i]
//         );
//         trees.push_back(tree);
//     }
//
//     return trees;
// }
//
// int calculate_max_iterations(int num_trees, int target_ffe = 3000) {
//     int neighborhood_size = 5;
//
//     const int local_search_deltas = 6;
//     int local_search_ffe = num_trees * local_search_deltas;
//
//     int max_iterations = (target_ffe - local_search_ffe) / neighborhood_size;
//
//     return max_iterations;
// }
//
// int main() {
//
//     auto program_start = std::chrono::high_resolution_clock::now();
//     const int MAX_TREES = 2;
//     string timestamp = get_timestamp();
//     const int TARGET_FFE = 3000;
//
//     double T0 = 200.0;
//     double alpha = 0.98;
//
//     ofstream scores_file("results/configuration_scores_" + timestamp + ".csv");
//     scores_file << "num_trees,greedy_score,sa_score,improvement,improvement_pct,time_seconds,max_iterations,actual_ffe\n";
//
//     cout << "\n=== Running Greedy for " << MAX_TREES << " trees ===" << endl;
//
//     GreedyPacker packer;
//     auto result = packer.packTrees(MAX_TREES);
//     vector<shared_ptr<ChristmasTree>> all_greedy_trees = result.first;
//
//     vector<TreeSolution> all_sa_solutions;
//     all_sa_solutions.reserve(MAX_TREES);
//
//     double total_score_greedy = 0.0;
//     double total_score_sa = 0.0;
//
//     for (int num_trees = 1; num_trees <= MAX_TREES; ++num_trees) {
//         cout << "\n=== Optimizing group " << num_trees << " trees ===" << endl;
//
//         string group_name = "g" + to_string(num_trees);
//
//         auto start_time = chrono::high_resolution_clock::now();
//
//         vector<shared_ptr<ChristmasTree>> greedy_trees(
//             all_greedy_trees.begin(),
//             all_greedy_trees.begin() + num_trees
//         );
//
//         TreeSolution greedy_solution = trees_to_solution(greedy_trees, group_name);
//
//         total_score_greedy += greedy_solution.fitness;
//
//         int max_iterations = calculate_max_iterations(num_trees, TARGET_FFE);
//         int neighborhood_size = 5;
//         int local_search_deltas = 6;
//         int actual_ffe = max_iterations * neighborhood_size + num_trees * local_search_deltas;
//
//
//         TreeSolution sa_solution = simulated_annealing(
//             greedy_solution,
//             num_trees,
//             T0,
//             alpha,
//             max_iterations,
//             group_name
//         );
//
//         all_sa_solutions.push_back(sa_solution);
//
//         total_score_sa += sa_solution.fitness;
//
//         double improvement = greedy_solution.fitness - sa_solution.fitness;
//         double improvement_pct = 100.0 * improvement / greedy_solution.fitness;
//
//         auto end_time = chrono::high_resolution_clock::now();
//         auto duration = chrono::duration_cast<chrono::seconds>(end_time - start_time);
//
//         scores_file << num_trees << ","
//                    << fixed << setprecision(6) << greedy_solution.fitness << ","
//                    << sa_solution.fitness << ","
//                    << improvement << ","
//                    << improvement_pct << ","
//                    << duration.count() << ","
//                    << max_iterations << ","
//                    << actual_ffe << "\n";
//         scores_file.flush();
//
//         if (num_trees % 10 == 0 || num_trees == MAX_TREES) {
//             cout << "\n--- Progress: " << num_trees << "/" << MAX_TREES << " completed ---" << endl;
//             cout << "Current cumulative greedy score: " << fixed << setprecision(6) << total_score_greedy << endl;
//             cout << "Current cumulative SA score:     " << total_score_sa << endl;
//             cout << "Current cumulative improvement:  " << (total_score_greedy - total_score_sa)
//                  << " (" << (100.0 * (total_score_greedy - total_score_sa) / total_score_greedy) << "%)" << endl;
//         }
//     }
//
//     scores_file.close();
//
// string sa_filename = "submission_sa_" + timestamp + ".csv";
// ofstream sa_file(sa_filename);
// sa_file << "id,x,y,deg\n";
//
// for (int config = 0; config < MAX_TREES; ++config) {
//     int num_trees = config + 1;
//     const TreeSolution& solution = all_sa_solutions[config];
//
//     stringstream group_ss;
//     group_ss << setfill('0') << setw(3) << num_trees;
//     string group_id = group_ss.str();
//
//     for (int tree_idx = 0; tree_idx < num_trees; ++tree_idx) {
//         sa_file << group_id << "_" << tree_idx << ","
//                << "s" << fixed << setprecision(6)
//                << solution.positions_x[tree_idx] << ","
//                << "s" << solution.positions_y[tree_idx] << ","
//                << "s" << solution.angles[tree_idx] << "\n";
//     }
// }
// sa_file.close();
//
// string greedy_filename = "submission_greedy_" + timestamp + ".csv";
// ofstream greedy_file(greedy_filename);
// greedy_file << "id,x,y,deg\n";
//
// for (int config = 0; config < MAX_TREES; ++config) {
//     int num_trees = config + 1;
//
//     stringstream group_ss;
//     group_ss << setfill('0') << setw(3) << num_trees;
//     string group_id = group_ss.str();
//
//     for (int tree_idx = 0; tree_idx < num_trees; ++tree_idx) {
//         const auto& tree = all_greedy_trees[tree_idx];
//         greedy_file << group_id << "_" << tree_idx << ","
//                    << "s" << fixed << setprecision(6)
//                    << tree->getX() << ","
//                    << "s" << tree->getY() << ","
//                    << "s" << tree->getAngle() << "\n";
//     }
// }
// greedy_file.close();
//
//     cout << fixed << setprecision(6);
//     cout << "GREEDY TOTAL SCORE: " << total_score_greedy << endl;
//     cout << "SA TOTAL SCORE:     " << total_score_sa << endl;
//
//     double total_improvement = total_score_greedy - total_score_sa;
//     double total_improvement_pct = 100.0 * total_improvement / total_score_greedy;
//
//     cout << "\nTOTAL IMPROVEMENT:  " << total_improvement
//          << " (" << total_improvement_pct << "%)" << endl;
//
//
//     try {
//         Evaluation eval;
//
//         auto submission_greedy = eval.loadSubmissionFile(greedy_filename);
//         double score_greedy_file = eval.evaluation_score(submission_greedy);
//         cout << "Greedy from file:   " << score_greedy_file << endl;
//         cout << "Greedy from sum:    " << total_score_greedy << endl;
//         cout << "Difference:         " << abs(score_greedy_file - total_score_greedy) << endl;
//
//
//         cout << endl;
//
//         auto submission_sa = eval.loadSubmissionFile(sa_filename);
//         double score_sa_file = eval.evaluation_score(submission_sa);
//         cout << "SA from file:       " << score_sa_file << endl;
//         cout << "SA from sum:        " << total_score_sa << endl;
//         cout << "Difference:         " << abs(score_sa_file - total_score_sa) << endl;
//
//
//         auto program_end = std::chrono::high_resolution_clock::now();
//         auto total_duration = std::chrono::duration_cast<std::chrono::seconds>(program_end - program_start);
//
//         cout << "Total time: " << total_duration.count() << endl;
//
//     } catch (const exception& e) {
//         cerr << "Error during verification: " << e.what() << endl;
//     }
    
    //-----------------SA  koniec --------------------------------
  
  
  
  
  // ------------------- TS -------------------------------------------------------------
      /// Tabu - wyniki dla n=10,50,100,200; FFE = 3k
int main() {
    std::cout << "TABU SEARCH URUCHOMIONE" << std::endl;
         int runsPerTest = 10;
         int numberOfIterations = 10; // 200
         int tabuListLength = 8;
         float ballRadius = 1.8; //10%
         int neighborhoodSize = 15;
         GreedyPacker packer;
         int seed = 9;
         double sigma = 15;
         int numberOfTrees =5;
         int k = 5;
         //k = 10, tLL=12 dla 100
         //k = 5, tLL=8 dla 50 (zobacz 10 też)
         //k = 1, tLL=4 dla 10

     Tabu tabu(numberOfIterations, tabuListLength, neighborhoodSize, ballRadius, packer, seed);
     TabuFullHistory result = tabu.algorithmWithScoresKFinal(numberOfTrees, sigma, k);

     std::ofstream treeFile("data/ts_output/finalResults/finalTrees10Coords.csv");
     if (treeFile.is_open()) {
         treeFile << "tree_x,tree_y,tree_angle\n";
         for (const auto& t : result.trees) {
             treeFile << t.x << "," << t.y << "," << t.angle << "\n";
         }
         treeFile.close();
     }

     std::ofstream scoreFile("data/ts_output/finalResults/finalTrees10Plot.csv");
     if (scoreFile.is_open()) {
         scoreFile << "iteration,global_best,current_score\n";
         for (const auto& s : result.scores) {
             scoreFile << s.iteration << "," << s.globalBestScore << "," << s.iterationBestScore << "\n";
         }
         scoreFile.close();
     }
      
  // ------------------- TS  koniec -------------------------------------------------------------


    return 0;
}
