#include "Character.hpp"

float Character::mod(sf::Vector2f pos){return std::sqrt(pos.x*pos.x + pos.y*pos.y);}
float Character::dot(sf::Vector2f a, sf::Vector2f b){return a.x*b.x + a.y*b.y;}
float Character::dat(sf::Vector2f a, sf::Vector2f b) {return a.x*b.y - a.y*b.x;}
void Character::rot(sf::Vector2f& a, float al)
{
    al = M_PI*al/180;
    float x = a.x*std::cos(al)-a.y*std::sin(al);
    float y = a.x*std::sin(al)+a.y*std::cos(al);
    a.x = x; a.y = y;
}

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(camera, states);
    target.draw(conture, states);
}

Character::Character(sf::Vector2f pos_, float nal_): 
    camera(3.f), conture(sf::LineStrip, sett->discretization+2), rays(sett->discretization, sf::Vector2f(std::cos(M_PI*nal_/180), std::sin(M_PI*nal_/180))),
    pos(pos_), ray_kol(sett->discretization), size(sett->size), vis_al(sett->visual_al), nal(sf::Vector2f(std::cos(M_PI*nal_/180), std::sin(M_PI*nal_/180)))
{
    camera.setFillColor(sf::Color::Black);
    camera.move(pos-sf::Vector2f(size, size)); 
    conture[0].color = sf::Color::Red;
    conture[ray_kol+1].color = sf::Color::Red;
    conture[0].position = pos;
    conture[ray_kol+1].position = pos;
    for (int i = 0; i < ray_kol; i++){rot(rays[i], vis_al*i/ray_kol-vis_al/2);}
}

void Character::rotate(float w)
{
    rot(nal, w*feeling);
    for (int i = 0; i < ray_kol; i++){rot(rays[i], w*feeling);}
}
void Character::move(std::vector<GeomObject*> objects, float p)
{
    sf::Vector2f vec = nal;
    sf::Vector2f d_v=sf::Vector2f(0, 0);
    std::vector<sf::Vector2f> n;
    bool fl=0, fr=0;
    rot(vec, p);
    sf::Vector2f l=vec, r=vec;
    for (int i = 0; i < objects.size(); i++)
    {
        n = objects[i]->collision(pos);
        for (int j = 0; j < n.size(); j++)
        {
            if(dot(vec, n[j])<dot(vec, r) && dat(vec, n[j])>0) {r = n[j];}
            if(dot(vec, n[j])<dot(vec, l) && dat(vec, n[j])<0) {l = n[j];}
        }
    }
    if(dot(-vec, r+l) > abs(dot(l, r+l))) {d_v = vec;}
    else if(dot(vec, r)<0) {d_v = dot(vec, r)*r;}
    else if(dot(vec, l)<0) {d_v = dot(vec, l)*l;}
    vec -= d_v; pos+=vec;
    camera.move(vec);
    conture[0].position=pos;
    conture[ray_kol+1].position=pos;
}
std::vector<Settings::vis_point> Character::tracing(std::vector<GeomObject*> objects)
{
    std::vector<Settings::vis_point> mat(ray_kol, {sett->len, sf::Color::Blue});
    Settings::vis_point ox; 
    for (int i = 0; i < ray_kol; i++)
    {
        for (int j = 0; j < objects.size(); j++)
        {
            ox = objects[j]->intersect(pos, rays[i]);
            if(ox.dist < mat[i].dist) mat[i] = ox;
        }
        conture[i+1].position = pos + mat[i].dist*rays[i];
    }
    return mat;
}