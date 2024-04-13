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

Character::Character(sf::Vector2f pos_, int kol_, float dist_, float size_, float al_, float nal_): 
    camera(3.f), conture(sf::LineStrip, kol_+2), rays(kol_, sf::Vector2f(std::cos(M_PI*nal_/180), std::sin(M_PI*nal_/180))),
    pos(pos_), ray_kol(kol_), dist(dist_), size(size_), vis_al(al_), nal(nal_)
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
    nal+=w*feeling;
    for (int i = 0; i < ray_kol; i++){rot(rays[i], w*feeling);}
}
void Character::move(std::vector<GeomObject*> objects, int p)
{
    sf::Vector2f vec((2*(p%2)-1)*(p>1), (2*(p%2)-1)*(p<2));
    sf::Vector2f d_v=sf::Vector2f(0, 0);
    std::vector<sf::Vector2f> n;
    bool fl=0, fr=0;
    rot(vec, nal);
    sf::Vector2f l=vec, r=vec;
    for (int i = 0; i < objects.size(); i++)
    {
        n = objects[i]->collision(pos, size);
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
std::vector<float> Character::tracing(std::vector<GeomObject*> objects)
{
    std::vector<float> mat(ray_kol, dist);
    for (int i = 0; i < ray_kol; i++)
    {
        for (int j = 0; j < objects.size(); j++)
        {
            float ox = objects[j]->intersect(pos, rays[i], dist);
            mat[i] = std::min(ox, mat[i]);
        }
        conture[i+1].color = sf::Color::Red;
        conture[i+1].position = pos + mat[i]*rays[i];
    }
    return mat;
}