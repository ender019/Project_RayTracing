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

Map::Map(Character _player, float p): 
    back(sf::Vector2f(p*sett->W, p*sett->H)),
    conture(sf::LineStrip, _player.ray_kol.x+2), camera(p*_player.size), kol(_player.ray_kol.x+2), part(p)
{
    back.setOutlineThickness(3);
    back.setOutlineColor(sf::Color::Black);
    camera.setFillColor(sf::Color::Black);
    camera.move(p*sf::Vector2f(_player.pos.x, _player.pos.y));
    for (int i = 0; i < kol; i++)
    {
        conture[i].position = p*_player.conture[i].position;
        conture[i].color = sf::Color::Red;
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
    camera.setPosition(part * sf::Vector2f(player.pos.x, player.pos.y));
    for (int i = 0; i < kol; i++){conture[i].position = part*player.conture[i].position;}
}

Map::~Map(){for (int i = 0; i < objects.size(); i++){delete objects[i];}}


void FPS::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(fps_lable, states);
}

FPS::FPS()
{
    if (!font.loadFromFile("times.ttf")) std::cout<<"error\n";
	fps_lable.setFont(font); 
    fps_lable.setString("0");
    fps_lable.setCharacterSize(20); 
	fps_lable.setFillColor(sf::Color::Black);
	fps_lable.setPosition(sf::Vector2f(sett->W-35,8));
}

void FPS::update(float dt)
{
    t+=dt; fps++;
    if(t>=1){fps_lable.setString(std::to_string(fps)); fps=0; t=0;}
}