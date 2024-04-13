#pragma once

#include "Interface.hpp"

class Screen : public sf::Drawable, public sf::Transformable
{
private:
    sf::RectangleShape floor;
    sf::RectangleShape sky;
    std::vector<sf::RectangleShape> vission;
    int kol;
    float dal=600;
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    Screen(int kol_);

    void update(std::vector<float> dist);
};


class App
{
private:
    sf::Vector2f cam_pos;
    sf::Vector2f center;
    int discretization = 180;
    std::vector<float> dist;
    int fps = 0;
    float t = 0;
    
public:
    App();
    ~App();
};