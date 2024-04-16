#pragma once

#include "Character.hpp"

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
    Map(Character player_, float p=0.16667);

    void reload(std::vector<GeomObject*> objects_);
    void update(Character player);

    ~Map();
};

class FPS : public sf::Drawable, public sf::Transformable
{
private:
    sf::Text fps_lable;
    sf::Font font;
    int fps = 0;
    float t = 0;
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    FPS();

    void update(float dt);
};