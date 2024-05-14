#pragma once

#include "Interface.hpp"

class Screen : public sf::Drawable, public sf::Transformable
{
private:
    sf::RectangleShape floor;
    sf::RectangleShape sky;
    sf::Shader shader;
    std::vector<sf::RectangleShape> vission;
    int kol;
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    Screen(int kol_);

    void update(std::vector<Settings::vis_point> dist);
};


class App
{
private:
    sf::ContextSettings settings;
    sf::RenderWindow window;
    sf::Clock clock;
    sf::Vector2f cam_pos;
    std::vector<Settings::vis_point> dist;
    // FPS counter;
    Character camera;
    std::vector<GeomObject*> objects;
    Screen screen;
    Map map;
    
public:
    App();
    void run();
    ~App();
};