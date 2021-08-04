#pragma once

#include <SDL_ttf.h>

#include "Scene.h"
#include "Laser.h"
#include "StillObject.h"

class StatusBar : public GameObject
{
    public:
        StatusBar(const Scene* scene,
            const Laser* laser, const std::vector<Obstacle*>* obstacles);
        virtual ~StatusBar();

        void update();
        void init();

        unsigned get_remaining() const;
        void refresh();
        void setSpriteSheet(SDL_Texture* sheet) {m_sprite_sheet = sheet;}

    private:
        SDL_Rect m_bar;
        bool m_won;
        const Laser* m_laser;
        const std::vector<Obstacle*>* m_obstacles;
        TTF_Font* m_font;
        SDL_Texture *m_texture, *m_sprite_sheet;
};
