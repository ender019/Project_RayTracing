#include "Application.hpp"
#include <SFML/Audio.hpp>

void Screen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(obj);
}

Screen::Screen(): vission(sett->discr.x*sett->discr.y*4), kol(sett->discr)
{
    tex.create(sett->discr.x, sett->discr.y); 
    obj.setTexture(tex);
    for (int i = 0; i < kol.x*kol.y; i++) {vission[i] = 255;}
}
    
void Screen::update(std::vector<Settings::vis_point> dist)
{
    float h, dc;
    for (int i = 0; i < kol.x*kol.y; i++)
    {
        // if(dist[i].dist>=sett->len-1) { dc=255;}
        // else dc = 255-50*dist[i].dist/sett->len;
        vission[4*i] = dist[i].rgb.r; //*sf::Color(dc,dc,dc);
        vission[4*i+1] = dist[i].rgb.g;
        vission[4*i+2] = dist[i].rgb.b;
        vission[4*i+3] = dist[i].rgb.a;
    }
    tex.update(vission.data());
}


App::App(): 
    window(sf::VideoMode(sett->W, sett->H), "window", sf::Style::Default, settings), 
    cam_pos(300, 300, 30), dist(sett->discr.x),
    camera(cam_pos), screen(), map(camera)
{
	settings.antialiasingLevel = 8;
    window.setFramerateLimit(60);

    objects.push_back(new SphearObj(sf::Vector3f(600, 250, 30), 30));
    // objects.push_back(new SphearObj(sf::Vector2f(750,200), 50));
    objects.push_back(new RectObj({780, 350, 30}, {50, 50, 50}));
    // objects.push_back(new RectObj(sf::Vector2f(580,480), 50, 30, 45));
    // objects.push_back(new LineObj(100,100,900,300));
    // objects.push_back(new LineObj(sf::Vector2f(650,150), 620, 120));
    // objects.push_back(new LineObj(sf::Vector2f(340,655), 450, -30));
    // objects.push_back(new LineObj(sf::Vector2f(1000,650), 56, -90));
    // objects.push_back(new LineObj(sf::Vector2f(1006,650), 50, -90));
    // objects.push_back(new LineObj(sf::Vector2f(1000,594), 56));
    // objects.push_back(new LineObj(sf::Vector2f(1006,600), 50));
    // objects.push_back(new BoxObj(sf::Vector2f(840,350), 120, 80));
    // objects.push_back(new BoxObj(sf::Vector2f(560,350), 140, 40, 30));
    // objects.push_back(new BoxObj(sf::Vector2f(sett->W/2,sett->H/2), sett->W-20, sett->H-20));
    map.reload(objects);
}
void App::run()
{
    // std::cout<<sett->discr.x*sett->discr.y<<'\n';
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }
        sf::Time elapsed = clock.restart();
        sf::Vector2f dx = window.mapPixelToCoords(sf::Mouse::getPosition(window)) - sf::Vector2f(sett->W/2, sett->H/2);
        // if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){camera.move(objects, {0,0,0});}
        // if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){camera.move(objects, {0,0,180});}
        // if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){camera.move(objects, {0,0,-90});}
        // if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){camera.move(objects, {0,0,90});}
        // sf::Mouse::setPosition(sf::Vector2i(sett->W/2, sett->H/2), window);
        // camera.rotate({0,dx.y*elapsed.asSeconds(),dx.x*elapsed.asSeconds()});
        window.clear(sf::Color::White);

        dist = camera.tracing(objects);
        screen.update(dist);
        map.update(camera);

        window.draw(screen);
        window.draw(map);
        counter.update(elapsed.asSeconds());
        window.draw(counter);
        window.display();
    }
}

App::~App()
{
    for (int i = 0; i < objects.size(); i++){delete objects[i];}
}