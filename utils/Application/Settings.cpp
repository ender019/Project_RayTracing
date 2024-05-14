#include "Settings.hpp"

float Settings::mod(sf::Vector3f pos){return std::sqrt(pos.x*pos.x + pos.y*pos.y + pos.z*pos.z);}
sf::Vector3f Settings::ort(sf::Vector3f pos){return sf::Vector3f(pos.x/mod(pos), pos.y/mod(pos), pos.z/mod(pos));}
sf::Vector3f Settings::norm(sf::Vector3f pos){return ort(sf::Vector3f(pos.y, -pos.x, pos.y));}
float Settings::dot(sf::Vector3f a, sf::Vector3f b) {return a.x*b.x + a.y*b.y + a.z*b.z;}
sf::Vector3f Settings::dat(sf::Vector3f a, sf::Vector3f b) {return sf::Vector3f(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);}
bool Settings::lin(sf::Vector3f a, sf::Vector3f b, sf::Vector3f x) {return mod(a-x)+mod(x-b)-mod(a-b)<=0.0001;}
bool Settings::inal(sf::Vector3f a, sf::Vector3f b, sf::Vector3f x){return dot(ort(x), a+b) >= abs(dot(ort(a), a+b));}
void Settings::rot(sf::Vector3f& a, sf::Vector3f al)
{
    al.z = M_PI*al.z/180;    al.y = M_PI*al.y/180;    al.x = M_PI*al.x/180;
    a = sf::Vector3f(a.x, a.y*std::cos(al.x)-a.z*std::sin(al.x), a.y*std::sin(al.x)+a.z*std::cos(al.x));
    a = sf::Vector3f(a.x*std::cos(al.y)+a.z*std::sin(al.y), a.y, -a.x*std::sin(al.y)+a.z*std::cos(al.y));
    a = sf::Vector3f(a.x*std::cos(al.z)-a.y*std::sin(al.z), a.x*std::sin(al.z)+a.y*std::cos(al.z), a.z);
}
sf::Vector3f Settings::trans(sf::Vector3f al, sf::Vector3f a, sf::Vector3f o)
{
    al.z = M_PI*al.z/180;    al.y = M_PI*al.y/180;    al.x = M_PI*al.x/180;
    a = sf::Vector3f(a.x, o.y+(a.y-o.y)*std::cos(al.x)-(a.z-o.z)*std::sin(al.x), o.z+(a.y-o.y)*std::sin(al.x)+(a.z-o.z)*std::cos(al.x));
    a = sf::Vector3f(o.x+(a.x-o.x)*std::cos(al.y)+(a.z-o.z)*std::sin(al.y), a.y, o.z-(a.x-o.x)*std::sin(al.y)+(a.z-o.z)*std::cos(al.y));
    a = sf::Vector3f(o.x+(a.x-o.x)*std::cos(al.z)-(a.y-o.y)*std::sin(al.z), o.y+(a.x-o.x)*std::sin(al.z)+(a.y-o.y)*std::cos(al.z), a.z);
    return a;
}


Settings::Settings(): vission(discr.x*discr.y*4, 255) {}

Settings* Settings::get() {
    if (!settings) {settings = new Settings();}
    return settings;
}

void Settings::reset() {
    if (settings) {delete settings;}
}

Settings::~Settings(){delete settings;}