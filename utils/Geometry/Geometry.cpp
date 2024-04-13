#include "Geometry.hpp"

float GeomObject::mod(sf::Vector2f pos){return std::sqrt(pos.x*pos.x + pos.y*pos.y);}
sf::Vector2f GeomObject::ort(sf::Vector2f pos){return sf::Vector2f(pos.x/mod(pos), pos.y/mod(pos));}
sf::Vector2f GeomObject::norm(sf::Vector2f pos){return ort(sf::Vector2f(pos.y, -pos.x));}
float GeomObject::dot(sf::Vector2f a, sf::Vector2f b) {return a.x*b.x + a.y*b.y;}
float GeomObject::dat(sf::Vector2f a, sf::Vector2f b) {return a.x*b.y - a.y*b.x;}
bool GeomObject::lin(sf::Vector2f a, sf::Vector2f b, sf::Vector2f x) {return mod(a-x)+mod(x-b)-mod(a-b)<=0.0001;}
sf::Vector2f GeomObject::trans(sf::Vector2f o, sf::Vector2f a, float al)
{
    al = M_PI*al/180;
    return sf::Vector2f(o.x+(a.x-o.x)*std::cos(al)-(a.y-o.y)*std::sin(al), o.y+(a.x-o.x)*std::sin(al)+(a.y-o.y)*std::cos(al));
}

GeomObject::GeomObject(sf::Vector2f pos_, sf::Color rgb_): pos(pos_), rgb(rgb_) {}


void SphearObj::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(obj, states);
}

void SphearObj::scale(GeomObject*& c, float p)
{
    c = new SphearObj(p*pos, p*r);
}
SphearObj::SphearObj(sf::Vector2f pos_, float size): GeomObject(pos_, sf::Color::Blue), r(size), obj(size)
{
    obj.move(sf::Vector2f(pos_.x-r, pos_.y-r));
    obj.setFillColor(rgb);
}
std::vector<sf::Vector2f> SphearObj::collision(sf::Vector2f pls, float size)
{
    float d = mod(pls-pos) - r;
    if(d>size) return {};
    return {ort(pls-pos)};
}
float SphearObj::intersect(sf::Vector2f rp, sf::Vector2f rv, float rl)
{
    sf::Vector2f aov=pos-rp;
    float ah = dot(aov, rv);
    float ao = dot(aov,aov);
    float bh = r*r - ao + ah*ah;
    if(bh < 0 || ah<0) return rl;
    float h = sqrt(bh);
    return std::min(ah - h, rl);
}


void LineObj::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(obj, 2, sf::Lines, states);
}

LineObj::LineObj(float a1_, float b1_, float a2_, float b2_): 
    GeomObject(sf::Vector2f(a1_, b1_), sf::Color(220,220,0)), 
    a1(a1_), b1(b1_), a2(a2_), b2(b2_)
{
    obj[0] = sf::Vertex (sf::Vector2f(a1, b1), rgb);
    obj[1] = sf::Vertex (sf::Vector2f(a2, b2), rgb);
}
LineObj::LineObj(sf::Vector2f pos_, float l, float al): 
    GeomObject(pos_, sf::Color(220,220,0)) 
{
    *this = LineObj(pos_.x, pos_.y, pos_.x + l*std::cos(M_PI*al/180), pos_.y + l*std::sin(M_PI*al/180));
}

void LineObj::scale(GeomObject*& c, float p)
{
    c = new LineObj(p*a1, p*b1, p*a2, p*b2);
}
std::vector<sf::Vector2f> LineObj::collision(sf::Vector2f pls, float size)
{
    sf::Vector2f v=sf::Vector2f(a2-a1, b2-b1), ax = pls-obj[0].position;
    float d = dat(ort(v), ax);
    if(abs(d)>size || mod(ax)>mod(v) || mod(pls-obj[1].position)>mod(v)) return {};
    sf::Vector2f n=norm(v);
    return {ort(n * dot(n, ax))};
}
float LineObj::intersect(sf::Vector2f rp, sf::Vector2f rv, float rl)
{
    sf::Vector2f v=ort(sf::Vector2f(a2-a1, b2-b1));
    sf::Vector2f n=norm(v);
    float pa = dot(n, rv);
    float h = dat(v, rp-pos);
    sf::Vector2f x = rp + rv*abs(h/pa);
    if(lin(sf::Vector2f(a1, b1), sf::Vector2f(a2, b2), x) && rl>=abs(h/pa)) {return abs(h/pa);}
    return rl;
}


void BoxObj::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(obj, 5, sf::LinesStrip, states);
}
BoxObj::BoxObj(sf::Vector2f pos_, float a_, float b_, float al_): GeomObject(pos_, sf::Color(0,220,220)), a(a_), b(b_), al(al_)
{
    obj[0] = sf::Vertex (trans(pos, sf::Vector2f(pos.x-a/2, pos.y-b/2), al), rgb);
    obj[1] = sf::Vertex (trans(pos, sf::Vector2f(pos.x+a/2, pos.y-b/2), al), rgb);
    obj[2] = sf::Vertex (trans(pos, sf::Vector2f(pos.x+a/2, pos.y+b/2), al), rgb);
    obj[3] = sf::Vertex (trans(pos, sf::Vector2f(pos.x-a/2, pos.y+b/2), al), rgb);
    obj[4] = sf::Vertex (trans(pos, sf::Vector2f(pos.x-a/2, pos.y-b/2), al), rgb);
}

void BoxObj::scale(GeomObject*& c, float p)
{
    c = new BoxObj(p*pos, p*a, p*b, al);
}
std::vector<sf::Vector2f> BoxObj::collision(sf::Vector2f pls, float size)
{
    float d, s;
    sf::Vector2f ax, v(std::cos(M_PI*al/180), std::sin(M_PI*al/180)), n=norm(v);
    std::vector<sf::Vector2f> p;
    for (int i = 0; i < 4; i++)
    {
        s = a - (a-b)*(i%2);
        ax = pls-obj[i].position;
        d = dat(v, ax);
        if(!(abs(d)>size || mod(ax)>mod(s*v)+size/3 || mod(pls-obj[i+1].position)>mod(s*v)+size/3)) p.push_back(ort(n * dot(n, ax)));
        std::swap(v, n); 
    }
    return p;
}
float BoxObj::intersect(sf::Vector2f rp, sf::Vector2f rv, float rl)
{
    float pa, h, ox = rl;
    sf::Vector2f o, x, v(std::cos(M_PI*al/180), std::sin(M_PI*al/180)), n=norm(v);
    for (int i = 0; i < 4; i++)
    {
        o=obj[i].position;
        pa = dot(n, rv);
        h = dat(v, rp-o);
        x = rp + rv*abs(h/pa);
        if(lin(o, obj[i+1].position, x) && rl>=abs(h/pa)) {ox=std::min(ox, abs(h/pa));}
        std::swap(v, n);
    }
    return ox;
}


void RectObj::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(obj, states);
}

RectObj::RectObj(sf::Vector2f pos_, float a_, float b_, float al_): GeomObject(pos_, sf::Color::Blue), obj(sf::Vector2f(a_, b_)), a(a_), b(b_), al(al_)
{
    obj.move(trans(pos, sf::Vector2f(pos.x-a/2, pos.y-b/2), al));
    obj.rotate(al);
    obj.setFillColor(rgb);
}

void RectObj::scale(GeomObject*& c, float p)
{
    c = new RectObj(p*pos, p*a, p*b, al);
}
std::vector<sf::Vector2f> RectObj::collision(sf::Vector2f pls, float size)
{
    sf::Vector2f o1=pls-trans(pos, sf::Vector2f(pos.x-a/2, pos.y-b/2), al);
    sf::Vector2f o2=pls-trans(pos, sf::Vector2f(pos.x+a/2, pos.y-b/2), al);
    sf::Vector2f o3=pls-trans(pos, sf::Vector2f(pos.x+a/2, pos.y+b/2), al);
    sf::Vector2f o4=pls-trans(pos, sf::Vector2f(pos.x-a/2, pos.y+b/2), al);
    sf::Vector2f v(std::cos(M_PI*al/180), std::sin(M_PI*al/180));
    sf::Vector2f n=norm(v);
    float d = dat(v, o1);
    if(!(abs(d)>size || mod(o1)>mod(a*v)+size/3 || mod(o2)>mod(a*v)+size/3)) return {ort(n * dot(n, o1))};
    // if(abs(d)<20) std::cout<<d<<" "<<d<<" "<<ax.x<<" "<<ax.y<<'\n';
    d = dat(n, o2);
    if(!(abs(d)>size || mod(o2)>mod(b*n)+size/3 || mod(o3)>mod(b*n)+size/3)) return {ort(v * dot(v, o2))};
    d = dat(v, o3);
    if(!(abs(d)>size || mod(o3)>mod(a*v)+size/3 || mod(o4)>mod(a*v)+size/3)) return {ort(n * dot(n, o3))};
    d = dat(n, o4);
    if(!(abs(d)>size || mod(o4)>mod(b*n)+size/3 || mod(o1)>mod(b*n)+size/3)) return {ort(v * dot(v, o4))};
    return {};
}
float RectObj::intersect(sf::Vector2f rp, sf::Vector2f rv, float rl)
{
    float ox = rl;
    sf::Vector2f o=trans(pos, sf::Vector2f(pos.x-a/2, pos.y-b/2), al);
    sf::Vector2f v(std::cos(M_PI*al/180), std::sin(M_PI*al/180));
    sf::Vector2f n=norm(v);
    float pa = dot(n, rv);
    float h = dat(v, rp-o);
    sf::Vector2f x = rp + rv*abs(h/pa);
    if(lin(o, trans(o, sf::Vector2f(o.x+a, o.y), al), x) && rl>=abs(h/pa)) {ox=std::min(ox, abs(h/pa));}
    pa = dot(v, rv);
    h = dat(n, rp-o);
    x = rp + rv*abs(h/pa);
    if(lin(o, trans(o, sf::Vector2f(o.x, o.y+b), al), x) && rl>=abs(h/pa)) {ox=std::min(ox, abs(h/pa));}
    o=trans(pos, sf::Vector2f(pos.x+a/2, pos.y+b/2), al);
    pa = dot(n, rv);
    h = dat(v, rp-o);
    x = rp + rv*abs(h/pa);
    if(lin(o, trans(o, sf::Vector2f(o.x-a, o.y), al), x) && rl>=abs(h/pa)) {ox=std::min(ox, abs(h/pa));}
    pa = dot(v, rv);
    h = dat(n, rp-o);
    x = rp + rv*abs(h/pa);
    if(lin(o, trans(o, sf::Vector2f(o.x, o.y-b), al), x) && rl>=abs(h/pa)) {ox=std::min(ox, abs(h/pa));}
    return ox;
}