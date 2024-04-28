#include "Character.hpp"

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(camera, states);
    target.draw(conture, states);
}

Character::Character(sf::Vector3f _pos, sf::Vector3f _nal): 
    camera(3.f), conture(sf::LineStrip, sett->discr.x+2), rays(sett->discr.y, std::vector<sf::Vector3f>(sett->discr.x, sett->trans(_nal))),
    pos(_pos), ray_kol(sett->discr), size(sett->size), nal(_nal)
{
    camera.setFillColor(sf::Color::Black);
    camera.move(sf::Vector2f(pos.x, pos.y)-sf::Vector2f(size, size)); 
    conture[0].position = sf::Vector2f(pos.x, pos.y);
    conture[ray_kol.x+1].position = sf::Vector2f(pos.x, pos.y);
    for (int i = 0; i < ray_kol.y; i++){
        for (int j = 0; j < ray_kol.x; j++) {sett->rot(rays[i][j], {0, sett->vis.y*((float)i/ray_kol.y-0.5f), sett->vis.x*((float)j/ray_kol.x-0.5f)});}
    }
    // std::cout<<rays[399][599].x<<' '<<rays[399][599].y<<' '<<rays[399][599].z<<' '<<'\n';
    // std::cout<<rays[399][0].x<<' '<<rays[399][0].y<<' '<<rays[399][0].z<<' '<<'\n';
    // std::cout<<rays[399][1199].x<<' '<<rays[399][1199].y<<' '<<rays[399][1199].z<<' '<<'\n';
    // std::cout<<rays[0][599].x<<' '<<rays[0][599].y<<' '<<rays[0][599].z<<' '<<'\n';
    // std::cout<<rays[799][599].x<<' '<<rays[799][599].y<<' '<<rays[799][599].z<<' '<<'\n';
}

void Character::rotate(sf::Vector3f w)
{
    sett->rot(nal, w*feeling);
    for (int i = 0; i < ray_kol.y; i++)
    {
        for (int j = 0; j < ray_kol.x; j++){sett->rot(rays[i][j], w*feeling);}
    }
}
void Character::move(std::vector<GeomObject*> objects, sf::Vector3f p)
{
    sf::Vector3f vec = nal;
    sf::Vector3f d_v = {0, 0, 0};
    std::vector<sf::Vector3f> n;
    sett->rot(vec, p);
    // sf::Vector3f l=vec, r=vec;
    // for (int i = 0; i < objects.size(); i++)
    // {
    //     n = objects[i]->collision(pos);
    //     for (int j = 0; j < n.size(); j++)
    //     {
    //         if(sett->dot(vec, n[j])<sett->dot(vec, r) && sett->dat(vec, n[j])>0) {r = n[j];}
    //         if(sett->dot(vec, n[j])<sett->dot(vec, l) && sett->dat(vec, n[j])<0) {l = n[j];}
    //     }
    // }
    // if(sett->dot(-vec, r+l) > abs(sett->dot(l, r+l))) {d_v = vec;}
    // else if(sett->dot(vec, r)<0) {d_v = sett->dot(vec, r)*r;}
    // else if(sett->dot(vec, l)<0) {d_v = sett->dot(vec, l)*l;}
    vec -= d_v; pos+=vec;
    camera.move(sf::Vector2f(vec.x, vec.y));
    conture[0].position=sf::Vector2f(pos.x, pos.y);
    conture[ray_kol.x+1].position=sf::Vector2f(pos.x, pos.y);
}
std::vector<Settings::vis_point> Character::tracing(std::vector<GeomObject*> objects)
{
    std::vector<Settings::vis_point> mat(ray_kol.x*ray_kol.y, {sett->len, sf::Color::White});
    Settings::vis_point ox; 
    for (int i = 0; i < ray_kol.y; i++)
    {
        for (int j = 0; j < ray_kol.x; j++)
        {
            for (int g = 0; g < objects.size(); g++)
            {
                ox = objects[g]->intersect(pos, rays[i][j]);
                if(ox.dist < mat[i*ray_kol.x+j].dist) mat[i*ray_kol.x+j] = ox;
            }
            if(i==399){
                conture[j+1].position = sf::Vector2f(pos.x, pos.y) + mat[i*ray_kol.x+j].dist*sf::Vector2f(rays[i][j].x, rays[i][j].y);
            }
        }
    }
    return mat;
}