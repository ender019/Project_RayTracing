#pragma once

#include "Geometry.hpp"

class Character : public sf::Drawable, public sf::Transformable
{
private:
    sf::CircleShape camera;
    std::vector<std::vector<sf::Vector3f>> rays;
    sf::VertexArray conture;
    sf::Vector3f pos;
    sf::Vector3f nal;
    float size;
    float feeling = 0.5f;
    sf::Vector2i ray_kol;

    friend class Map;

protected: 
    void rot(sf::Vector3f& a, sf::Vector3f al);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
public:
    Character(sf::Vector3f _pos, sf::Vector3f _nal ={0,0,0});

    void rotate(sf::Vector3f w);
    void move(std::vector<GeomObject*> objects, sf::Vector3f p);
    void tracing(std::vector<GeomObject*> objects);
};