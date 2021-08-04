#pragma once

#include <GameObject.h>


class Background : public GameObject
{
    public:
        Background(int width, int height);

        void update();

    private:
        int m_width;
        int m_height;
        int a;
};
