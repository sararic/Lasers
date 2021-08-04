#pragma once

#include <random>
#include <vector>

#include "Scene.h"
#include "Background.h"
#include "StillObject.h"
#include "Laser.h"
#include "StatusBar.h"
#include "Button.h"

class GameManager : public GameObject, public ButtonHandler
{
    public:
        GameManager(Scene* scene);
        ~GameManager();
        GameManager(GameManager const& c)=delete;
        GameManager& operator=(GameManager const& c)=delete;

        void handle_button();

        void update(){}
        int manage_event(SDL_Event const& e);
        void init();

        void refresh_all();

    private:
        void place_dots();

        std::random_device rd;
        std::default_random_engine rnd_gen;
        int m_width, m_height;

        SDL_Surface* m_dots_surface;
        SDL_Texture* m_dots_texture;

        Scene* m_scene;
        Background m_bg;
        Shooter m_shooter;
        Target m_target;
        std::vector<Obstacle*> m_obstacles;
        Laser m_laser;
        StatusBar m_statbar;
        Button m_button;

        Obstacle* m_dragged_obj;
        std::pair<int,int> m_drag_initial;
};
