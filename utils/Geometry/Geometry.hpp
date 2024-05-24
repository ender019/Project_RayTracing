#pragma once

#include "Settings.hpp"

class GeomObject : public sf::Drawable, public sf::Transformable
{
protected:
    sf::Vector3f pos;
    sf::Color rgb;
    sf::Vector3f mat; // коэффициенты отражения поглощения пропускания 

protected: 

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
public:
    GeomObject(sf::Vector3f _pos, sf::Color _rgb, sf::Vector3f _mat);

    virtual void push(std::vector<float>& shd_geom) = 0;
    virtual std::vector<sf::Vector3f> collision(sf::Vector3f pls) = 0;
    virtual float intersect(sf::Vector3f rp, sf::Vector3f rv) = 0;
};

class SphearObj : public GeomObject
{
protected:
    sf::CircleShape obj;
    float r;
protected: 
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    SphearObj(sf::Vector3f _pos, float _r, sf::Vector3f _mat);

    virtual void push(std::vector<float>& shd_geom);
    virtual std::vector<sf::Vector3f> collision(sf::Vector3f pls);
    virtual float intersect(sf::Vector3f rp, sf::Vector3f rv);
};

class LineObj : public GeomObject
{
protected:
    sf::Vertex obj[2];
    sf::Vector3f a,b;
protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    LineObj(sf::Vector3f _pos, float l, sf::Vector3f al={0,0,0}, sf::Vector3f _mat={1.f,0.f,0.f});
    LineObj(sf::Vector3f _a, sf::Vector3f _b, sf::Vector3f _mat);
    LineObj(float _x1, float _y1, float _z1, float _x2, float _y2, float _z2, sf::Vector3f _mat);

    virtual void push(std::vector<float>& shd_geom);
    virtual std::vector<sf::Vector3f> collision(sf::Vector3f pls);
    virtual float intersect(sf::Vector3f rp, sf::Vector3f rv);
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
    BoxObj(sf::Vector3f _pos, sf::Vector3f _s, sf::Vector3f _al={0, 0, 0}, sf::Vector3f _mat={1.f,0.f,0.f});

    virtual void push(std::vector<float>& shd_geom);
    virtual std::vector<sf::Vector3f> collision(sf::Vector3f pls);
    virtual float intersect(sf::Vector3f rp, sf::Vector3f rv);
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
    PlaneObj(sf::Vector3f _pos, sf::Vector3f _k, sf::Vector3f _mat);

    virtual void push(std::vector<float>& shd_geom);
    virtual std::vector<sf::Vector3f> collision(sf::Vector3f pls);
    virtual float intersect(sf::Vector3f rp, sf::Vector3f rv);
};

class CubeObj : public GeomObject
{
protected:
    sf::RectangleShape obj;
    sf::Vector3f s;
    sf::Vector3f al;
protected: 
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
    CubeObj(sf::Vector3f _pos, sf::Vector3f _s, sf::Vector3f _al, sf::Vector3f _mat);

    virtual void push(std::vector<float>& shd_geom);
    virtual std::vector<sf::Vector3f> collision(sf::Vector3f pls);
    virtual float intersect(sf::Vector3f rp, sf::Vector3f rv);
};


class ObjBase
{
private:
    int geom_kol = 5;
	std::vector<float> geom;
    std::vector<GeomObject*> objects;
    std::vector<std::vector<float>> cash_geom;
public:
    ObjBase();
    void createSphear(sf::Vector3f _pos, float _r, sf::Vector3f _mat = {1.f, 0.f, 0.f});
    void createPlane(sf::Vector3f _pos, sf::Vector3f _k, sf::Vector3f _mat = {1.f, 0.f, 0.f});
    void createCube(sf::Vector3f _pos, sf::Vector3f _s, sf::Vector3f _al={0, 0, 0}, sf::Vector3f _mat = {1.f, 0.f, 0.f});
    int get_kol();
    std::vector<float> get_geom();
    std::vector<float> get_cash(int i);
    GeomObject* get_obj(int i);

    ~ObjBase();
};

extern ObjBase base;