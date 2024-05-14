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

Map::Map(Character player_, float p): 
    back(sf::Vector2f(p*sett->W, p*sett->H)),
    conture(sf::LineStrip, player_.ray_kol+2), camera(p*player_.size), kol(player_.ray_kol+2), part(p)
{
    back.setOutlineThickness(3);
    back.setOutlineColor(sf::Color::Black);
    camera.setFillColor(sf::Color::Black);
    camera.move(p*player_.pos);
    for (int i = 0; i < kol; i++)
    {
        conture[i] = player_.conture[i];
        conture[i].position = p*player_.conture[i].position;
    }
}

void Map::reload(std::vector<GeomObject*> objects_)
{
    if(objects.size()>0)
    {
        for (int i = 0; i < objects.size(); i++){delete objects[i];}
    }
    objects.resize(objects_.size());
    for (int i = 0; i < objects.size(); i++){objects_[i]->scale(objects[i]);}
}

void Map::update(Character player)
{
    camera.setPosition(part * player.pos);
    conture = player.conture;
    for (int i = 0; i < kol; i++){conture[i].position = part*player.conture[i].position;}
}

Map::~Map(){for (int i = 0; i < objects.size(); i++){delete objects[i];}}


// void FPS::draw(sf::RenderTarget& target, sf::RenderStates states) const
// {
//     states.transform *= getTransform();
//     states.texture = NULL;
//     target.draw(fps_lable, states);
// }

// FPS::FPS()
// {
//     if (!font.loadFromFile("times.ttf")) std::cout<<"error\n";
// 	fps_lable.setFont(font); 
//     fps_lable.setString("0");
//     fps_lable.setCharacterSize(20); 
// 	fps_lable.setFillColor(sf::Color::Black);
// 	fps_lable.setPosition(sf::Vector2f(sett->W-35,8));
// }

// void FPS::update(float dt)
// {
//     t+=dt; fps++;
//     if(t>=1){fps_lable.setString(std::to_string(fps)); fps=0; t=0;}
// }