#include "GreedyPacker.h"
#include <cmath>
#include <iostream>
#include <iomanip>

std::pair<std::vector<std::shared_ptr<ChristmasTree>>, std::vector<double>>
GreedyPacker::packTrees(int num_trees) {

    // Vector umieszczonych choinek
    std::vector<std::shared_ptr<ChristmasTree>> trees;
    std::vector<double> sides_square; // boki kwadratów dla kazdej choinki
    
    if (num_trees == 0) return {trees, sides_square};
    // Pierwsza choinka w (0,0)
    std::cout << "\n Umieszczanie choinki 0 w (0, 0) z katem 0" << std::endl;
    auto tree0 = std::make_shared<ChristmasTree>(0.0, 0.0, 0.0);
    trees.push_back(tree0);
    double current_side0 = calculateGlobalSquareSide(trees, tree0);
    sides_square.push_back(current_side0);
    const auto* env = tree0->getEnvelope();
    std::cout << " Pierwsza choinka umieszczona. Ramka ograniczajaca: ["
              << std::fixed << std::setprecision(2)
              << env->getMinX() << ", " << env->getMinY() << "] - ["
              << env->getMaxX() << ", " << env->getMaxY() << "]"
              << std::endl;

    
    if (num_trees == 1) return {trees, sides_square};




    // Główna pętla -- kolejne choinki
    for (int i = 1; i < num_trees; i++) {
        // std::cout << "\n Szukanie pozycji dla choinki " << i << std::endl;
        
        // Tworzymy choinkę tymczasowo w (0, 0) z kątem 0
        auto new_tree = std::make_shared<ChristmasTree>(0.0, 0.0, 0.0);
        
        // Szukamy najlepszej pozycji (bez zmiany kąta)
        // std::cout << " Choinka" << i << std::endl;
        auto [best_x, best_y] = findBestPosition(new_tree, trees);
        
        // Ustawiamy znalezioną pozycję
        new_tree->setPosition(best_x, best_y);

        // Szukamy najlepszego kąta dla tej pozycji
        double best_angle = findBestAngle(new_tree, trees);
        new_tree->setAngle(best_angle);
        
        trees.push_back(new_tree);
        double current_side = calculateGlobalSquareSide(trees, new_tree);
        sides_square.push_back(current_side);

        
        // std::cout << " Umieszczona w (" 
                //   << std::fixed << std::setprecision(2) 
                //   << best_x << ", " << best_y << ")" << std::endl;
    }
    
    return {trees, sides_square};
}

std::pair<double, double>
GreedyPacker::findBestPosition(
    const std::shared_ptr<ChristmasTree>& new_tree,
    const std::vector<std::shared_ptr<ChristmasTree>>& placed) {
    
    // paraemtry przeszukiwania
    const int NUM_DIRECTIONS = 90;      // 90 kierunków (co 4 stopni)
    const double START_DISTANCE = 30.0; // Start 5 jednostek daleko
    const double STEP = 0.1;           // Zbliżaj co 0.1 jednostki
    const double MIN_DIST = 0.01;      // Granica zbliżania
    
    double best_distance = 1e9;  // "Nieskończoność"

    double best_square = 1e9;   // Nowy główny priorytet
    double best_center = 1e9;   // Drugorzędny priorytet
    double best_x = 0.0, best_y = 0.0;
    

    // petla po każdym kierunku
    for (int dir = 0; dir < NUM_DIRECTIONS; dir++) {
        // Kierunek jako kąt -- 0, 45, 90, itd
        // jak dir=0 -> to kąt 0 wiec wektor jednstkowy (1,0) -> przeuswamy w prawo
        // jak dir=1 -> to kąt 45 wiec wektor jednstkowy (sqrt(2)/2, sqrt(2)/2) -> przesuwamy w prawo i w góre
        // jak dir=2 -> to kąt 90 wiec wektor jednstkowy (0,1) -> przesuwamy w góre
        // itd.

        double angle_rad = (2.0 * M_PI * dir) / NUM_DIRECTIONS;
        double dx = std::cos(angle_rad);
        double dy = std::sin(angle_rad);

        
        // std::cout << "    Kierunek " << (dir * 45) <<  "stopni: ";
        


        // pętla zbliżania do środka
        for (double dist = START_DISTANCE; dist > MIN_DIST; dist -= STEP) {
            double x = dx * dist;
            double y = dy * dist;
            
            // usawianie choinki w tej pozycji
            new_tree->setPosition(x, y);
            
            
            // sprawdzanie kolizji z już umieszzczonymi choinkami
            bool collision = false;
            for (const auto& placed_tree : placed) {
                if (new_tree->intersects(*placed_tree)) {
                    collision = true;
                    break;  //  kolizja przerwac
                }
            }
            

        //     if (!collision && dist < best_distance) {
        //         best_distance = dist;
        //         best_x = x;
        //         best_y = y;
        //     }
        // }

        // if (!collision) {
        //         double metric = std::max(std::abs(x), std::abs(y)); // Metryka Chebyshev - tworzy kwadrat
        //         if (metric < best_distance) {
        //             best_distance = metric;
        //         best_x = x;
        //         best_y = y;
        //     }
        // }
        if (!collision) {
            // Obliczamy bok kwadratu dla całego układu
            double current_square = calculateGlobalSquareSide(placed, new_tree);
            // Obliczamy odległość pnia (środka) od (0,0)
            double current_center = std::hypot(x, y);

            // Warunek wyboru: mniejszy kwadrat MA PRIORYTET
            if (current_square < best_square - 0.001) {
                best_square = current_square;
                best_center = current_center;
                best_x = x;
                best_y = y;
            } 
            // Jeśli kwadrat jest taki sam, wybieramy pozycję bliżej środka
            else if (std::abs(current_square - best_square) < 0.001) {
                if (current_center < best_center) {
                    best_center = current_center;
                    best_x = x;
                    best_y = y;
                }
            }
        }

        }
        
        // std::cout << "best_dist=" << std::fixed << std::setprecision(2) 
                //   << best_distance << std::endl;
    }
   
    
    return {best_x, best_y};

}

 double GreedyPacker::findBestAngle(const std::shared_ptr<ChristmasTree>& new_tree,
    const std::vector<std::shared_ptr<ChristmasTree>>& placed) {
        int step = 1; // krok zmiany kata
        double best_angle = 0.0;
        double current_x = new_tree->getX();
        double current_y = new_tree->getY();
        // double best_distance = std::hypot(current_x, current_y);
        double best_square = 1e9; // Zamiast best_distance

        for (int rotation = 0; rotation < 360; rotation += step) {
            new_tree -> setAngle(rotation);
            new_tree->setPosition(current_x, current_y);

            bool collision = false;
            for (const auto& placed_tree : placed) {
                if (new_tree->intersects(*placed_tree)) {
                    collision = true;
                    break;
                }
            }
            // if (!collision) {
            //     double improved_distance = tryMoveCloser(new_tree, placed);
            
            //     if (improved_distance < best_distance) {
            //         best_distance = improved_distance;
            //         best_angle = rotation;
            //         // std::cout << "     Znaleziono lepszy kat: " << rotation 
            //                 // << "° (odleglosc: " << improved_distance << ")" << std::endl;
            //     }
            // }
            if (!collision) {
                // Chwilowo dopychamy, żeby sprawdzić ostateczny wynik dla tego kąta
                tryMoveCloser(new_tree, placed); 
                
                // Sprawdzamy rozmiar kwadratu dla tego kąta
                double current_square = calculateGlobalSquareSide(placed, new_tree);

                if (current_square < best_square) {
                    best_square = current_square;
                    best_angle = rotation;
                }
            }

        }
        new_tree->setAngle(best_angle);
        new_tree->setPosition(current_x, current_y);
        tryMoveCloser(new_tree, placed); // zeby jeszcze sprobowac zblizyc
        
        return best_angle;

    }

double GreedyPacker::tryMoveCloser(
    const std::shared_ptr<ChristmasTree>& tree,
    const std::vector<std::shared_ptr<ChristmasTree>>& placed) {
    
    double current_x = tree->getX();
    double current_y = tree->getY();
    double current_distance = std::hypot(current_x, current_y);
    
    
    if (current_distance < 0.01) { 
        return current_distance; // nie da sie blizej
    }
    
    // Kierunek do centrum (0, 0)
    double to_center_x = -current_x / current_distance;
    double to_center_y = -current_y / current_distance;
    
    const double PUSH_STEP = 0.1; 
    double best_distance = current_distance;
    double best_x = current_x;
    double best_y = current_y;
    
    for (double d = PUSH_STEP; d < current_distance; d += PUSH_STEP) {
        double new_x = current_x + to_center_x * d;
        double new_y = current_y + to_center_y * d;
        
        tree->setPosition(new_x, new_y);
        
        bool collision = false;
        for (const auto& placed_tree : placed) {
            if (tree->intersects(*placed_tree)) {
                collision = true;
                break;
            }
        }
        
        if (collision) {
            tree->setPosition(best_x, best_y);
            return best_distance;
        } else {
            best_x = new_x;
            best_y = new_y;
            // best_distance = std::hypot(new_x, new_y);
            best_distance = calculateGlobalSquareSide(placed, tree);
        }
    }

    
    return best_distance;
}

double GreedyPacker::calculateGlobalSquareSide(
    const std::vector<std::shared_ptr<ChristmasTree>>& placed,
    const std::shared_ptr<ChristmasTree>& new_tree) {

    double minX = 1e9; 
    double maxX = -1e9;
    double minY = 1e9; 
    double maxY = -1e9;
    
    auto all_trees = placed;
    all_trees.push_back(new_tree);
    // pobieranie envelope  choinki -- min i max x, y  i sprawdzenie najdalszego punktu od (0,0)
    for (const auto& t : all_trees) {
        const auto* env = t->getEnvelope();
        if (env->getMinX() < minX) minX = env->getMinX();
        if (env->getMaxX() > maxX) maxX = env->getMaxX();
        if (env->getMinY() < minY) minY = env->getMinY();
        if (env->getMaxY() > maxY) maxY = env->getMaxY();
    }


    double sf = new_tree->getScaleFactor(); 
    double width = maxX - minX;
    double height = maxY - minY;

    // bierzemy najdalszy punkt i zwracamy bok kwadratu jako podwojona odleglosc
    return std::max(width, height) / sf; //bok kwadratu
}


