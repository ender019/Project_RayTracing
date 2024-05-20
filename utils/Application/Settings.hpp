#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <SFML/Graphics.hpp>

#define SET Settings

class Settings
{
    Settings();
    static Settings* settings;
public:
    struct vis_point
    {
        float dist;
        sf::Color rgb;
    };

    float mod(sf::Vector3f pos);
    sf::Vector3f ort(sf::Vector3f pos);
    sf::Vector3f norm(sf::Vector3f pos);
    float dot(sf::Vector3f a, sf::Vector3f b);
    sf::Vector3f dat(sf::Vector3f a, sf::Vector3f b);
    bool lin(sf::Vector3f a, sf::Vector3f b, sf::Vector3f x);
    bool inal(sf::Vector3f a, sf::Vector3f b, sf::Vector3f x);
    void rot(sf::Vector3f& a, sf::Vector3f al);
    sf::Vector3f trans(sf::Vector3f al, sf::Vector3f a={1,0,0}, sf::Vector3f o={0, 0, 0});

    int W = 1200;
    int H = 800;
    int thp = 6;
    sf::Vector2i discr = {1200, 800};
    float rast =1.f;
    float len = 1500.f;
    sf::Vector2f vis = {74.f, 40.f};
    float size = 3.f;
    float scale = 1.f/6.f;
    sf::Vector3f light = {1, 1, -1};
    int geom_kol = 5;
	float geom[5]={0, 0, 0, 0, 0};
    
    Settings(Settings const&) = delete;
    Settings& operator=(Settings const&) = delete;
    static Settings* get();
    static void reset();
    ~Settings();
};

extern Settings* sett;