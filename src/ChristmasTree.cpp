#include "ChristmasTree.h"
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/Point.h>
#include <cmath>

using namespace std;
// Stała: PI do konwersji stopni na radiany
static constexpr double PI = 3.14159265358979323846;

ChristmasTree::ChristmasTree(double center_x, double center_y, double angle)
    : center_x(center_x), center_y(center_y), angle(angle) {
    
    // Utwórz fabrykę geometrii
    factory = GeometryFactory::create();

    //OPTYMALIZACJA
    rebuildRotatedPolygon();

    

    // tworzenie choinki w 0,0
    std::unique_ptr<Geometry> initial = buildInitialPolygon();
    // obracanie choinki o kąt
    std::unique_ptr<Geometry> rotated = rotatePolygon(initial.get(), this->angle);
    polygon = translatePolygon(rotated.get(), 
                               this->center_x * scale_factor, 
                               this->center_y * scale_factor);
}

//OPTYMALIZACJA
void ChristmasTree::rebuildRotatedPolygon() {
    std::unique_ptr<Geometry> initial = buildInitialPolygon();
    base_rotated_polygon = rotatePolygon(initial.get(), this->angle);
    cached_angle = this->angle;
}


// zwraca wektor współrzędnych początkowych choinki, które potem są używane do budowy choinki
std::vector<Coordinate> ChristmasTree::getInitialCoordinates() {
    std::vector<Coordinate> coords;
    double sf = scale_factor;
    double trunk_bottom_y = -trunk_h;
    
    // Wierzchołek (tip)
    coords.emplace_back(0.0 * sf, tip_y * sf);
    
    // Prawe ramię górnego piętra (TOP TIER)
    coords.emplace_back((top_w / 2.0) * sf, tier_1_y * sf);
    coords.emplace_back((top_w / 4.0) * sf, tier_1_y * sf);
    
    // Prawe ramię środkowego piętra (MID TIER)
    coords.emplace_back((mid_w / 2.0) * sf, tier_2_y * sf);
    coords.emplace_back((mid_w / 4.0) * sf, tier_2_y * sf);
    
    // Prawe ramię dolnego piętra (BASE TIER)
    coords.emplace_back((base_w / 2.0) * sf, base_y * sf);
    
    // Prawy trzon
    coords.emplace_back((trunk_w / 2.0) * sf, base_y * sf);
    coords.emplace_back((trunk_w / 2.0) * sf, trunk_bottom_y * sf);
    
    // Lewy trzon
    coords.emplace_back(-(trunk_w / 2.0) * sf, trunk_bottom_y * sf);
    coords.emplace_back(-(trunk_w / 2.0) * sf, base_y * sf);
    
    // Lewe ramię dolnego piętra (BASE TIER)
    coords.emplace_back(-(base_w / 2.0) * sf, base_y * sf);
    
    // Lewe ramię środkowego piętra (MID TIER)
    coords.emplace_back(-(mid_w / 4.0) * sf, tier_2_y * sf);
    coords.emplace_back(-(mid_w / 2.0) * sf, tier_2_y * sf);
    
    // Lewe ramię górnego piętra (TOP TIER)
    coords.emplace_back(-(top_w / 4.0) * sf, tier_1_y * sf);
    coords.emplace_back(-(top_w / 2.0) * sf, tier_1_y * sf);
    
    return coords;
}

// budowanie wielokąta -- choinki
std::unique_ptr<Geometry> ChristmasTree::buildInitialPolygon() {
    std::vector<Coordinate> coords = getInitialCoordinates(); // 15 punktów
    
    // tworzymy sekwencje +1 punkt, aby zamknąć pętlę
    std::unique_ptr<CoordinateSequence> coord_seq(
        new CoordinateSequence(coords.size() + 1, 2)
    );
    
    // wstawiaanie współrzędnych do sekwencji
    for (std::size_t i = 0; i < coords.size(); ++i) {
        coord_seq->setAt(coords[i], i);
    }
    
    // zamkniecie pętli
    coord_seq->setAt(coords[0], coords.size());
    
    // tworzenie krawedzi
    std::unique_ptr<LinearRing> ring(factory->createLinearRing(std::move(coord_seq)));
    std::unique_ptr<Geometry> poly(factory->createPolygon(std::move(ring)));
    
    return poly;
}

// obracanie wielokąta
std::unique_ptr<Geometry> ChristmasTree::rotatePolygon(Geometry* geom, double angle_deg) {
    double angle_rad = angle_deg * PI / 180.0;
    double cos_a = std::cos(angle_rad);
    double sin_a = std::sin(angle_rad);
    
    std::unique_ptr<CoordinateSequence> old_seq = geom->getCoordinates();
    std::size_t n = old_seq->size();

    std::unique_ptr<CoordinateSequence> new_seq(
        new CoordinateSequence(n, 2)
    );
    
    // transformacja każdego punktu choinki (macierz rotacji)
    for (std::size_t i = 0; i < n; ++i) {
        const Coordinate& c = old_seq->getAt(i);
        double nx = c.x * cos_a - c.y * sin_a;
        double ny = c.x * sin_a + c.y * cos_a;
        new_seq->setAt(Coordinate(nx, ny), i);
    }
    
    std::unique_ptr<LinearRing> ring(factory->createLinearRing(std::move(new_seq)));
    std::unique_ptr<Geometry> poly(factory->createPolygon(std::move(ring)));
    
    return poly;
}

// przesuwanie wielokąta np o dx lub dy
std::unique_ptr<Geometry> ChristmasTree::translatePolygon(Geometry* geom, double dx, double dy) {
    std::unique_ptr<CoordinateSequence> old_seq = geom->getCoordinates();
    std::size_t n = old_seq->size();
    
    std::unique_ptr<CoordinateSequence> new_seq(
        new CoordinateSequence(n, 2)
    );
    
    // Przesuń każdy punkt
    for (std::size_t i = 0; i < n; ++i) {
        const Coordinate& c = old_seq->getAt(i);
        new_seq->setAt(Coordinate(c.x + dx, c.y + dy), i);
    }
    
    std::unique_ptr<LinearRing> ring(factory->createLinearRing(std::move(new_seq)));
    std::unique_ptr<Geometry> poly(factory->createPolygon(std::move(ring)));
    
    return poly;
}

// gettery
Geometry* ChristmasTree::getPolygon() const {
    return polygon.get();
}
// zwraca prostokątną ramkę ograniczająco choinki
const geos::geom::Envelope* ChristmasTree::getEnvelope() const {
    return polygon->getEnvelopeInternal();
}


// sprawdzanie czy choinka przecina sie z innymi
bool ChristmasTree::intersects(const ChristmasTree& other) const {
    return polygon->intersects(other.polygon.get());
}

// spawdzenie czy dany punkt leży wewnątrz choinki
bool ChristmasTree::contains(const Coordinate& point) const {
    std::unique_ptr<Geometry> point_geom(factory->createPoint(point));
    return polygon->contains(point_geom.get());
}

// zwraca pole powierzchni choinki
double ChristmasTree::getArea() const {
    return polygon->getArea();
}

//ustawi nowa pozycje choinki
//void ChristmasTree::setPosition(double new_center_x, double new_center_y) {
//    center_x = new_center_x;
//    center_y = new_center_y;
//    
//    std::unique_ptr<Geometry> initial = buildInitialPolygon(); // kosztowne
//    std::unique_ptr<Geometry> rotated = rotatePolygon(initial.get(), angle); // kosztowne 
//    polygon = translatePolygon(rotated.get(), center_x * scale_factor, center_y * scale_factor);
//}



//OPTYMALIZACJA
void ChristmasTree::setPosition(double new_center_x, double new_center_y) {
    center_x = new_center_x;
    center_y = new_center_y;

    polygon = translatePolygon(base_rotated_polygon.get(),
        center_x * scale_factor,
        center_y * scale_factor);
}



// ustawia nowy kąt obrotu choinki
//void ChristmasTree::setAngle(double new_angle) {
//    angle = new_angle;
//    
//    std::unique_ptr<Geometry> initial = buildInitialPolygon();
//    std::unique_ptr<Geometry> rotated = rotatePolygon(initial.get(), angle);
//    polygon = translatePolygon(rotated.get(), center_x * scale_factor, center_y * scale_factor);
//}


//OPTYMALIZACJA
void ChristmasTree::setAngle(double new_angle) {
    // Skip jeśli kąt się nie zmienił
    if (std::abs(new_angle - cached_angle) < 1e-9) {
        return;
    }

    angle = new_angle;


    rebuildRotatedPolygon();

    polygon = translatePolygon(base_rotated_polygon.get(),
        center_x * scale_factor,
        center_y * scale_factor);
}

// przesuwa choinkę o dx i dy
void ChristmasTree::move(double dx, double dy) {
    setPosition(center_x + dx, center_y + dy);
}
// obraca choinkę o nowy kąt
void ChristmasTree::rotate(double new_angle) {
    setAngle(new_angle);
}

double ChristmasTree::getScaleFactor() const { 
    return scale_factor; 
}
