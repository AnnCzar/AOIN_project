// #include "structures.h"
// #include "simulated_annealing.h"
// #include "evaluation.h"
// #include "operators.h"
// #include "GreedyPacker.h"
// #include "CSVWriter.h"
// #include <chrono>
// #include <fstream>
// #include <random>
// #include <cmath>
// #include <iomanip>
// #include <sstream>
//
// using namespace std;
//
// thread_local vector<SubmissionRow> g_submission_buffer;
//
// vector<SubmissionRow> convert_to_submission(const TreeSolution& solution, const string& group_name) {
//     g_submission_buffer.clear();
//     g_submission_buffer.reserve(solution.angles.size());
//
//     for (size_t i = 0; i < solution.angles.size(); ++i) {
//         SubmissionRow row;
//         row.id = group_name + "_" + to_string(i);
//         row.x = solution.positions_x[i];
//         row.y = solution.positions_y[i];
//         row.deg = solution.angles[i];
//         g_submission_buffer.push_back(row);
//     }
//
//     return g_submission_buffer;
// }
//
// double evaluate_tree_solution(const TreeSolution& solution, Evaluation& eval, const string& group_name) {
//     try {
//        const auto& submission = convert_to_submission(solution, group_name);
//         return eval.evaluation_score(submission);
//     } catch (const runtime_error& e) {
//         cerr << "ERROR in evaluate_tree_solution: " << e.what() << endl;
//         return 1e9;
//     }
// }
//
// TreeSolution local_search(const TreeSolution& solution, int num_trees,
//                          Evaluation& eval, vector<double>& deltas, const string& filename) {
//     TreeSolution best = solution;
//     TreeSolution candidate;
//
//     candidate.angles.reserve(num_trees);
//     candidate.positions_x.reserve(num_trees);
//     candidate.positions_y.reserve(num_trees);
//
//     for (int tree_idx = 0; tree_idx < num_trees; ++tree_idx) {
//         double original_angle = best.angles[tree_idx];
//
//         for (double delta : deltas) {
//             candidate = best;
//             candidate.angles[tree_idx] = fmod(original_angle + delta, 360.0);
//             if (candidate.angles[tree_idx] < 0.0) candidate.angles[tree_idx] += 360.0;
//
//             candidate.fitness = eval.evaluation_score(
//                 convert_to_submission(candidate, filename));
//
//             if (candidate.fitness < best.fitness) {
//                 best = candidate;
//             }
//         }
//     }
//
//     return best;
// }
//
// TreeSolution local_search_greedy(const TreeSolution& solution, int num_trees,
//                          Evaluation& eval, vector<double>& deltas, const string& filename) {
//     TreeSolution best = solution;
//     TreeSolution candidate;
//     GreedyPacker packer;
//
//     candidate.angles.reserve(num_trees);
//     candidate.positions_x.reserve(num_trees);
//     candidate.positions_y.reserve(num_trees);
//
//     for (int tree_idx = 0; tree_idx < num_trees; ++tree_idx) {
//         double original_angle = best.angles[tree_idx];
//
//         for (double delta : deltas) {
//             candidate = best;
//             candidate.angles[tree_idx] = fmod(original_angle + delta, 360.0);
//             if (candidate.angles[tree_idx] < 0.0) candidate.angles[tree_idx] += 360.0;
//
//             auto [new_trees, sides] = packer.packTreesWithFixedAnglesSA(candidate.angles);
//             candidate.positions_x.clear();
//             candidate.positions_y.clear();
//             for (const auto& tree : new_trees) {
//                 candidate.positions_x.push_back(tree->getX());
//                 candidate.positions_y.push_back(tree->getY());
//             }
//
//             candidate.fitness = eval.evaluation_score(
//                 convert_to_submission(candidate, filename));
//
//             if (candidate.fitness < best.fitness) {
//                 best = candidate;
//             }
//         }
//     }
//
//     return best;
// }
//
// vector<TreeSolution> createNeighborhood(const TreeSolution& current,
//                                        double sigma,
//                                        int neighborhood_size,
//                                        int greedy_frequency,
//                                        int iteration,
//                                        GreedyPacker& packer) {
//
//     static vector<TreeSolution> neighborhood;
//     neighborhood.clear();
//     neighborhood.reserve(neighborhood_size);
//
//     for (int i = 0; i < neighborhood_size; ++i) {
//         TreeSolution neighbor = current;
//         neighbor.angles = rotate_tree(current.angles, sigma);
//
//         if (iteration % greedy_frequency == 0) {
//             auto [new_trees, sides] = packer.packTreesWithFixedAnglesSA(neighbor.angles);
//             neighbor.positions_x.clear();
//             neighbor.positions_y.clear();
//             neighbor.positions_x.reserve(new_trees.size());
//             neighbor.positions_y.reserve(new_trees.size());
//
//             for (size_t idx = 0; idx < new_trees.size(); ++idx) {
//                 neighbor.positions_x.push_back(new_trees[idx]->getX());
//                 neighbor.positions_y.push_back(new_trees[idx]->getY());
//                 neighbor.angles[idx] = new_trees[idx]->getAngle();
//             }
//         } else {
//             neighbor.positions_x = current.positions_x;
//             neighbor.positions_y = current.positions_y;
//         }
//
//         neighborhood.push_back(std::move(neighbor));
//     }
//
//     return neighborhood;
// }
//
//
// TreeSolution getBestNeighbor(vector<TreeSolution>& neighborhood,
//                             Evaluation& eval,
//                             const string& filename) {
//     TreeSolution* best_neighbor = &neighborhood[0];
//     double best_fitness = 1e9;
//
//     #pragma omp parallel for
//     for (int i = 0; i < neighborhood.size(); ++i) {
//         neighborhood[i].fitness = evaluate_tree_solution(neighborhood[i], eval, filename);
//     }
//
//     for (auto& neighbor : neighborhood) {
//         if (neighbor.fitness < best_fitness) {
//             best_fitness = neighbor.fitness;
//             best_neighbor = &neighbor;
//         }
//     }
//
//     return *best_neighbor;
// }
//
//
// TreeSolution simulated_annealing(const TreeSolution& start_solution, int num_trees,
//                                 double T0, double alpha, int max_iterations,
//                                 const string& filename, int neighborhood_size) {
//
//     ostringstream ss_T0, ss_alpha;
//
//     auto now = chrono::system_clock::now();
//     auto time_t_now = chrono::system_clock::to_time_t(now);
//     stringstream ss;
//     ss << put_time(localtime(&time_t_now), "%Y%m%d_%H%M%S");
//
//
//     ss_T0 << fixed << setprecision(2) << T0;
//     ss_alpha << fixed << setprecision(3) << alpha;
//     string output_filename = "results/sa_log_" + filename + "_T" + ss_T0.str() +
//                             "_A" + ss_alpha.str() + "_maxI" + to_string(max_iterations) +
//                             "_NS" + to_string(neighborhood_size) + ss.str() + ".csv";
//
//     ofstream log(output_filename);
//     log << "iteration;current_fitness;best_fitness;temperature\n";
//
//     TreeSolution current = start_solution;
//     TreeSolution best = current;
//     double T = T0;
//
//     random_device rd;
//     mt19937 gen(rd());
//     uniform_real_distribution<> dist(0.0, 1.0);
//
//     Evaluation eval;
//     GreedyPacker packer;
//
//     int greedy_frequency;
//     if (num_trees <= 10) {
//         greedy_frequency = 1;
//     } else if (num_trees <= 30) {
//         greedy_frequency = 25;
//     } else {
//         greedy_frequency = 50;
//     }
//
//     double sigma = 5.0 + (T / T0) * 40.0;
//     log << fixed << setprecision(6);
//
//
//     for (int i = 0; i < max_iterations; ++i) {
//         double temp_factor = T / T0;
//
//        auto neighborhood = createNeighborhood(
//             current, sigma, neighborhood_size, greedy_frequency, i, packer
//         );
//
//         TreeSolution best_neighbor = getBestNeighbor(neighborhood, eval, filename);
//
//         double delta = best_neighbor.fitness - current.fitness;
//
//         if (delta < 0) {
//             current = std::move(best_neighbor);
//             if (current.fitness < best.fitness) {
//                 best = current;
//             }
//         } else {
//             double acceptance_prob = exp(-delta / T);
//             if (dist(gen) < acceptance_prob) {
//                 current = std::move(best_neighbor);
//             }
//         }
//
//         if (i % 1 == 0) {
//             log << i << ";"
//                 << current.fitness << ";"
//                 << best.fitness << ";"
//                 << T << "\n";
//         }
//
//         T = T * alpha;
//     }
//
//     log.flush();
//     log.close();
//
//     static const vector<double> deltas = {-5, -2, -1, 1, 2, 5};
//
//     if (num_trees <= 30) {
//         best = local_search_greedy(best, num_trees, eval, const_cast<vector<double>&>(deltas), filename);
//     } else {
//         best = local_search(best, num_trees, eval, const_cast<vector<double>&>(deltas), filename);
//     }
//
//     return best;
// }