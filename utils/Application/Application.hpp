#pragma once

#include "Interface.hpp"

class Screen : public sf::Drawable, public sf::Transformable
{
private:
    std::vector<sf::Uint8> vission;
    sf::Texture tex;
    sf::Sprite obj;
    sf::Vector2i kol;
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    Screen();

    void update(std::vector<Settings::vis_point> dist);
};


class App
{
private:
    sf::ContextSettings settings;
    sf::RenderWindow window;
    sf::Clock clock;
    sf::Vector3f cam_pos;
    std::vector<Settings::vis_point> dist;
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