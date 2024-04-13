#include "Application.hpp"

void Screen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(floor, states);
    target.draw(sky, states);
    for (int i = 0; i < kol; i++) {target.draw(vission[i]);}
}

Screen::Screen(int kol_): 
    floor(sf::Vector2f(W,H/2)), sky(sf::Vector2f(W,H/2)), 
    vission(kol_, sf::RectangleShape(sf::Vector2f(W/kol_, H/2))), kol(kol_)
{
    floor.setFillColor(sf::Color(200, 165, 0));
    sky.setFillColor(sf::Color(165, 165, 255));
    floor.move(sf::Vector2f(0,H/2));
    for (int i = 0; i < kol; i++)
    {
        vission[i].move(sf::Vector2f(i*(W/kol),H/2));
        vission[i].setFillColor(sf::Color::Red);
    }
}
    
void Screen::update(std::vector<float> dist)
{
    float h;
    for (int i = 0; i < kol; i++)
    {
        h=(dal/(dist[i]+dal))*H;
        if(dist[i]>=599) h=0;
        vission[i].setSize(sf::Vector2f((float)W/kol, h));
        vission[i].setPosition(sf::Vector2f(i*(float)W/kol, (H-h)/2));
    }
}