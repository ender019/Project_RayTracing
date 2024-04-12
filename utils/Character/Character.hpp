#include "Geometry.hpp"

class Character : public sf::Drawable, public sf::Transformable
{
private:
    sf::CircleShape camera;
    std::vector<sf::Vector2f> rays;
    sf::VertexArray conture;
    sf::Vector2f pos;
    float nal;
    float feeling = 2;
    int ray_kol;
    float dist;
    float vis_al;

protected: 
    float mod(sf::Vector2f pos);
    float dot(sf::Vector2f a, sf::Vector2f b);
    float dat(sf::Vector2f a, sf::Vector2f b);
    void rot(sf::Vector2f& a, float al);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    
public:
    Character(sf::Vector2f pos_, float kol_, float dist_, float al_=90, float nal_=-90);

    void rotate(float w);
    void move(std::vector<GeomObject*> objects, int p);
    std::vector<float> tracing(std::vector<GeomObject*> objects);
};