#include "Character.hpp"

void Character::rot(sf::Vector3f& a, sf::Vector3f al)
{
    al.z = M_PI*al.z/180;    al.y = M_PI*al.y/180;
    a = sf::Vector3f(std::cos(al.y), 0, -std::sin(al.y));
    a = sf::Vector3f(a.x*std::cos(al.z), a.x*std::sin(al.z), a.z);
}

void Character::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(camera, states);
    target.draw(conture, states);
}

Character::Character(sf::Vector3f _pos, sf::Vector3f _nal): 
    camera(3.f), conture(sf::LineStrip, sett->W+2), rays(sett->H, std::vector<sf::Vector3f>(sett->W, {1,0,0})),
    pos(_pos), ray_kol(sf::Vector2i(sett->W, sett->H)), size(sett->size), nal(_nal)
{
    camera.setFillColor(sf::Color::Black);
    camera.move({pos.x-size, pos.y-size}); 
    conture[0].position = {pos.x, pos.y};
    conture[ray_kol.x+1].position = {pos.x, pos.y};
    for (int i = 0; i < ray_kol.y; i++){
        for (int j = 0; j < ray_kol.x; j++) {rot(rays[i][j], {0, nal.y+sett->vis.y*((float)i/ray_kol.y-0.5f), nal.z+sett->vis.x*((float)j/ray_kol.x-0.5f)});}
    }
}

void Character::rotate(sf::Vector3f w)
{
    if(abs(nal.y + w.y*feeling)>89) {nal.y=(1-2*(nal.y<0))*89;} //std::cout<<nal.y<<'\n';
    else nal += w*feeling;
    for (int j = 0; j < ray_kol.x; j++) {rot(rays[399][j], {0, 0, nal.z+sett->vis.x*((float)j/ray_kol.x-0.5f)});}
}

void Character::move(sf::Vector3f p)
{
    sf::Vector3f vec = {2,0,0}, d_v = {0, 0, 0};
    std::vector<sf::Vector3f> n;
    rot(vec, nal+p);
    sf::Vector3f l=vec, r=vec;
    for (int i = 0; i < base.get_kol(); i++)
    {
        n = base.get_obj(i)->collision(pos);
        for (int j = 0; j < n.size(); j++)
        {
            // if(sett->dot(vec, n[j])<sett->dot(vec, r) && sett->dat(vec, n[j])>0) {r = n[j];}
            // if(sett->dot(vec, n[j])<sett->dot(vec, l) && sett->dat(vec, n[j])<0) {l = n[j];}
            d_v = sett->dot(vec, n[j])*n[j];
        }
    }
    // if(sett->dot(-vec, r+l) > abs(sett->dot(l, r+l))) {d_v = vec;}
    // else if(sett->dot(vec, r)<0) {r;}
    // else if(sett->dot(vec, l)<0) {d_v = sett->dot(vec, l)*l;}
    // std::cout<<vec.x<<' '<<vec.y<<' '<<vec.z<<"\n";
    // std::cout<<nal.x<<' '<<nal.y<<' '<<nal.z<<"\n";
    vec -= d_v; pos+=vec;
    camera.move({vec.x, vec.y});
    conture[0].position={pos.x, pos.y};
    conture[ray_kol.x+1].position={pos.x, pos.y};
}

void Character::scan()
{
    float ox, pix;
    for (int i = 0; i < sett->W; i++)
    {
        pix=sett->len;
        if (rays[399][i].y>0) pix=std::min((sett->H-pos.y)/rays[399][i].y, pix);
        if (rays[399][i].y<0) pix=std::min(-pos.y/rays[399][i].y, pix);
        if (rays[399][i].x>0) pix=std::min((sett->W-pos.x)/rays[399][i].x, pix);
        if (rays[399][i].x<0) pix=std::min(-pos.x/rays[399][i].x, pix);
        for (int j = 0; j < base.get_kol(); j++)
        {
            ox=base.get_obj(j)->intersect(pos, rays[399][i]);
            pix=std::min(pix, ox);
        }
        conture[i+1].position={pos.x+pix*rays[399][i].x, pos.y+pix*rays[399][i].y};
    }
}

void Character::tracing(sf::Shader& shader)
{
    shader.setUniform("pos", pos);
    shader.setUniform("nal", nal);
    shader.setUniformArray("geom", base.get_geom().data(), base.get_kol());
	shader.setUniformArray("sphears", base.get_cash(0).data(), base.get_cash(0).size());
	shader.setUniformArray("planes", base.get_cash(3).data(), base.get_cash(3).size());
	shader.setUniformArray("cubes", base.get_cash(4).data(), base.get_cash(4).size());
}

