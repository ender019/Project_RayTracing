#include "Geometry.hpp"

GeomObject::GeomObject(sf::Vector3f pos_, sf::Color rg_b): pos(pos_), rgb(rg_b) {}


void SphearObj::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(obj, states);
}

void SphearObj::push(std::vector<std::vector<float>>& shd_geom)
{
    shd_geom[0].push_back(pos.x);
    shd_geom[0].push_back(pos.y);
    shd_geom[0].push_back(pos.z);
    shd_geom[0].push_back(r);
    shd_geom[0].push_back(rgb.r);
    shd_geom[0].push_back(rgb.g);
    shd_geom[0].push_back(rgb.b);
    sett->geom[0]++;
}
void SphearObj::scale(GeomObject*& c)
{
    c = new SphearObj(sett->scale*pos, sett->scale*r);
}
SphearObj::SphearObj(sf::Vector3f _pos, float _r): GeomObject(_pos, sf::Color(0,0,255)), r(_r), obj(_r)
{
    obj.move(sf::Vector2f(_pos.x-r, _pos.y-r));
    obj.setFillColor(rgb);
}
std::vector<sf::Vector3f> SphearObj::collision(sf::Vector3f pls)
{
    float d = sett->mod(pls-pos) - r;
    if(d>sett->size) return {};
    return {sett->ort(pls-pos)};
}
Settings::vis_point SphearObj::intersect(sf::Vector3f rp, sf::Vector3f rv)
{
    sf::Vector3f aov=pos-rp;
    float ah = sett->dot(aov, rv);
    float ao = sett->dot(aov,aov);
    float bh = r*r - ao + ah*ah;
    if(bh < 0 || ah<0) return {sett->len, sf::Color::White};
    float h = sqrt(bh);
    float dc = 100+155*abs(sett->dot(sett->ort(sett->light), rv));
    // std::cout<<ah-h<<'\n';
    return {std::min(ah - h, sett->len), rgb};//*sf::Color(dc,dc,dc)
}


void LineObj::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(obj, 2, sf::Lines, states);
}

void LineObj::push(std::vector<std::vector<float>>& shd_geom)
{
    shd_geom[1].push_back(pos.x);
    shd_geom[1].push_back(pos.y);
    shd_geom[1].push_back(pos.z);
    shd_geom[1].push_back(a.x);
    shd_geom[1].push_back(rgb.r);
    shd_geom[1].push_back(rgb.g);
    shd_geom[1].push_back(rgb.b);
    sett->geom[1]++;
}
LineObj::LineObj(float x1_, float y1_, float z1_, float x2_, float y2_, float z2_): 
    GeomObject(sf::Vector3f(x1_, y1_, z1_), sf::Color(220,220,0)), 
    a(x1_, y1_, z1_), b(x2_, y2_, z2_)
{
    obj[0] = sf::Vertex(sf::Vector2f(a.x, a.y), rgb);
    obj[1] = sf::Vertex(sf::Vector2f(b.x, b.y), rgb);
}
LineObj::LineObj(sf::Vector3f _a, sf::Vector3f _b): 
    GeomObject(a, sf::Color(220,220,0)), 
    a(_a), b(_b)
{
    obj[0] = sf::Vertex (sf::Vector2f(a.x, a.y), rgb);
    obj[1] = sf::Vertex (sf::Vector2f(b.x, b.y), rgb);
}
LineObj::LineObj(sf::Vector3f _pos, float l, sf::Vector3f al): 
    GeomObject(_pos, sf::Color(220,220,0)) 
{
    *this = LineObj(_pos, _pos + sett->trans(al)*l);
}

void LineObj::scale(GeomObject*& c)
{
    c = new LineObj(sett->scale*a, sett->scale*b);
}
std::vector<sf::Vector3f> LineObj::collision(sf::Vector3f pls)
{
    sf::Vector3f v=b-a, ax = pls-a;
    float d = sett->mod(sett->dat(sett->ort(v), ax));
    if(abs(d)>sett->size || sett->mod(ax)>sett->mod(v) || sett->mod(pls-b)>sett->mod(v)) return {};
    sf::Vector3f n=sett->norm(v);
    return {sett->ort(n * sett->dot(n, ax))};
}
Settings::vis_point LineObj::intersect(sf::Vector3f rp, sf::Vector3f rv)
{
    sf::Vector3f v=sett->ort(b-a);
    sf::Vector3f n=sett->norm(v);
    float pa = sett->dot(n, rv);
    float h = sett->mod(sett->dat(v, rp-pos));
    sf::Vector3f x = rp + rv*abs(h/pa);
    if(sett->lin(a, b, x) && sett->len>=abs(h/pa))
    {
        float dc = 155+100*abs(pa); 
        return {abs(h/pa), rgb*sf::Color(dc,dc,dc)};
    }
    return {sett->len, rgb};
}


void BoxObj::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    for (int i = 0; i < 12; i++) target.draw(obj[i], states);
}
BoxObj::BoxObj(sf::Vector3f _pos, sf::Vector3f _s, sf::Vector3f _al): GeomObject(_pos, sf::Color(0,220,220)), s(_s), al(_al)
{
    obj.push_back(LineObj(sett->trans(pos, pos-(0.5f*s), al), sett->trans(pos, pos+(0.5f*s), al)));
}

void BoxObj::push(std::vector<std::vector<float>>& shd_geom)
{
    shd_geom[2].push_back(pos.x);
    shd_geom[2].push_back(pos.y);
    shd_geom[2].push_back(pos.z);
    shd_geom[2].push_back(s.x);
    shd_geom[2].push_back(s.y);
    shd_geom[2].push_back(s.z);
    shd_geom[2].push_back(rgb.r);
    shd_geom[2].push_back(rgb.g);
    shd_geom[2].push_back(rgb.b);
    sett->geom[2]++;
}
void BoxObj::scale(GeomObject*& C)
{
    C = new BoxObj(sett->scale*pos, sett->scale*s, al);
}
std::vector<sf::Vector3f> BoxObj::collision(sf::Vector3f pls)
{
    std::vector<sf::Vector3f> p, q;
    for (int i = 0; i < 12; i++)
    {
        q=obj[i].collision(pos);
        for(auto e: q){p.push_back(e);}
    }
    return p;
}
SET::vis_point BoxObj::intersect(sf::Vector3f rp, sf::Vector3f rv)
{
    Settings::vis_point ox{sett->len, rgb}, ot;
    for (int i = 0; i < 4; i++)
    {
        ot=obj[i].intersect(rp, rv);
        if(ot.dist<ox.dist) ox={ot.dist, rgb};
    }
    return ox;
}

void PlaneObj::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(obj, states);
}

PlaneObj::PlaneObj(sf::Vector3f _pos, sf::Vector3f _k): 
    GeomObject(_pos, sf::Color(100, 100, 100)), obj({sett->scale*sett->W, sett->scale*sett->H}), 
    nr(sett->ort(_k)), d(-sett->dot(sett->ort(_k),_pos))
{
    obj.setFillColor(rgb);
}
void PlaneObj::push(std::vector<std::vector<float>>& shd_geom)
{
    shd_geom[3].push_back(nr.x);
    shd_geom[3].push_back(nr.y);
    shd_geom[3].push_back(nr.z);
    shd_geom[3].push_back(d);
    shd_geom[3].push_back(rgb.r);
    shd_geom[3].push_back(rgb.g);
    shd_geom[3].push_back(rgb.b);
    sett->geom[3]++;
}
void PlaneObj::scale(GeomObject*& C)
{
    C = new PlaneObj(sett->scale*pos, nr);
}
std::vector<sf::Vector3f> PlaneObj::collision(sf::Vector3f pls)
{
    // sf::Vector3f o1=pls-trans(pos, sf::Vector3f(pos.x-a/2, pos.y-b/2), al);
    // sf::Vector3f o2=pls-trans(pos, sf::Vector3f(pos.x+a/2, pos.y-b/2), al);
    // sf::Vector3f o3=pls-trans(pos, sf::Vector3f(pos.x+a/2, pos.y+b/2), al);
    // sf::Vector3f o4=pls-trans(pos, sf::Vector3f(pos.x-a/2, pos.y+b/2), al);
    // sf::Vector3f v(std::cos(M_PI*al/180), std::sin(M_PI*al/180));
    // sf::Vector3f n=sett->norm(v);
    // float d = sett->dat(v, o1);
    // if(!(abs(d)>sett->size || sett->mod(o1)>sett->mod(a*v)+sett->size/3 || sett->mod(o2)>sett->mod(a*v)+sett->size/3)) return {sett->ort(n * sett->dot(n, o1))};
    // d = sett->dat(n, o2);
    // if(!(abs(d)>sett->size || sett->mod(o2)>sett->mod(b*n)+sett->size/3 || sett->mod(o3)>sett->mod(b*n)+sett->size/3)) return {sett->ort(v * sett->dot(v, o2))};
    // d = sett->dat(v, o3);
    // if(!(abs(d)>sett->size || sett->mod(o3)>sett->mod(a*v)+sett->size/3 || sett->mod(o4)>sett->mod(a*v)+sett->size/3)) return {sett->ort(n * sett->dot(n, o3))};
    // d = sett->dat(n, o4);
    // if(!(abs(d)>sett->size || sett->mod(o4)>sett->mod(b*n)+sett->size/3 || sett->mod(o1)>sett->mod(b*n)+sett->size/3)) return {sett->ort(v * sett->dot(v, o4))};
    return {};
}
SET::vis_point PlaneObj::intersect(sf::Vector3f rp, sf::Vector3f rv)
{
    SET::vis_point ox{sett->len, rgb};
    if(sett->dot(nr,rv)>=0) return ox;
    float h = abs((sett->dot(nr,rp)+d)/sett->dot(nr,rv));
    // std::cout<<d<<' '<<h<<' '<<rp.z<<"\n";
    ox.dist=std::min(h,ox.dist);
    return ox;
}

void RectObj::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(obj, states);
}

RectObj::RectObj(sf::Vector3f _pos, sf::Vector3f _s, sf::Vector3f _al): GeomObject(_pos, sf::Color(100, 0, 255)),  s(_s), al(_al), obj({_s.x, _s.y})
{
    obj.move({pos.x-s.x/2, pos.y-s.y/2});
    obj.setFillColor(rgb);
}

void RectObj::push(std::vector<std::vector<float>>& shd_geom)
{
    shd_geom[4].push_back(pos.x);
    shd_geom[4].push_back(pos.y);
    shd_geom[4].push_back(pos.z);
    shd_geom[4].push_back(s.x);
    shd_geom[4].push_back(s.y);
    shd_geom[4].push_back(s.z);
    shd_geom[4].push_back(rgb.r);
    shd_geom[4].push_back(rgb.g);
    shd_geom[4].push_back(rgb.b);
    sett->geom[4]++;
}
void RectObj::scale(GeomObject*& C)
{
    C = new RectObj(sett->scale*pos, sett->scale*s, al);
}
std::vector<sf::Vector3f> RectObj::collision(sf::Vector3f pls)
{
    // sf::Vector3f o1=pls-trans(pos, sf::Vector3f(pos.x-a/2, pos.y-b/2), al);
    // sf::Vector3f o2=pls-trans(pos, sf::Vector3f(pos.x+a/2, pos.y-b/2), al);
    // sf::Vector3f o3=pls-trans(pos, sf::Vector3f(pos.x+a/2, pos.y+b/2), al);
    // sf::Vector3f o4=pls-trans(pos, sf::Vector3f(pos.x-a/2, pos.y+b/2), al);
    // sf::Vector3f v(std::cos(M_PI*al/180), std::sin(M_PI*al/180));
    // sf::Vector3f n=sett->norm(v);
    // float d = sett->dat(v, o1);
    // if(!(abs(d)>sett->size || sett->mod(o1)>sett->mod(a*v)+sett->size/3 || sett->mod(o2)>sett->mod(a*v)+sett->size/3)) return {sett->ort(n * sett->dot(n, o1))};
    // d = sett->dat(n, o2);
    // if(!(abs(d)>sett->size || sett->mod(o2)>sett->mod(b*n)+sett->size/3 || sett->mod(o3)>sett->mod(b*n)+sett->size/3)) return {sett->ort(v * sett->dot(v, o2))};
    // d = sett->dat(v, o3);
    // if(!(abs(d)>sett->size || sett->mod(o3)>sett->mod(a*v)+sett->size/3 || sett->mod(o4)>sett->mod(a*v)+sett->size/3)) return {sett->ort(n * sett->dot(n, o3))};
    // d = sett->dat(n, o4);
    // if(!(abs(d)>sett->size || sett->mod(o4)>sett->mod(b*n)+sett->size/3 || sett->mod(o1)>sett->mod(b*n)+sett->size/3)) return {sett->ort(v * sett->dot(v, o4))};
    return {};
}
SET::vis_point RectObj::intersect(sf::Vector3f rp, sf::Vector3f rv)
{
    SET::vis_point ox{sett->len, rgb}; 
    rp-=pos;
    if((rv.y==0 && 2*abs(rp.y)>s.y) || (rv.z==0 && 2*abs(rp.z)>s.z)) return ox;
    sf::Vector3f n(rp.x/rv.x, rp.y/rv.y, rp.z/rv.z);
    sf::Vector3f k(0.5f*s.x/abs(rv.x), 0.5f*s.y/abs(rv.y), 0.5f*s.z/abs(rv.z));
    sf::Vector3f t2=-n-k;
    sf::Vector3f t1=-n+k;
    float ss=std::max(std::max(t2.x,t2.y),t2.z);
    float ff=std::min(std::min(t1.x,t1.y),t1.z);
    if(ff<ss || ff<0) return ox;
    // if(rv.y==0) std::cout<<t1.x<<' '<<t1.y<<' '<<t1.z<<"\n";
    // if(rv.y==0) std::cout<<t2.x<<' '<<t2.y<<' '<<t2.z<<"\n";
    ox.dist=std::min(ss,ox.dist);
    return ox;
}