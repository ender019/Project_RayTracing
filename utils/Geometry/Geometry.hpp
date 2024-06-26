#pragma once

#include "Settings.hpp"

class GeomObject : public sf::Drawable, public sf::Transformable
{
protected:
    sf::Vector2f pos;
    sf::Color rgb;

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
    GeomObject(sf::Vector2f pos_, sf::Color rgb_);

    virtual void scale(GeomObject*& c) = 0;
    virtual std::vector<sf::Vector2f> collision(sf::Vector2f pls) = 0;
    virtual Settings::vis_point intersect(sf::Vector2f rp, sf::Vector2f rv) = 0;
};

class CircleObj : public GeomObject
{
protected:
    sf::CircleShape obj;
    float r;
protected: 
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    CircleObj(sf::Vector2f pos_, float r_);

    virtual void scale(GeomObject*& c);
    virtual std::vector<sf::Vector2f> collision(sf::Vector2f pls);
    virtual Settings::vis_point intersect(sf::Vector2f rp, sf::Vector2f rv);
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

    virtual void scale(GeomObject*& c);
    virtual std::vector<sf::Vector2f> collision(sf::Vector2f pls);
    virtual Settings::vis_point intersect(sf::Vector2f rp, sf::Vector2f rv);
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

    virtual void scale(GeomObject*& c);
    virtual std::vector<sf::Vector2f> collision(sf::Vector2f pls);
    virtual Settings::vis_point intersect(sf::Vector2f rp, sf::Vector2f rv);
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

    virtual void scale(GeomObject*& c);
    virtual std::vector<sf::Vector2f> collision(sf::Vector2f pls);
    virtual Settings::vis_point intersect(sf::Vector2f rp, sf::Vector2f rv);
};