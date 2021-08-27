#include "Scene.h"
#include <SDL_ttf.h>
#include <SDL_image.h>

Scene::Scene(int win_width, int win_height,
             std::string title, int delta_t, Uint32 flags):
    m_window{nullptr}, m_renderer{nullptr},
    m_dt{delta_t}, m_w{win_width}, m_h{win_height}
{
    if ( SDL_Init( flags ) != 0 )
        throw InitError{};

    if ( !( IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG ) )
        throw InitError{InitError::IMG};

    if ( TTF_Init() != 0 )
        throw InitError{InitError::TTF};

    m_window = SDL_CreateWindow("Lasers!",
                                SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
                                win_width, win_height,
                                SDL_WINDOW_HIDDEN);
    if(m_window == nullptr) throw InitError{};

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if(m_renderer == nullptr) throw InitError{};
}

Scene::~Scene()
{
    SDL_DestroyWindow( m_window );
    SDL_DestroyRenderer( m_renderer );

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int Scene::w() const {return m_w;}
int Scene::h() const {return m_h;}
int Scene::dt() const {return m_dt;}

bool Scene::contains(GameObject const& obj) const
{
    for(auto i : m_objs) if(i == &obj) return true;
    return false;
}

int Scene::next_frame()
{
    SDL_Event e;
    SDL_SetRenderDrawColor(m_renderer,0,0,0,0);
    SDL_RenderClear( m_renderer );

    while(SDL_PollEvent(&e))
        switch(e.type)
        {
            case SDL_QUIT:
                return 0;
            default:
                for(auto i : m_objs)
                    if(i->manage_event(e)) return 0;
                break;
        };

    for(auto i : m_objs) i->update();

    for(auto i : m_to_be_deleted) delete i;
    m_to_be_deleted = {};
    m_objs = m_queue; // copy the (possibly) modified list

    SDL_RenderPresent( m_renderer );
    return 1;
}

void Scene::play()
{
    SDL_ShowWindow(m_window);
    while(next_frame()) {SDL_Delay(m_dt);}
}

void Scene::add(GameObject* obj, GameObject* in_front)
// we add and remove on the queue, so that we do not
// disturb the main m_obs  vector during polling.
{
    unsigned long i{0};
    while(i < m_queue.size() && m_queue.at(i) != in_front) i++;
    if(i == m_queue.size()) m_queue.push_back(obj);
    else m_queue.insert(m_queue.begin()+i, obj);

    if(obj->hasRenderer()) return;
    obj->setRenderer(m_renderer);
    obj->init();
}

void Scene::remove(GameObject* obj, bool delete_obj)
{
    for(unsigned i{0} ; i < m_queue.size() ; i++)
        if(obj == m_queue.at(i))
            m_queue.erase (m_queue.begin() + i);
    if(delete_obj)
        m_to_be_deleted.push_back(obj);
}
