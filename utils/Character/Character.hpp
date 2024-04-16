#pragma once

#include "Geometry.hpp"

class Character : public sf::Drawable, public sf::Transformable
{
private:
    sf::CircleShape camera;
    std::vector<sf::Vector2f> rays;
    sf::VertexArray conture;
    sf::Vector2f pos;
    sf::Vector2f nal;
    float size;
    float feeling = 2;
    int ray_kol;
    float vis_al;

    friend class Map;

protected: 
    float mod(sf::Vector2f pos);
    float dot(sf::Vector2f a, sf::Vector2f b);
    float dat(sf::Vector2f a, sf::Vector2f b);
    void rot(sf::Vector2f& a, float al);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
public:
    Character(sf::Vector2f pos_, float nal_=-90);

    void rotate(float w);
    void move(std::vector<GeomObject*> objects, float p);
    std::vector<Settings::vis_point> tracing(std::vector<GeomObject*> objects);
};