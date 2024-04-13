#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <SFML/Graphics.hpp>

// typedef Settings::get() sett

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

    // float mod(sf::Vector2f pos);
    // sf::Vector2f ort(sf::Vector2f pos);
    // sf::Vector2f norm(sf::Vector2f pos);
    // float dot(sf::Vector2f a, sf::Vector2f b);
    // float dat(sf::Vector2f a, sf::Vector2f b);
    // bool lin(sf::Vector2f a, sf::Vector2f b, sf::Vector2f x);
    // sf::Vector2f trans(sf::Vector2f o, sf::Vector2f a, float al);

    int W = 1200;
    int H = 800;
    int discretization = 180;
    float len = 600.f;
    float visual_al = 90.f;
    float size = 3.f;
    float scale = 1.f/6;
    
    Settings(Settings const&) = delete;
    Settings& operator=(Settings const&) = delete;
    static Settings* get();
    static void reset();
    ~Settings();
};

extern Settings* sett;