#include "ChristmasTree.h"
#include <iostream>
#include <iomanip>
#include "GreedyPacker.h"
#include "CSVWriter.h"


int main(){

    std::cout << "TESTY ChristmasTree ==================" << std::endl;

    // choinka w (0,0) bez obrotu
    ChristmasTree tree11(0.0, 0.0, 0.0);
    std::cout << "tree1 (0,0,0°):" << std::endl;
    std::cout << "   Pole: " << tree11.getArea() << std::endl;

    // 
    const auto* env1 = tree11.getEnvelope();
    std::cout << "   Ramka: [" 
              << env1->getMinX() << "," << env1->getMaxX() 
              << "," << env1->getMinY() << "," << env1->getMaxY() << "]" << std::endl;
    
    std::cout << std::endl;

    std::cout << "TEST PRZECIĘĆ ===" << std::endl;

    ChristmasTree tree1(0.0, 0.0, 0.0);     // Środek
    ChristmasTree tree2(0.1, 0.0, 0.0);     // Blisko – powinny się przecinać
    ChristmasTree tree3(3.0, 0.0, 0.0);     // Daleko – nie przecinają się

    std::cout << "tree1 intersects tree2: " << (tree1.intersects(tree2) ? "TAK" : "NIE") << std::endl;
    std::cout << "tree1 intersects tree3: " << (tree1.intersects(tree3) ? "TAK" : "NIE") << std::endl;
    
    std::cout <<  "Greedy test - bez zmiany kata -- kat 0" << std::endl;

    int num_trees = 200;
    std::cout << "\nPakowanie do pudła " << num_trees << " choinek..." << std::endl;
    GreedyPacker packer;
    auto trees = packer.packTrees(num_trees);
    std::cout << "\nZapakowane" << std::endl;

    std::cout << "Zapis do pliku " << std::endl;

    CSVWriter::saveTreesToCSV(trees, "results.csv");

    std::cout << "Koniec" << std::endl;

    






    return 0;



}