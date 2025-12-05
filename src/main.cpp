#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>

#include <geos/io/WKTReader.h>
#include <geos/io/WKTWriter.h>

#include <iostream>

using namespace geos::geom;
using namespace geos::io;

int main()
{
    /* New factory with default (float) precision model */
    GeometryFactory::Ptr factory = GeometryFactory::create();

    /*
    * Reader requires a factory to bind the geometry to
    * for shared resources like the PrecisionModel
    */
    WKTReader reader(*factory);

    /* Input WKT strings */
    std::string wkt_a("POLYGON((0 0, 10 0, 10 10, 0 10, 0 0))");
    std::string wkt_b("POLYGON((5 5, 15 5, 15 15, 5 15, 5 5))");

    /* Convert WKT to Geometry */
    std::unique_ptr<Geometry> geom_a(reader.read(wkt_a));
    std::unique_ptr<Geometry> geom_b(reader.read(wkt_b));

    /* Calculate intersection */
    std::unique_ptr<Geometry> inter = geom_a->intersection(geom_b.get());

    /* Convert Geometry to WKT */
    WKTWriter writer;
    writer.setTrim(true); /* Only needed before GEOS 3.12 */
    std::string inter_wkt = writer.write(inter.get());

    /* Print out results */
    std::cout << "Geometry A:         " << wkt_a << std::endl;
    std::cout << "Geometry B:         " << wkt_b << std::endl;
    std::cout << "Intersection(A, B): " << inter_wkt << std::endl;

}