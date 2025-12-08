#include <iostream>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>

#include <geos/io/WKTReader.h>
#include <geos/io/WKTWriter.h>


using namespace geos::geom;
using namespace geos::io;

int main() {
    std::cout << "TEST 1: Podstawowy C++ DZIAŁA!" << std::endl;
    std::cout << "TEST 2: Kompilator OK!" << std::endl;
    std::cout << "dziala" << std::endl;
    /* New factory with default (float) precision model */
    GeometryFactory::Ptr factory = GeometryFactory::create();
    return 0;
}
