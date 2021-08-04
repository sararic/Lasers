#include "GameManager.h"
#include <SDL_image.h>

#define PXL 8

GameManager::GameManager(Scene* scene):
    GameObject{0,0},
    rnd_gen{rd()},
    m_width{scene->w()},
    m_height{scene->h()-100},
    m_bg{m_width, m_height},
    m_dots_surface{nullptr},
    m_dots_texture{nullptr},
    m_scene{scene},
    m_shooter{0,0},
    m_target{0,0},
    m_obstacles{},
    m_laser{scene->w(), scene->h()-100, &m_shooter, &m_target, &m_obstacles},
    m_statbar{scene, &m_laser, &m_obstacles},
    m_button{scene->w() - 140, scene->h() - 70, this, "show solution"},
    m_dragged_obj{nullptr},
    m_drag_initial{-1,-1}
{
    m_dots_surface = IMG_Load((RESOURCES+"dots.png").c_str());
    if(m_dots_surface == nullptr) throw InitError{InitError::IMG};

    m_scene->add(&m_bg);
    m_scene->add(&m_laser);
    m_scene->add(&m_shooter);
    m_scene->add(&m_target);
    place_dots();
    m_scene->add(&m_statbar);
    m_scene->add(&m_button);
    m_scene->add(this);
}

GameManager::~GameManager()
{
    if(m_dots_texture) SDL_DestroyTexture(m_dots_texture);
    if(m_dots_surface) SDL_FreeSurface(m_dots_surface);
    for(auto obs : m_obstacles) delete obs;
}

void GameManager::init()
{
    if(m_dots_texture) return;
    m_dots_texture = SDL_CreateTextureFromSurface(
        m_renderer, m_dots_surface);
    if(m_dots_texture == nullptr) throw InitError{};

    m_shooter.setSpriteSheet(m_dots_texture);
    m_target.setSpriteSheet(m_dots_texture);
    m_statbar.setSpriteSheet(m_dots_texture);
}

int GameManager::manage_event(SDL_Event const& e)
{
    if(e.type == SDL_MOUSEBUTTONDOWN // add an obstacle or start drag motion
       && e.button.button == SDL_BUTTON_LEFT
       && e.button.y < m_statbar.y())
    {
        int x{e.button.x};
        int y{e.button.y};

        for(auto obs : m_obstacles)
            if((x - obs->x())*(x - obs->x()) + (y - obs->y())*(y - obs->y()) < RAD*RAD)
            {
                m_dragged_obj = obs;
                m_drag_initial.first = x;
                m_drag_initial.second = y;
                return 0;
            }

        if(m_statbar.get_remaining()){
            m_obstacles.push_back(new Obstacle{x,y});
            m_dragged_obj = m_obstacles.back();
            m_dragged_obj->setSpriteSheet(m_dots_texture);
            m_scene->add(m_dragged_obj, &m_statbar);
        }
    }

    else if(e.type == SDL_MOUSEBUTTONUP // place the obstacle, refresh the laser
         && e.button.button == SDL_BUTTON_LEFT
         && m_dragged_obj != nullptr)
    {
        if(m_drag_initial.first >= 0)
        {
            if(e.button.x == m_drag_initial.first
            && e.button.y == m_drag_initial.second) // we remove the obstacle
            {
                for(unsigned i{0} ; i < m_obstacles.size() ; i++)
                    if(m_dragged_obj == m_obstacles.at(i))
                        m_obstacles.erase(m_obstacles.begin() + i);
                m_scene->remove(m_dragged_obj, true);
                m_dragged_obj = nullptr;
                m_drag_initial.first = -1;
                refresh_all();
                return 0;
            }
            m_drag_initial.first = -1;
        }

        int x{PXL*std::round(float{m_dragged_obj->x()}/PXL)};
        int y{PXL*std::round(float{m_dragged_obj->y()}/PXL)};

        // avoid covering existing objects
        if(x == m_shooter.x() && y == m_shooter.y()
        || x == m_target.x() && y == m_target.y())
            x += PXL;
        for(auto obs : m_obstacles) if(
            m_dragged_obj != obs && x == obs->x() && y == obs->y())
            x += PXL;

        m_dragged_obj->setPos(x, y);
        m_dragged_obj = nullptr;

        if(!m_laser.can_hit_target()) // we won!
            m_target.setHit(false);

        refresh_all();
    }

    else if(e.type == SDL_MOUSEMOTION && m_dragged_obj) // drag animation
    {
        int x{e.motion.x};
        int y{std::min(e.motion.y, m_height)};

        m_dragged_obj->setPos(x,y);
    }

    return 0;
}

void GameManager::refresh_all()
{
    m_laser.refresh_trajectory();
    m_statbar.refresh();
    if(m_laser.can_hit_target()) {
        m_button.setLabel("show solution");
        m_target.setHit(true);
    }
    else {
        m_button.setLabel("play again");
        m_target.setHit(false);
    }
}

void GameManager::place_dots()
{
    std::uniform_int_distribution<int> dist_x{1, m_width/(2*PXL) -1};
    std::uniform_int_distribution<int> dist_y{1, m_height/(2*PXL) -1};

    m_shooter.setPos(2*PXL*dist_x(rnd_gen), 2*PXL*dist_y(rnd_gen));

    do { m_target.setPos(2*PXL*dist_x(rnd_gen), 2*PXL*dist_y(rnd_gen));
    } while(m_shooter.pos() == m_target.pos());

    m_laser.refresh_trajectory();
}

void GameManager::handle_button()
{
    if(m_laser.can_hit_target()){ // show solution
        std::set<std::pair<int, int>> sol{m_laser.get_solution()};
        while(m_obstacles.size() < sol.size()) {
            m_obstacles.push_back(new Obstacle{0,0});
            m_obstacles.back()->setSpriteSheet(m_dots_texture);
            m_scene->add(m_obstacles.back(), &m_statbar);
        }
        unsigned i{0};
        for(auto pos : sol) {
            m_obstacles[i]->setPos(pos.first, pos.second);
            i++;
        }
        refresh_all();
    }
    else { // new game
        for(auto obs : m_obstacles){
            m_scene->remove(obs, true);
        }
        m_obstacles = {};
        place_dots();
        refresh_all();
    }
}
