#include <iostream>
#include <math.h>
#include <vector>
#include <SFML/Graphics.hpp>

class GeomObject : public sf::Drawable, public sf::Transformable
{
protected:
    sf::Vector2f pos;

protected: 
    float mod(sf::Vector2f pos);
    sf::Vector2f ort(sf::Vector2f pos);
    sf::Vector2f norm(sf::Vector2f pos);
    float dot(sf::Vector2f a, sf::Vector2f b);
    float dat(sf::Vector2f a, sf::Vector2f b);
    bool lin(sf::Vector2f a, sf::Vector2f b, sf::Vector2f x);
    sf::Vector2f trans(sf::Vector2f o, sf::Vector2f a, float al);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
public:
    GeomObject(sf::Vector2f pos_);

    virtual std::vector<sf::Vector2f> collision(sf::Vector2f pls) = 0;
    virtual float intersect(sf::Vector2f rp, sf::Vector2f rv, float rl) = 0;
};

class SphearObj : public GeomObject
{
protected:
    sf::CircleShape obj;
    float r;
protected: 
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    SphearObj(sf::Vector2f pos_, float size);

    virtual std::vector<sf::Vector2f> collision(sf::Vector2f pls);
    virtual float intersect(sf::Vector2f rp, sf::Vector2f rv, float rl);
};

class LineObj : public GeomObject
{
protected:
    sf::Vertex obj[2];
    float a1, b1;
    float a2, b2;
protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    LineObj(sf::Vector2f pos_, float l, float al = 0);
    LineObj(float a1_, float b1_, float a2_, float b2_);

    virtual std::vector<sf::Vector2f> collision(sf::Vector2f pls);
    virtual float intersect(sf::Vector2f rp, sf::Vector2f rv, float rl);
};

class BoxObj : public GeomObject
{
protected:
    sf::Vertex obj[5];
    float a;
    float b;
    float al;
protected: 
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    BoxObj(sf::Vector2f pos_, float a_, float b_, float al_=0);

    virtual std::vector<sf::Vector2f> collision(sf::Vector2f pls);
    virtual float intersect(sf::Vector2f rp, sf::Vector2f rv, float rl);
};

class RectObj : public GeomObject
{
protected:
    sf::RectangleShape obj;
    float a;
    float b;
    float al;
protected: 
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    RectObj(sf::Vector2f pos_, float a_, float b_, float al_=0);

    virtual std::vector<sf::Vector2f> collision(sf::Vector2f pls);
    virtual float intersect(sf::Vector2f rp, sf::Vector2f rv, float rl);
};