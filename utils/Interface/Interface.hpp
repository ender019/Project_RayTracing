#pragma once

#include "Character.hpp"

class Map : public sf::Drawable, public sf::Transformable
{
private:
    sf::RectangleShape back;
    sf::CircleShape camera;
    sf::VertexArray conture;
    int kol;
    float part;
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    Map(Character player_);

    void update(Character player);
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