#pragma once

#include "Settings.hpp"

class GeomObject : public sf::Drawable, public sf::Transformable
{
protected:
    sf::Vector3f pos;
    sf::Color rgb;

protected: 

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
public:
    GeomObject(sf::Vector3f pos_, sf::Color rgb_);

    virtual void push(std::vector<std::vector<float>>& shd_geom) = 0;
    virtual void scale(GeomObject*& c) = 0;
    virtual std::vector<sf::Vector3f> collision(sf::Vector3f pls) = 0;
    virtual Settings::vis_point intersect(sf::Vector3f rp, sf::Vector3f rv) = 0;
};

class SphearObj : public GeomObject
{
protected:
    sf::CircleShape obj;
    float r;
protected: 
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    SphearObj(sf::Vector3f _pos, float _r);

    virtual void push(std::vector<std::vector<float>>& shd_geom);
    virtual void scale(GeomObject*& c);
    virtual std::vector<sf::Vector3f> collision(sf::Vector3f pls);
    virtual Settings::vis_point intersect(sf::Vector3f rp, sf::Vector3f rv);
};

class LineObj : public GeomObject
{
protected:
    sf::Vertex obj[2];
    sf::Vector3f a,b;
protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    LineObj(sf::Vector3f _pos, float l, sf::Vector3f al={0,0,0});
    LineObj(sf::Vector3f _a, sf::Vector3f _b);
    LineObj(float _x1, float _y1, float _z1, float _x2, float _y2, float _z2);

    virtual void push(std::vector<std::vector<float>>& shd_geom);
    virtual void scale(GeomObject*& c);
    virtual std::vector<sf::Vector3f> collision(sf::Vector3f pls);
    virtual Settings::vis_point intersect(sf::Vector3f rp, sf::Vector3f rv);
};

class BoxObj : public GeomObject
{
protected:
    std::vector<LineObj> obj;
    sf::Vector3f s;
    sf::Vector3f al;
protected: 
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    BoxObj(sf::Vector3f _pos, sf::Vector3f _s, sf::Vector3f _al={0, 0, 0});

    virtual void push(std::vector<std::vector<float>>& shd_geom);
    virtual void scale(GeomObject*& c);
    virtual std::vector<sf::Vector3f> collision(sf::Vector3f pls);
    virtual Settings::vis_point intersect(sf::Vector3f rp, sf::Vector3f rv);
};

class PlaneObj : public GeomObject
{
protected:
    sf::RectangleShape obj;
    sf::Vector3f nr;
    float d;
protected: 
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    PlaneObj(sf::Vector3f _pos, sf::Vector3f _k);

    virtual void push(std::vector<std::vector<float>>& shd_geom);
    virtual void scale(GeomObject*& c);
    virtual std::vector<sf::Vector3f> collision(sf::Vector3f pls);
    virtual Settings::vis_point intersect(sf::Vector3f rp, sf::Vector3f rv);
};

class RectObj : public GeomObject
{
protected:
    sf::RectangleShape obj;
    sf::Vector3f s;
    sf::Vector3f al;
protected: 
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    RectObj(sf::Vector3f _pos, sf::Vector3f _s, sf::Vector3f _al={0, 0, 0});

    virtual void push(std::vector<std::vector<float>>& shd_geom);
    virtual void scale(GeomObject*& c);
    virtual std::vector<sf::Vector3f> collision(sf::Vector3f pls);
    virtual Settings::vis_point intersect(sf::Vector3f rp, sf::Vector3f rv);
};