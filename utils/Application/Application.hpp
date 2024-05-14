#pragma once

#include "Interface.hpp"

class Screen : public sf::Drawable, public sf::Transformable
{
private:
    sf::Texture tex;
    sf::Sprite obj;
    sf::Vector2i kol;
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    Screen();

    void update();
};


class App
{
private:
    sf::ContextSettings settings;
    sf::RenderWindow window;
    sf::Clock clock;
    sf::Vector3f cam_pos;
    FPS counter;
    Character camera;
    std::vector<GeomObject*> objects;
    Screen screen;
    Map map;
    
public:
    App();
    void run();
    ~App();
};