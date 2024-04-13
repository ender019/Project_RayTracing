#include "Application.hpp"

int main() 
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(W, H), "window", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    sf::Clock clock;

    sf::Vector2f cam_pos(600, 700);
    sf::Vector2f center(W/2, H/2);
    int discretization = 180;
    std::vector<float> dist(discretization);
    bool act = 1;
    int fps = 0;
    float t = 0;

    sf::Font font;
    sf::Text fps_lable;
    if (!font.loadFromFile("times.ttf")) std::cout<<"uidsvnhvfsv\n";
	fps_lable.setFont(font); 
    fps_lable.setString("0");
    fps_lable.setCharacterSize(20); 
	fps_lable.setFillColor(sf::Color::Black);
	fps_lable.setPosition(sf::Vector2f(W-35,8));

    Character camera(cam_pos, discretization, 600);
    std::vector<GeomObject*> objects;
    objects.push_back(new SphearObj(sf::Vector2f(700,300), 30));
    objects.push_back(new SphearObj(sf::Vector2f(750,200), 50));
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
    objects.push_back(new BoxObj(sf::Vector2f(600,400), 1180, 780));

    Screen screen(discretization);
    Map map(objects, camera);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }
        sf::Time elapsed = clock.restart();
        sf::Vector2f dx = window.mapPixelToCoords(sf::Mouse::getPosition(window)) - center;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)){camera.move(objects, 3);}
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){camera.move(objects, 2);}
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){camera.move(objects, 1);}
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){camera.move(objects, 0);}
        sf::Mouse::setPosition(sf::Vector2i(W/2, H/2), window);
        camera.rotate(dx.x*elapsed.asSeconds());
        window.clear(sf::Color::White);

        dist = camera.tracing(objects);
        screen.update(dist);
        map.update(camera);

        window.draw(screen);
        window.draw(map);
        t+=elapsed.asSeconds(); fps++;
        if(t>=1){fps_lable.setString(std::to_string(fps)); fps=0; t=0;}
        window.draw(fps_lable);
        window.display();
    }

    for (int i = 0; i < objects.size(); i++){delete objects[i];}

    return 0;
}