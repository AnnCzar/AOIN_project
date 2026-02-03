#include "DE.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <omp.h>


DE::DE( int population_size, int num_trees,int max_iterations,double F,double CR):

    population_size(population_size),
    num_trees(num_trees),
    max_iterations(max_iterations),
    F(F),
    CR(CR),
    rng(std::random_device{}()),
    uniform_dist(0.0, 1.0),
    best_fitness(1e9),
    num_configs(0) 
    {
        population.resize(population_size, std::vector<float>(num_trees)); 
        fitness.resize(population_size);
        best_individual.resize(num_trees);

        mutant_buffer.resize(num_trees);
        child_buffer.resize(num_trees);
        trees_cache.reserve(num_trees);

    }


/**
 * Inicjalizacja populacji 
 * 
 * current_config -- numer konfiguracji
 * window_start -- to indesk pierwszej choinki w oknie
 * window_size -- ile jest choinek w oknie
 * greedy_angles -- kąty z greedy
 */
void DE::initializePopulationWithWindow( int current_config, int window_start, int window_size,
                                        const std::vector<std::vector<float>>& greedy_angles) {
    
    int num_trees_in_config = current_config + 1;


    
    for (int i = 0; i < population_size; ++i) {

        // katy przed oknem optymalizacyjnym są zamrozone -- nie ruszam ich -- juz zoptymalizowane
        for (int j = 0; j < window_start; ++j) {
            population[i][j] = optimized_angles[j];
        }
        
        // okno optymalizacji 
        if (i == 0 && !greedy_angles.empty()) { // osobnik 0 zawsze ma katy z greedy
            
            for (int j = window_start; j <= current_config; ++j) {
                if (j < greedy_angles.size() && !greedy_angles[j].empty()) {
                    int last_idx = greedy_angles[j].size() - 1;
                    population[0][j] = greedy_angles[j][last_idx];
                } else {
                    population[0][j] = optimized_angles[j];
                } 
            }
        } else {
            // reszta 
            for (int j = window_start; j <= current_config; ++j) {
                if (j < current_config) {
                    // dodanie małych zabużen

                     double perturbation;
                    
                    // if (i < population_size / 2) {
                    //     // polow populacje malezabuzenia
                    //     perturbation = (uniform_dist(rng) - 0.5) * 0.1; // +-5%
                    // } else {
                    //     // druga polowa wieksze zabużenia
                    //     perturbation = (uniform_dist(rng) - 0.5) * 0.2; // +-10%
                    // }

                    if (i < population_size / 2) {
                        std::normal_distribution<double> normal_small(0.0, 0.05);
                        perturbation = std::round(normal_small(rng)* 1000) / 1000.0;
                        // std::cout << "peturbacja mala = " << perturbation << std::endl;
                        population[i][j] = optimized_angles[j] + perturbation;
                    } else {
                        std::normal_distribution<double> normal_large(0.0, 0.10);
                        perturbation = std::round(normal_large(rng)* 1000) / 1000.0;
                        // std::cout << "peturbacja duza = " << perturbation << std::endl;
                        population[i][j] = optimized_angles[j] + perturbation;
                    }
                    
                    if (population[i][j] < 0.0) population[i][j] += 1.0;
                    if (population[i][j] >= 1.0) population[i][j] -= 1.0;
                    
                    population[i][j] = std::round(population[i][j] * 1000) / 1000.0;
                } else {

                    // nowy kat: losuj
                    population[i][j] = uniform_dist(rng);
                    population[i][j] = std::round(population[i][j] * 1000) / 1000.0;
                }
            }
        }
        
        // poza oknem 0
        for (int j = num_trees_in_config; j < num_trees; ++j) {
            population[i][j] = 0.0;
        }

        // std::cout << "Osobnik " << i << " [";

        // for (int j = 0; j < population[i].size(); j++) {
        //     std::cout << population[i][j];
        //     if (j != population[i].size() - 1)
        //         std::cout << ", ";
        // }

        // std::cout << "]" << std::endl;
    }

    
    // std::cout << "Populacja zainicjalizowana (okno: " << window_size << ")" << std::endl;
}

void DE::setFrozenPositions(
    const std::vector<std::vector<std::pair<double, double>>>& positions) {
    
    frozen_positions = positions;
    num_configs = positions.size();
    
    std::cout << "Zamrozone pozycje ustawione." << std::endl;

}

float DE::evaluateFitnessForConfig(const std::vector<float>& angles, int config) {
    int num_trees_in_config = config + 1;

    if (frozen_positions[config].size() < num_trees_in_config) {
        return 1e9;
    }

    trees_cache.clear();
    for (int tree_id = 0; tree_id < num_trees_in_config; ++tree_id) {
        double x = frozen_positions[config][tree_id].first;
        double y = frozen_positions[config][tree_id].second;
        double normalized_angle = angles[tree_id];
        double angle_deg = denormalizeAngle(normalized_angle);

        trees_cache.emplace_back(x, y, angle_deg);
    }

    //liczenie kolizji
    int collision_count = 0;
    for (int i = 0; i < trees_cache.size(); ++i) {
        for (int j = i + 1; j < trees_cache.size(); ++j) {
            if (trees_cache[i].intersects(trees_cache[j])) {
                if (!trees_cache[i].getPolygon()->touches(trees_cache[j].getPolygon())) {
                    collision_count++;
                }
            }
        }
    }

//obliczanie score
    double min_x = std::numeric_limits<double>::max();
    double max_x = std::numeric_limits<double>::lowest();
    double min_y = std::numeric_limits<double>::max();
    double max_y = std::numeric_limits<double>::lowest();

    for (const auto& tree : trees_cache) {
        const geos::geom::Envelope* env = tree.getEnvelope();
        min_x = std::min(min_x, env->getMinX());
        max_x = std::max(max_x, env->getMaxX());
        min_y = std::min(min_y, env->getMinY());
        max_y = std::max(max_y, env->getMaxY());
    }

    double side_length_scaled = std::max(max_x - min_x, max_y - min_y);
    double scale_factor = trees_cache[0].getScaleFactor();

    double base_score = ((side_length_scaled * side_length_scaled) /
        (scale_factor * scale_factor)) /
        static_cast<double>(num_trees_in_config);

    // kara  za kolizje
    if (collision_count > 0) {
        // liczba kolizji * 10 
        double penalty = base_score * 10.0 * collision_count;
        return base_score + penalty;

    }

    return base_score;
}


int DE::getWindowSize(int current_config)  {
    if (current_config < 5) {
        return current_config + 1;  // wszystkie katy 1-5 choinek
    } else if (current_config < 15) {
        return 5;  // 5
    } else if (current_config < 50) {
        return 8;  //8
    } else if (current_config < 100) {
        return 10; //10
    } else {
        return 15;  //15
    }
}


std::vector<float> DE::run(int num_configs2, const std::vector<std::vector<float>>& greedy_angles){


    this->num_configs = num_configs2;
    optimized_angles.resize(num_trees, 0.0);  // zoptymalizowane już kąty
    frozen_genes.resize(num_trees, false);   // które katy są juz zoptymalizowane

    iteration_scores.clear();
    iteration_scores.resize(num_configs);  // do zapisu score z konfiguracji

    best_angles_per_config.clear();
    best_angles_per_config.resize(num_configs);
    iteration_worst_scores.clear();
    iteration_worst_scores.resize(num_configs);

    iteration_avg_scores.clear();
    iteration_avg_scores.resize(num_configs);
    
    best_positions_per_config.clear();
    best_positions_per_config.resize(num_configs);
    
    best_fitness_per_config.clear();
    best_fitness_per_config.resize(num_configs, 1e9);
    

    // petla po konfiguracjach
    for (int current_config = 0; current_config < num_configs; ++current_config) {
        int num_trees_to_optimize = current_config + 1;  // +1 bo config 0 to 1 choinka
        int num_trees_in_config = current_config + 1;

        // rozmiaar okna optymalizacji
        int window_size = getWindowSize(current_config);
        int window_start = std::max(0, current_config - window_size + 1);
        
        config_scores.clear();
        config_scores.resize(num_configs, 0.0);

        // std::cout << "  Populacja: " << population_size << std::endl;
        // std::cout << "  Zmienne optymalizowane: " << num_trees_to_optimize << std::endl;
        // std::cout << "  Zamrozonych zmiennych: " << current_config << std::endl;
        // std::cout << "  Iteracje: " << max_iterations << std::endl;
        // std::cout << "  F (mutacja): " << F << std::endl;
        // std::cout << "  CR (krzyzowanie): " << CR << std::endl;
;
        std::cout << "\nKONFIGURACJA " << (current_config + 1) << std::endl;


        for (int i = 0; i < num_trees; ++i) {
            frozen_genes[i] = (i < window_start);  
        }

        iteration_scores[current_config].clear();
        iteration_scores[current_config].reserve(max_iterations);

        iteration_worst_scores[current_config].clear();
        iteration_worst_scores[current_config].reserve(max_iterations);

        iteration_avg_scores[current_config].clear();
        iteration_avg_scores[current_config].reserve(max_iterations);
        

        // inicjalizacja populacji
        initializePopulationWithWindow(current_config, window_start, 
                                       window_size, greedy_angles);
        
        //reset
        best_fitness = 1e9;

        // poczatkowa ocena 
        for (int i = 0; i< population_size; ++i){


            fitness[i] = evaluateFitnessForConfig(population[i], current_config);
            // if (fitness[i] > 1000){
            //     std::cout << "Osobnik kolidacja " << i << " population: ";
            //     for (double v : population[i]) {
            //         std::cout << std::fixed << std::setprecision(3) << v << " ";
            //     }
            //     std::cout << std::endl;
            // }
            // else{
            //     std::cout << "Osobnik  " << i << " population: ";
            //     for (double v : population[i]) {
            //         std::cout << std::fixed << std::setprecision(3) << v << " ";
            //     }
            //     std::cout << std::endl;
            // }
            // if (i == 0){
            //     std::cout << "Fitness dla osobnika z greedy: " << i << " = " << fitness[i] << std::endl;
            // }
             //std::cout << "Fitness dla osobnika: " << i << " = " << fitness[i] << std::endl;

            if (fitness[i] < best_fitness){
                best_fitness = fitness[i];
                best_individual = population[i];
            }
        }

        // losowanie jednego osobnika do przepakowania
        int repack_individual = std::uniform_int_distribution<int>(0, population_size - 1)(rng);


        double fitness_before_repack = fitness[repack_individual]; 

        repackIndividual(population[repack_individual], current_config, window_start);
        
        double fitness_after_repack = evaluateFitnessForConfig(population[repack_individual], current_config);
        
   /*      std::cout << "Fitness przed repack " << std::fixed << fitness_before_repack << std::endl;
         std::cout << "Fitness po repack " << std::fixed << fitness_after_repack << std::endl;
        */
        if (fitness_after_repack < fitness_before_repack) {
            fitness[repack_individual] = fitness_after_repack;
            
            if (fitness_after_repack < best_fitness) {
                best_fitness = fitness_after_repack;
                best_individual = population[repack_individual];
            }
        } else {

            restoreOldPositions(current_config); // przywrócenie starych współrzednych jak nie ma poprawy
        }

        // std::cout << "Najlepszy fitness na poczatku " << std::fixed << std::setprecision(2) << best_fitness << std::endl;

        // glowna pętla-----------------------

        iterations_without_improvement = 0;  // przerwanie jak nie ma poprawy
        last_best_fitness = best_fitness;
        // int patience = max_iterations / 2;  // połowa max_iterations
        int patience =150;

        for (int iter = 0; iter < max_iterations; ++iter){

            iterationWithWindow(current_config, window_start, window_size);

            iteration_scores[current_config].push_back(best_fitness);

            double worst_fitness = *std::max_element(fitness.begin(), fitness.end());
            iteration_worst_scores[current_config].push_back(worst_fitness);

            double sum_fitness = 0.0;
            for (const auto& f : fitness) {
                sum_fitness += f;
            }
            double avg_fitness = sum_fitness / static_cast<double>(fitness.size());
            iteration_avg_scores[current_config].push_back(avg_fitness);

            
            // std::cout << "\n------------- sprawdzenie po iteracji  konfiguracja nr: " << (current_config + 1) << " ---" << std::endl;
            // std::cout << "Najlepszy fitness " << std::fixed << best_fitness << std::endl;


            // for (int i = 0; i <= current_config; ++i) {
            //     double angle_deg = best_individual[i] * 360.0;
            //     std::cout << " kat[" << i << "] = "<< angle_deg << std::endl;
            // }

            // for (int i = 0; i <= current_config; ++i) {
            //     double x = frozen_positions[current_config][i].first;
            //     double y = frozen_positions[current_config][i].second;
            //     std::cout << "choinka[" << i << "]: x = " << x << ", y = " << y << std::endl;
            // }

            //if (best_fitness < last_best_fitness - improvement_threshold) {
            //    // Jest poprawa - resetuj licznik
            //    iterations_without_improvement = 0;
            //    last_best_fitness = best_fitness;
            //} else {
            //    // Brak poprawy
            //    iterations_without_improvement++;
            //}
            //
            //if (iterations_without_improvement >= patience) {
            //    std::cout << "Zatrzymanie: brak poprawy przez " << patience 
            //            << " iteracji (iter " << (iter + 1) << "/" << max_iterations << ")" << std::endl;
            //    break;
            //}

            // przepakowanie co 10 iteracji
            if ((iter + 1) % 10 == 0) {
                // przepakowanie 3 osobnikow z najlepszym fit, najgorszym  i losowy
                std::vector<int> index_to_repack;

                auto best_fit = std::min_element(fitness.begin(), fitness.end());
                int best_osob = std::distance(fitness.begin(), best_fit);
                index_to_repack.push_back(best_osob);

                auto worst_fit = std::max_element(fitness.begin(), fitness.end());
                int worst_osob = std::distance(fitness.begin(), worst_fit);
                if (worst_osob != best_osob) {
                    index_to_repack.push_back(worst_osob);
                }   

                int random_osob;

                do {
                    random_osob = std::uniform_int_distribution<int>(0, population_size - 1)(rng);
                } while (random_osob == best_osob || random_osob == worst_osob);


                index_to_repack.push_back(random_osob);

                for (int idx : index_to_repack) {
                    double old_fitness = fitness[idx];
                    repackIndividual(population[idx], current_config, window_start);
                    double new_fitness = evaluateFitnessForConfig(population[idx], current_config);
                    
                    if (new_fitness < old_fitness) {
                        fitness[idx] = new_fitness;
                        if (new_fitness < best_fitness) {
                            best_fitness = new_fitness;
                            best_individual = population[idx];
                        }
                    } else {
                        restoreOldPositions(current_config);
                    }
                }

            }
        }
        // przepakowanie finalnego najlepszego osobnika
        std::vector<std::pair<double, double>> final_positions;
        
        GreedyPacker packer;
        packer.packWithFixedAnglesWindow(best_individual,current_config,window_start,frozen_positions,final_positions);
        
        for (int i = 0; i < final_positions.size(); ++i) {
            frozen_positions[current_config][i] = final_positions[i];
        }
        
        // do nastepmnej konfiguracji
        if (current_config + 1 < num_configs) {
            
            if (frozen_positions[current_config + 1].size() < num_trees_in_config + 1) {
                frozen_positions[current_config + 1].resize(num_trees_in_config + 1);
            }
            
            
            for (int i = 0; i < num_trees_in_config; ++i) {
                frozen_positions[current_config + 1][i] = frozen_positions[current_config][i];
            }
        }
        
        for (int i = window_start; i <= current_config; ++i) {
            optimized_angles[i] = best_individual[i];
        }
        best_fitness_per_config[current_config] = best_fitness;
        
        best_angles_per_config[current_config].clear();
        for (int i = 0; i <= current_config; ++i) {
            best_angles_per_config[current_config].push_back(best_individual[i]);
        }
        best_positions_per_config[current_config].clear();
        for (int i = 0; i <= current_config; ++i) {
            best_positions_per_config[current_config].push_back(
                frozen_positions[current_config][i]
            );
        }
        

        std::cout << "Najlepszy fitness " << best_fitness << std::endl;
        // std::cout << "Katy najlepszego osobnika:" << std::endl;
        // for (int i = 0; i <= current_config; ++i) {   // tylko aktywne kąty
        //     double angle_deg = best_individual[i] * 360.0; // znormalizowane -> stopnie
        //     std::cout << "  kat[" << i << "] = " 
        //             << std::fixed << std::setprecision(3) << angle_deg  << std::endl;
        // }
        // std::cout << "Wspolrzedne najlepszego osobnika:" << std::endl;
        // for (int i = 0; i <= current_config; ++i) {
        //     double x = frozen_positions[current_config][i].first;
        //     double y = frozen_positions[current_config][i].second;
        //     std::cout << "  choinka[" << i << "]: x = " 
        //             << std::fixed << std::setprecision(3) << x
        //             << ", y = " << y << std::endl;
        // }

        // Pokaz wszystkie zamrozone katy
        // std::cout << "Zamrozone katy do tej pory:" << std::endl;
        // for (int i = 0; i <= current_config; ++i) {
        //     std::cout << "  kat[" << i << "]: " 
        //               << std::fixed << std::setprecision(6)
        //               << (optimized_angles[i] * 360.0) <<  std::endl;
        // }

        // std::cout << "Zoptymalizowane katy w oknie:" << std::endl;
        // for (int i = window_start; i <= current_config; ++i) {
        //     std::cout << "  kat[" << i << "]: " 
        //               << std::fixed << std::setprecision(3)
        //               << (optimized_angles[i] * 360.0) ;
        //     if (i == current_config) {
        //         std::cout << " (NOWY)";
        //     }
        //     std::cout << std::endl;
        // }
        // std::cout << "\n-------------------------------------------" << std::endl;
        // std::cout << "\n-------------------------------------------" << std::endl;
    }
    std::cout << "Finalne przepakowanie" << std::endl;
    GreedyPacker packer;
    
    for (int config = 0; config < num_configs; ++config) {
        int num_trees_in_config = config + 1;
        
        std::vector<std::pair<double, double>> final_positions;
        
        std::vector<float> config_angles(num_trees, 0.0);
        for (int i = 0; i <= config; ++i) {
            config_angles[i] = optimized_angles[i];
        }
        
        packer.packWithFixedAnglesWindow(
            config_angles,
            config,
            0,  // window_start = 0 - przepakuj wszystko od początku
            frozen_positions,
            final_positions
        );
        
        for (int i = 0; i < final_positions.size(); ++i) {
            frozen_positions[config][i] = final_positions[i];
        }
        
        // std::cout << "Config " << (config + 1) << " przepakowane" << std::endl;
    }
    

    std::cout << "\nNajlepsze score " << std::endl;
    
    config_scores.clear();
    config_scores.resize(num_configs, 0.0);
    
    for (int config = 0; config < num_configs; ++config) {
        int num_trees_in_config = config + 1;
        
        trees_cache.clear();
        for (int tree_id = 0; tree_id < num_trees_in_config; ++tree_id) {
            double x = frozen_positions[config][tree_id].first;
            double y = frozen_positions[config][tree_id].second;
            double angle_deg = optimized_angles[tree_id] * 360.0;
            
            trees_cache.emplace_back(x, y, angle_deg);
        }
        
        bool collision = false;
        for (int i = 0; i < trees_cache.size() && !collision; ++i) {
            for (int j = i + 1; j < trees_cache.size(); ++j) {
                if (trees_cache[i].intersects(trees_cache[j])) {
                    if (!trees_cache[i].getPolygon()->touches(trees_cache[j].getPolygon())) {
                        collision = true;
                        break;
                    }
                }
            }
        }
        
        if (collision) {
            config_scores[config] = 1e6;
        } else {
            double min_x = std::numeric_limits<double>::max();
            double max_x = std::numeric_limits<double>::lowest();
            double min_y = std::numeric_limits<double>::max();
            double max_y = std::numeric_limits<double>::lowest();
            
            for (const auto& tree : trees_cache) {
                const geos::geom::Envelope* env = tree.getEnvelope();
                min_x = std::min(min_x, env->getMinX());
                max_x = std::max(max_x, env->getMaxX());
                min_y = std::min(min_y, env->getMinY());
                max_y = std::max(max_y, env->getMaxY());
            }
            
            double side_length_scaled = std::max(max_x - min_x, max_y - min_y);
            double scale_factor = trees_cache[0].getScaleFactor();
            
            config_scores[config] = ((side_length_scaled * side_length_scaled) / 
                                     (scale_factor * scale_factor)) / 
                                     static_cast<double>(num_trees_in_config);
            
            std::cout << "Config " << (config + 1) << " score = " << std::fixed << std::setprecision(4) << config_scores[config] << std::endl;
        }
    }


    return best_individual;

}


void DE::iterationWithWindow(int current_config, int window_start, int window_size) {
    
    for (int i = 0; i < population_size; ++i) {

        // Mutacja
        std::vector<float> mutant = mutateWindow(i, window_start, current_config);
        
        // Crossover
        std::vector<float> child = crossoverWindow(population[i], mutant, window_start, current_config);
        // Ocena
        double child_fitness = evaluateFitnessForConfig(child, current_config);
        
        // Selekcja
        if (child_fitness < fitness[i]) {
            population[i] = child;
            fitness[i] = child_fitness;
            
            if (child_fitness < best_fitness) {
                best_fitness = child_fitness;
                best_individual = child;
            }
        }
    }

}


std::vector<float>& DE::mutateWindow(int parent_id, int window_start, int window_end) {
    
    // 3 rożróżne osobniki
    int r1, r2, r3;

    
    do {
        r1 = std::uniform_int_distribution<int>(0, population_size - 1)(rng);
    } while (r1 == parent_id);
    
    do {
        r2 = std::uniform_int_distribution<int>(0, population_size - 1)(rng);
    } while (r2 == parent_id || r2 == r1);
    
    do {
        r3 = std::uniform_int_distribution<int>(0, population_size - 1)(rng);
    } while (r3 == parent_id || r3 == r1 || r3 == r2);
    
    //std::vector<float> mutant = population[parent_id];
    mutant_buffer = population[parent_id];
    
    
    for (int gene = window_start; gene <= window_end; ++gene) {
        //random
        // double v = population[r1][gene] + F * (population[r2][gene] - population[r3][gene]);

        // DE/current/1: mutant = current + F * (r1 - r2)
        // double v = population[parent_id][gene] + F * (population[r1][gene] - population[r2][gene]);

        // DE/best/1: mutant = best + F * (r1 - r2)
        double v = best_individual[gene] + F * (population[r1][gene] - population[r2][gene]);
        
        if (v < 0.0) v += 1.0; //normalizacja
        if (v >= 1.0) v -= 1.0;
        
        mutant_buffer[gene] = std::round(v * 1000) / 1000.0;
        if (mutant_buffer[gene] >= 1.0) mutant_buffer[gene] = 0.999;
    }
    
    return mutant_buffer;
}
std::vector<float>& DE::crossoverWindow(const std::vector<float>& parent, const std::vector<float>& mutant, int window_start, int window_end) {
    
    //std::vector<float> child = parent;
    child_buffer = parent;
    
    // jeden gen na pewno z mutanta
    int gene_v = std::uniform_int_distribution<int>(window_start, window_end)(rng);
    
    // Crossover dla każdego genu w oknie
    for (int gene = window_start; gene <= window_end; ++gene) {
        double prob = uniform_dist(rng);

        if (prob < CR || gene == gene_v) {
            child_buffer[gene] = mutant[gene];
        }
    }
    
    return child_buffer;
}

void DE::repackIndividual(std::vector<float>& individual_angles,int current_config,int window_start) {
    
    int num_trees = current_config + 1;


    std::vector<std::pair<double, double>> old_positions = frozen_positions[current_config];
    
    std::vector<std::pair<double, double>> new_positions;
    
    GreedyPacker packer;
    double square_side = packer.packWithFixedAnglesWindow( individual_angles, current_config,
                        window_start, frozen_positions, new_positions);
    
    // zapisanie tymczasowo nowych pozycji
    for (size_t i = 0; i < new_positions.size(); ++i) {
        frozen_positions[current_config][i] = new_positions[i];
    }

    temporary_backup_positions[current_config] = old_positions;
}


void DE::restoreOldPositions(int current_config) {
    if (temporary_backup_positions.find(current_config) != temporary_backup_positions.end()) {
        
        frozen_positions[current_config] = temporary_backup_positions[current_config];
        
        temporary_backup_positions.erase(current_config);
    }
}

