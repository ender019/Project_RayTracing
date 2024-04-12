#pragma once

#include "Character.hpp"

const int W = 1200;
const int H = 800;

class Map : public sf::Drawable, public sf::Transformable
{
private:
    sf::RectangleShape back;
    std::vector<GeomObject*> objects;
    sf::CircleShape camera;
    sf::VertexArray conture;
    int kol;
    float part;
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    Map(std::vector<GeomObject*> objects_, Character player_, float p=0.16667);

    void update(Character player);

    ~Map();
};