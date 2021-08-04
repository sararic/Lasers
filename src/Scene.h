#pragma once

#include <vector>

#include "GameObject.h"

class Scene
{
private:
    SDL_Window * m_window;
    SDL_Renderer * m_renderer;
    std::vector<GameObject*> m_objs, m_queue, m_to_be_deleted;
    int m_dt, m_w, m_h;

public:
    Scene(int win_width, int win_height,
          std::string title, int delta_t, Uint32 flags = 0);
    virtual ~Scene();

    int next_frame();
    void play();

    int w() const;
    int h() const;
    int dt() const;
    bool contains(GameObject const& obj) const;

    void add(GameObject* obj, GameObject* in_front = nullptr);
    void remove(GameObject* obj, bool delete_obj=false);
};
