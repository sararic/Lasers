#pragma once

#include <GameObject.h>
#define RAD 6

template<int SSX, int SSY> //position on the spritesheet
class StillObject : public GameObject
{
    public:

        StillObject(int x, int y):
            GameObject{x,y},
            m_square{x-RAD,y-RAD,2*RAD,2*RAD},
            m_ssx{SSX}, m_ssy{SSY}
        {}

        void update()
        {
            if(m_spriteSheet == nullptr)
                throw InitError{"StillObject: Set up a spritesheet before adding to the scene."};
            SDL_Rect sprite{m_ssx*34,m_ssy*34,34,34};
            SDL_RenderCopy(m_renderer, m_spriteSheet, &sprite, &m_square);
        }

        void setPos(int x, int y)
        {
            m_pos.first = x;
            m_pos.second = y;
            m_square.x = x-RAD;
            m_square.y = y-RAD;
        }

        void setSpriteSheet(SDL_Texture* sheet) {m_spriteSheet = sheet;}

        void setSpritePos(int ssx, int ssy) {m_ssx = ssx; m_ssy = ssy;}

    protected:
    SDL_Texture* m_spriteSheet;
    SDL_Rect m_square;
    int m_ssx, m_ssy;
};

class Target : public StillObject<0,1>
{
    public:
        Target(int x, int y):
            StillObject<0,1>{x,y} {}

        void setHit(bool is_hit) {m_ssx = !is_hit;}
        bool isHit() const {return !m_ssx;}
};

typedef StillObject<0,0> Shooter;
typedef StillObject<1,0> Obstacle;
