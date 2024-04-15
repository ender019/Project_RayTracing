#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include <SFML/Graphics.hpp>

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

    int W = 1200;
    int H = 800;
    int discretization = 1800;
    float rast =30.f;
    float len = 1500.f;
    float visual_al = 60.f;
    float size = 3.f;
    float scale = 1.f/6;
    
    Settings(Settings const&) = delete;
    Settings& operator=(Settings const&) = delete;
    static Settings* get();
    static void reset();
    ~Settings();
};

extern Settings* sett;