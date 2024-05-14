#include "Application.hpp"
#include <SFML/Audio.hpp>

void Screen::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(floor, states);
    target.draw(sky, states);
    for (int i = 0; i < kol; i++) {target.draw(vission[i]);}
}

Screen::Screen(int kol_): 
    floor(sf::Vector2f(sett->W, sett->H/2)), sky(sf::Vector2f(sett->W, sett->H/2)), 
    vission(kol_, sf::RectangleShape(sf::Vector2f(sett->W/kol_, sett->H/2))), kol(kol_)
{
    floor.setFillColor(sf::Color(200, 165, 0));
    sky.setFillColor(sf::Color(165, 165, 255));
    floor.move(sf::Vector2f(0, sett->H/2));
    for (int i = 0; i < kol; i++)
    {
        vission[i].move(sf::Vector2f(i*(sett->W/kol), sett->H/2));
        vission[i].setFillColor(sf::Color::Red);
    }
}
    
void Screen::update(std::vector<Settings::vis_point> dist)
{
    float h, dc;
    for (int i = 0; i < kol; i++)
    {
        if(dist[i].dist>=sett->len-1) h=0;
        else{
            dist[i].dist*=cos(M_PI*sett->visual_al/180*((float)kol/2-i)/kol);
            h=sett->H*sett->rast/dist[i].dist;
            dc = 255-100*dist[i].dist/sett->len;
        }
        vission[i].setSize(sf::Vector2f((float)sett->W/kol, h));
        vission[i].setPosition(sf::Vector2f(i*(float)sett->W/kol, (sett->H-h)/2));
        vission[i].setFillColor(dist[i].rgb*sf::Color(dc,dc,dc));
    }
}


App::App(): 
    window(sf::VideoMode(sett->W, sett->H), "window", sf::Style::Default, settings), 
    cam_pos(800, 700), dist(sett->discretization),
    camera(cam_pos), screen(sett->discretization), map(camera)
{
	settings.antialiasingLevel = 8;
    window.setFramerateLimit(60);

    objects.push_back(new CircleObj(sf::Vector2f(700,300), 30));
    objects.push_back(new CircleObj(sf::Vector2f(750,200), 50));
    objects.push_back(new RectObj(sf::Vector2f(380,280), 150, 130));
    objects.push_back(new RectObj(sf::Vector2f(580,480), 50, 30, 45));
    objects.push_back(new LineObj(100,100,900,300));
    objects.push_back(new LineObj(sf::Vector2f(650,150), 620, 120));
    objects.push_back(new LineObj(sf::Vector2f(340,655), 450, -30));
    objects.push_back(new LineObj(sf::Vector2f(1000,650), 56, -90));
    objects.push_back(new LineObj(sf::Vector2f(1006,650), 50, -90));
    objects.push_back(new LineObj(sf::Vector2f(1000,594), 56));
    objects.push_back(new LineObj(sf::Vector2f(1006,600), 50));
    objects.push_back(new BoxObj(sf::Vector2f(840,350), 120, 80));
    objects.push_back(new BoxObj(sf::Vector2f(560,350), 140, 40, 30));
    objects.push_back(new BoxObj(sf::Vector2f(sett->W/2,sett->H/2), sett->W-20, sett->H-20));
    map.reload(objects);
}
void App::run()
{
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }
        sf::Time elapsed = clock.restart();
        sf::Vector2f dx = window.mapPixelToCoords(sf::Mouse::getPosition(window)) - sf::Vector2f(sett->W/2, sett->H/2);
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){camera.move(objects, 0);}
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){camera.move(objects, 180);}
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){camera.move(objects, -90);}
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){camera.move(objects, 90);}
        sf::Mouse::setPosition(sf::Vector2i(sett->W/2, sett->H/2), window);
        camera.rotate(dx.x*elapsed.asSeconds());
        window.clear(sf::Color::White);

        dist = camera.tracing(objects);
        screen.update(dist);
        map.update(camera);

        window.draw(screen);
        window.draw(map);
        // counter.update(elapsed.asSeconds());
        // window.draw(counter);
        window.display();
    }
}

App::~App()
{
    for (int i = 0; i < objects.size(); i++){delete objects[i];}
}