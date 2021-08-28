#pragma once

#include <GameObject.h>


class Background : public GameObject
{
    public:
        Background(int width, int height);
        virtual ~Background();

        void update();
        void init();

    private:
        int m_width;
        int m_height;
        int a;
        SDL_Texture* m_texture;
};
