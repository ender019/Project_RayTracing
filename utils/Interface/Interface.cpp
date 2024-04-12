#include "Interface.hpp"

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(back, states);
    for (int i = 0; i < objects.size(); i++) {target.draw(*objects[i]);}
    target.draw(camera, states);
    target.draw(conture, states);
}

Map::Map(std::vector<GeomObject*> objects_, Character player_, float p): 
    back(sf::Vector2f(p*W, p*H)), objects(objects_.size()), 
    conture(sf::LineStrip, player_.ray_kol+2), camera(p*player_.size), kol(player_.ray_kol+2), part(p)
{
    // back.setFillColor(sf::Color(220, 220, 220));
    back.setOutlineThickness(3);
    back.setOutlineColor(sf::Color::Black);
    camera.setFillColor(sf::Color::Black);
    camera.move(p*player_.pos);
    for (int i = 0; i < kol; i++)
    {
        conture[i] = player_.conture[i];
        conture[i].position = p*player_.conture[i].position;
    }
    for (int i = 0; i < objects.size(); i++){objects_[i]->scale(objects[i], p);}
}

void Map::update(Character player)
{
    camera.setPosition(part * player.pos);
    conture = player.conture;
    for (int i = 0; i < kol; i++){conture[i].position = part*player.conture[i].position;}
}

Map::~Map(){for (int i = 0; i < objects.size(); i++){delete objects[i];}}