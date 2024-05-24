#include "Geometry.hpp"

ObjBase base;

GeomObject::GeomObject(sf::Vector3f _pos, sf::Color _rgb, sf::Vector3f _mat): pos(_pos), rgb(_rgb), mat(_mat)
{
    mat/=(mat.x+mat.y+mat.z);
}


void SphearObj::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(obj, states);
}

void SphearObj::push(std::vector<float>& shd_geom)
{
    shd_geom.push_back(pos.x);
    shd_geom.push_back(pos.y);
    shd_geom.push_back(pos.z);
    shd_geom.push_back(r);
    shd_geom.push_back(rgb.r);
    shd_geom.push_back(rgb.g);
    shd_geom.push_back(rgb.b);
    shd_geom.push_back(mat.x);
    shd_geom.push_back(mat.y);
    shd_geom.push_back(mat.z);
}
SphearObj::SphearObj(sf::Vector3f _pos, float _r, sf::Vector3f _mat): 
    GeomObject(_pos, sf::Color(0,0,255), _mat), r(_r), obj(_r*sett->scale)
{
    obj.move(sf::Vector2f(_pos.x-r, _pos.y-r)*sett->scale);
    obj.setFillColor(rgb);
}
std::vector<sf::Vector3f> SphearObj::collision(sf::Vector3f pls)
{
    float d = sett->mod(pls-pos) - r;
    if(d>sett->size) return {};
    return {sett->ort(pls-pos)};
}
float SphearObj::intersect(sf::Vector3f rp, sf::Vector3f rv)
{
    sf::Vector3f aov=pos-rp;
    float ah = sett->dot(aov, rv);
    float ao = sett->dot(aov,aov);
    float bh = r*r - ao + ah*ah;
    if(bh < 0 || ah<0) return sett->len;
    float h = sqrt(bh);
    float dc = 100+155*abs(sett->dot(sett->ort(sett->light), rv));
    // std::cout<<ah-h<<'\n';
    return std::min(ah - h, sett->len); //*sf::Color(dc,dc,dc)
}


void LineObj::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(obj, 2, sf::Lines, states);
}

LineObj::LineObj(float x1_, float y1_, float z1_, float x2_, float y2_, float z2_, sf::Vector3f _mat): 
    GeomObject(sf::Vector3f(x1_, y1_, z1_), sf::Color(220,220,0), _mat), 
    a(x1_, y1_, z1_), b(x2_, y2_, z2_)
{
    obj[0] = sf::Vertex(sf::Vector2f(a.x, a.y), rgb);
    obj[1] = sf::Vertex(sf::Vector2f(b.x, b.y), rgb);
}
LineObj::LineObj(sf::Vector3f _a, sf::Vector3f _b, sf::Vector3f _mat): 
    GeomObject(a, sf::Color(220,220,0), _mat), 
    a(_a), b(_b)
{
    obj[0] = sf::Vertex (sf::Vector2f(a.x, a.y), rgb);
    obj[1] = sf::Vertex (sf::Vector2f(b.x, b.y), rgb);
}
LineObj::LineObj(sf::Vector3f _pos, float l, sf::Vector3f al, sf::Vector3f _mat): 
    GeomObject(a, sf::Color(220,220,0), _mat)
{
    *this = LineObj(_pos, _pos + sett->trans(al)*l, _mat);
}

void LineObj::push(std::vector<float>& shd_geom)
{
    shd_geom.push_back(pos.x);
    shd_geom.push_back(pos.y);
    shd_geom.push_back(pos.z);
    shd_geom.push_back(a.x);
    shd_geom.push_back(rgb.r);
    shd_geom.push_back(rgb.g);
    shd_geom.push_back(rgb.b);
    shd_geom.push_back(mat.x);
    shd_geom.push_back(mat.y);
    shd_geom.push_back(mat.z);
}
std::vector<sf::Vector3f> LineObj::collision(sf::Vector3f pls)
{
    sf::Vector3f v=b-a, ax = pls-a;
    float d = sett->mod(sett->dat(sett->ort(v), ax));
    if(abs(d)>sett->size || sett->mod(ax)>sett->mod(v) || sett->mod(pls-b)>sett->mod(v)) return {};
    sf::Vector3f n=sett->norm(v);
    return {sett->ort(n * sett->dot(n, ax))};
}
float LineObj::intersect(sf::Vector3f rp, sf::Vector3f rv)
{
    sf::Vector3f v=sett->ort(b-a);
    sf::Vector3f n=sett->norm(v);
    float pa = sett->dot(n, rv);
    float h = sett->mod(sett->dat(v, rp-pos));
    sf::Vector3f x = rp + rv*abs(h/pa);
    if(sett->lin(a, b, x) && sett->len>=abs(h/pa))
    {
        float dc = 155+100*abs(pa); 
        return abs(h/pa);
    }
    return sett->len;
}


void BoxObj::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    for (int i = 0; i < 12; i++) target.draw(obj[i], states);
}
BoxObj::BoxObj(sf::Vector3f _pos, sf::Vector3f _s, sf::Vector3f _al, sf::Vector3f _mat): 
    GeomObject(_pos, sf::Color(0,220,220), _mat), s(_s), al(_al)
{
    obj.push_back(LineObj(sett->trans(pos, pos-(0.5f*s), al), sett->trans(pos, pos+(0.5f*s), al), _mat));
}

void BoxObj::push(std::vector<float>& shd_geom)
{
    shd_geom.push_back(pos.x);
    shd_geom.push_back(pos.y);
    shd_geom.push_back(pos.z);
    shd_geom.push_back(s.x);
    shd_geom.push_back(s.y);
    shd_geom.push_back(s.z);
    shd_geom.push_back(rgb.r);
    shd_geom.push_back(rgb.g);
    shd_geom.push_back(rgb.b);
    shd_geom.push_back(mat.x);
    shd_geom.push_back(mat.y);
    shd_geom.push_back(mat.z);
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
float BoxObj::intersect(sf::Vector3f rp, sf::Vector3f rv)
{
    float ox = sett->len, ot;
    for (int i = 0; i < 4; i++)
    {
        ot=obj[i].intersect(rp, rv);
        ox = std::min(ox,ot);
    }
    return ox;
}

void PlaneObj::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(obj, states);
}

PlaneObj::PlaneObj(sf::Vector3f _pos, sf::Vector3f _k, sf::Vector3f _mat): 
    GeomObject(_pos, sf::Color(70, 70, 70), _mat), obj({sett->scale*sett->W, sett->scale*sett->H}), 
    nr(sett->ort(_k)), d(-sett->dot(sett->ort(_k),_pos))
{
    obj.setFillColor(rgb);
}
void PlaneObj::push(std::vector<float>& shd_geom)
{
    shd_geom.push_back(nr.x);
    shd_geom.push_back(nr.y);
    shd_geom.push_back(nr.z);
    shd_geom.push_back(d);
    shd_geom.push_back(rgb.r);
    shd_geom.push_back(rgb.g);
    shd_geom.push_back(rgb.b);
    shd_geom.push_back(mat.x);
    shd_geom.push_back(mat.y);
    shd_geom.push_back(mat.z);
}
std::vector<sf::Vector3f> PlaneObj::collision(sf::Vector3f pls)
{
    float h = (sett->dot(nr,pls)+d);
    if(h>sett->size) return {};
    return{nr*(1.f-2.f*(h<0))};
}
float PlaneObj::intersect(sf::Vector3f rp, sf::Vector3f rv)
{
    float ox = sett->len;
    if(sett->dot(nr,rv)>=0) return ox;
    float h = abs((sett->dot(nr,rp)+d)/sett->dot(nr,rv));
    // std::cout<<d<<' '<<h<<' '<<rp.z<<"\n";
    ox = std::min(h, ox);
    return ox;
}

void CubeObj::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;
    target.draw(obj, states);
}

CubeObj::CubeObj(sf::Vector3f _pos, sf::Vector3f _s, sf::Vector3f _al, sf::Vector3f _mat): 
    GeomObject(_pos, sf::Color(100, 0, 255), _mat),  s(_s), al(_al), obj({_s.x*sett->scale, _s.y*sett->scale})
{
    obj.move(sf::Vector2f(pos.x-s.x/2, pos.y-s.y/2)*sett->scale);
    obj.setFillColor(rgb);
}

void CubeObj::push(std::vector<float>& shd_geom)
{
    shd_geom.push_back(pos.x);
    shd_geom.push_back(pos.y);
    shd_geom.push_back(pos.z);
    shd_geom.push_back(s.x);
    shd_geom.push_back(s.y);
    shd_geom.push_back(s.z);
    shd_geom.push_back(rgb.r);
    shd_geom.push_back(rgb.g);
    shd_geom.push_back(rgb.b);
    shd_geom.push_back(mat.x);
    shd_geom.push_back(mat.y);
    shd_geom.push_back(mat.z);
}
std::vector<sf::Vector3f> CubeObj::collision(sf::Vector3f pls)
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
float CubeObj::intersect(sf::Vector3f rp, sf::Vector3f rv)
{
    float ox = sett->len; 
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
    ox = std::min(ss, ox);
    return ox;
}





ObjBase::ObjBase(): geom(geom_kol), cash_geom(geom_kol) {}

void ObjBase::createSphear(sf::Vector3f _pos, float _r, sf::Vector3f _mat)
{
    objects.push_back(new SphearObj(_pos, _r, _mat));
    objects[objects.size()-1]->push(cash_geom[0]);
    geom[0]++;
}
void ObjBase::createPlane(sf::Vector3f _pos, sf::Vector3f _k, sf::Vector3f _mat)
{
    objects.push_back(new PlaneObj(_pos, _k, _mat));
    objects[objects.size()-1]->push(cash_geom[3]);
    geom[3]++;
}
void ObjBase::createCube(sf::Vector3f _pos, sf::Vector3f _s, sf::Vector3f _al, sf::Vector3f _mat)
{
    objects.push_back(new CubeObj(_pos, _s, _al, _mat));
    objects[objects.size()-1]->push(cash_geom[4]);
    geom[4]++;
}

int ObjBase::get_kol()
{
    return objects.size();
}
std::vector<float> ObjBase::get_geom()
{
    return geom;
}
std::vector<float> ObjBase::get_cash(int i)
{
    return cash_geom[i];
}
GeomObject* ObjBase::get_obj(int i)
{
    return objects[i];
}

ObjBase::~ObjBase()
{
    for (int i = 0; i < objects.size(); i++){delete objects[i];}
}