
#ifndef CHRISTMAS_TREE_H
#define CHRISTMAS_TREE_H

#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/Coordinate.h>
#include <memory>
#include <vector>
#include <cmath>

using namespace geos::geom;

class ChristmasTree {
private:
    // Parametry pozycji i rotacji
    double center_x;
    double center_y;
    double angle;  // w stopniach
    
    // Fabryka geometrii (współdzielona)
    std::shared_ptr<GeometryFactory> factory;
    
    // Wielokąt reprezentujący chinkę
    std::unique_ptr<Geometry> polygon;
    
    // arametry wymiarów choinki
    static constexpr double scale_factor = 1000.0;
    static constexpr double trunk_w = 0.15;
    static constexpr double trunk_h = 0.2;
    static constexpr double base_w = 0.7;
    static constexpr double mid_w = 0.4;
    static constexpr double top_w = 0.25;
    
    static constexpr double tip_y = 0.8;
    static constexpr double tier_1_y = 0.5;
    static constexpr double tier_2_y = 0.25;
    static constexpr double base_y = 0.0;

    std::unique_ptr<Geometry> buildInitialPolygon();
    std::unique_ptr<Geometry> rotatePolygon(Geometry* geom, double angle_deg);
    std::unique_ptr<Geometry> translatePolygon(Geometry* geom, double dx, double dy);
    std::vector<Coordinate> getInitialCoordinates();
    
public:
    // Konstruktor
    ChristmasTree(double center_x = 0.0, double center_y = 0.0, double angle = 0.0);
    
    // Destruktor
    ~ChristmasTree() = default;
    
    // Gettery
    double getX() const { return center_x; }
    double getY() const { return center_y; }
    double getAngle() const { return angle; }
    Geometry* getPolygon() const;
    const geos::geom::Envelope* getEnvelope() const;
    
    // Operacje geometryczne
    bool intersects(const ChristmasTree& other) const;
    bool contains(const Coordinate& point) const;
    double getArea() const;
    
    // Settery / transformacje
    void setPosition(double new_center_x, double new_center_y);
    void setAngle(double new_angle);
    void move(double dx, double dy);
    void rotate(double new_angle);

    double getScaleFactor() const;

};

#endif 